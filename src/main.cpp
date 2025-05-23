#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

// --- Helper Functions ---
void typeKey(uint8_t key) {
  Keyboard.press(key);
  delay(50); // Small delay for key press
  Keyboard.release(key);
  delay(50); // Delay after release
}

void modifierKey(uint8_t modifier, char key) {
  Keyboard.press(modifier);
  Keyboard.press(key);
  delay(100);
  Keyboard.releaseAll();
  delay(100);
}

void typeString(const char* str) {
  Keyboard.print(str);
  // Adjusted delay for potentially faster typing, test this
  for (size_t i = 0; i < strlen(str); ++i) {
    delay(30); // Shorter delay per character
  }
  delay(100); // Small delay after typing string
}

// --- macOS Payload Functions ---
#if defined(TARGET_MACOS)
void macos_net_config() {
  Serial.println("  Executing Script: Network Config (ifconfig)");

  // 1. Open Spotlight (Command + Space)
  Serial.println("    Opening Spotlight...");
  modifierKey(KEY_LEFT_GUI, ' '); // KEY_LEFT_GUI is Command on Mac
  delay(700); // Wait for Spotlight

  // 2. Type "Terminal"
  Serial.println("    Typing Terminal...");
  typeString("Terminal");
  delay(700);

  // 3. Press Enter
  Serial.println("    Pressing Enter to open Terminal...");
  typeKey(KEY_RETURN);
  delay(1500); // Wait for Terminal to open and become active

  // 4. Type "ifconfig"
  Serial.println("    Typing ifconfig...");
  typeString("ifconfig");
  delay(200);

  // 5. Press Enter
  Serial.println("    Pressing Enter to run ifconfig...");
  typeKey(KEY_RETURN);
}

void payload_macos_open_calculator() {
  Serial.println("  Executing Script: Open Calculator");

  Serial.println("    Opening Spotlight...");
  modifierKey(KEY_LEFT_GUI, ' ');
  delay(700);
  typeString("Calculator");
  delay(700);
  typeKey(KEY_RETURN);
  delay(1000); // Wait for Calculator
}
#endif // TARGET_MACOS

// --- Windows Payload Functions ---
#if defined(TARGET_WINDOWS)
void payload_windows_net_config() {
  Serial.println("  Executing Script: Network Config (ipconfig)");

  // 1. Open Run Dialog (Windows Key + R)
  Serial.println("    Opening Run dialog...");
  modifierKey(KEY_LEFT_GUI, 'r'); // KEY_LEFT_GUI is often Windows Key too
  delay(700); // Wait for Run dialog

  // 2. Type "cmd"
  Serial.println("    Typing cmd...");
  typeString("cmd");
  delay(500);

  // 3. Press Enter
  Serial.println("    Pressing Enter to open Command Prompt...");
  typeKey(KEY_RETURN);
  delay(1500); // Wait for Command Prompt to open and become active

  // 4. Type "ipconfig"
  Serial.println("    Typing ipconfig...");
  typeString("ipconfig");
  delay(200);

  // 5. Press Enter
  Serial.println("    Pressing Enter to run ipconfig...");
  typeKey(KEY_RETURN);
}

void payload_windows_open_notepad() {
  Serial.println("  Executing Script: Open Notepad");
  Serial.println("    Opening Run dialog...");
  modifierKey(KEY_LEFT_GUI, 'r');
  delay(700);
  typeString("notepad");
  delay(500);
  typeKey(KEY_RETURN);
  delay(1000); // Wait for Notepad
}
#endif // TARGET_WINDOWS

// --- Linux Payload Functions (Example Structure) ---
#if defined(TARGET_LINUX)
void payload_linux_net_config() {
  Serial.println("  Executing Script: Network Config (ip addr)");
  // Example: Open terminal (Ctrl+Alt+T is common)
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  delay(100);
  Keyboard.releaseAll();
  delay(1500); // Wait for terminal

  typeString("ip addr");
  delay(200);
  typeKey(KEY_RETURN);
}
#endif // TARGET_LINUX


// --- Main Setup ---
void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for USB enumeration

  Keyboard.begin();
  USB.begin();

  Serial.println("Starting Rubber Ducky sequence in 5 seconds...");
  delay(5000); // Time to plug into the target machine

  Serial.print("Selected Target OS: ");
#if defined(TARGET_MACOS)
  Serial.println("macOS");
  // Dispatch to macOS specific scripts
  #if defined(SCRIPT_NET_CONFIG)
    macos_net_config();
  #elif defined(SCRIPT_OPEN_CALCULATOR)
    payload_macos_open_calculator();
  #else
    Serial.println("  No specific macOS script defined or script not recognized.");
  #endif

#elif defined(TARGET_WINDOWS)
  Serial.println("Windows");
  // Dispatch to Windows specific scripts
  #if defined(SCRIPT_NET_CONFIG)
    payload_windows_net_config();
  #elif defined(SCRIPT_OPEN_NOTEPAD) // Example of a different script
    payload_windows_open_notepad();
  #else
    Serial.println("  No specific Windows script defined or script not recognized.");
  #endif

#elif defined(TARGET_LINUX) // Example for Linux
  Serial.println("Linux");
  // Dispatch to Linux specific scripts
  #if defined(SCRIPT_NET_CONFIG)
    payload_linux_net_config();
  #else
    Serial.println("  No specific Linux script defined or script not recognized.");
  #endif

#else
  Serial.println("Undefined/Unknown OS Target. Halting.");
  Keyboard.end();
  while(1) delay(100); // Halt if no target OS is defined
#endif

  Serial.println("Ducky script finished.");
  Keyboard.end(); // Release HID
}

void loop() {
  // Nothing to do here for a one-shot script
  delay(10000); // Idle
}