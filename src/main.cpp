#include <Arduino.h>
#include "USB.h" // For ESP32-S3 USB capabilities
#include "USBHIDKeyboard.h" // Or a similar HID keyboard library

USBHIDKeyboard Keyboard;

// macOS Key Codes (approximate, refer to HID usage tables for exact codes)
// You might need to use raw HID codes for modifiers if KEY_LEFT_GUI isn't defined
// or directly supported by your chosen library.
// Many libraries provide defines like KEY_LEFT_GUI, KEY_SPACE, KEY_RETURN etc.

void typeKey(uint8_t key) {
  Keyboard.press(key);
  delay(50); // Small delay for key press
  Keyboard.release(key);
}

void typeString(const char* str) {
  Keyboard.print(str);
}

void setup() {
  Serial.begin(115200); // For debugging (optional, ensure CDC on boot is enabled)
  delay(1000); // Wait for USB enumeration

  // Initialize the Keyboard
  Keyboard.begin();
  USB.begin(); // Initialize USB stack

  Serial.println("Starting Rubber Ducky sequence in 1 seconds...");
  delay(500); // Give yourself time to plug it into the target Mac

  // 1. Open Spotlight (Command + Space)
  Serial.println("Opening Spotlight...");
  Keyboard.press(KEY_LEFT_GUI); // 'KEY_LEFT_GUI' or 'KEY_COMMAND' depending on library
  Keyboard.press(' ');
  delay(100);
  Keyboard.releaseAll();
  delay(500); // Wait for Spotlight to appear

  // 2. Type "Terminal"
  Serial.println("Typing Terminal...");
  typeString("Terminal");
  delay(500);

  // 3. Press Enter
  Serial.println("Pressing Enter...");
  typeKey(KEY_RETURN); // Or KEY_ENTER
  delay(1500); // Wait for Terminal to open and become active

  // 4. Type "ifconfig"
  Serial.println("Typing ifconfig...");
  typeString("ifconfig");
  delay(200);

  // 5. Press Enter
  Serial.println("Pressing Enter to run ifconfig...");
  typeKey(KEY_RETURN); // Or KEY_ENTER

  Serial.println("Script finished.");
  Keyboard.end(); // Release HID
}

void loop() {
  // Nothing to do here for a one-shot script
  delay(10000); // Idle
}