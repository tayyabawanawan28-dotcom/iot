
//  NAME    : TAYYAB AWAN
//  TITLE   : TASK 1 
//  ROLL NO : 1288

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED setup
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// Pin assignments
const int LED_A = 18;
const int LED_B = 19;
const int BTN_MODE = 14;
const int BTN_RESET = 27;

// States and timing
int currentMode = 0;
bool prevModeButton = HIGH;
bool prevResetButton = HIGH;
unsigned long lastButtonTime = 0;
const unsigned long debounce = 180;

//==============================//
//        OLED Display
//==============================//
void updateDisplay(int mode) {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 10);
  oled.print("Current Mode: ");
  oled.println(mode);
  oled.setCursor(0, 30);

  switch (mode) {
    case 0: oled.print("LEDs OFF"); break;
    case 1: oled.print("Alternating Blink"); break;
    case 2: oled.print("Both ON"); break;
    case 3: oled.print("PWM Fade"); break;
  }
  oled.display();
}

//==============================//
//         LED Behavior
//==============================//
void handleLEDs(int mode) {
  switch (mode) {
    case 0: // OFF
      digitalWrite(LED_A, LOW);
      digitalWrite(LED_B, LOW);
      break;

    case 1: // Alternate Blink
      digitalWrite(LED_A, HIGH);
      digitalWrite(LED_B, LOW);
      delay(250);
      digitalWrite(LED_A, LOW);
      digitalWrite(LED_B, HIGH);
      delay(250);
      break;

    case 2: // Both ON
      digitalWrite(LED_A, HIGH);
      digitalWrite(LED_B, HIGH);
      break;

    case 3: // PWM Fade
      for (int val = 0; val <= 255; val++) {
        analogWrite(LED_A, val);
        delay(4);
      }
      for (int val = 255; val >= 0; val--) {
        analogWrite(LED_A, val);
        delay(4);
      }
      break;
  }
}

//==============================//
//          Setup
//==============================//
void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  updateDisplay(currentMode);
}

//==============================//
//           Loop
//==============================//
void loop() {
  bool modePressed = digitalRead(BTN_MODE);
  bool resetPressed = digitalRead(BTN_RESET);
  unsigned long now = millis();

  // Mode change
  if (modePressed == LOW && prevModeButton == HIGH && now - lastButtonTime > debounce) {
    lastButtonTime = now;
    currentMode = (currentMode + 1) % 4;
    updateDisplay(currentMode);
  }

  // Reset mode
  if (resetPressed == LOW && prevResetButton == HIGH && now - lastButtonTime > debounce) {
    lastButtonTime = now;
    currentMode = 0;
    updateDisplay(currentMode);
  }

  prevModeButton = modePressed;
  prevResetButton = resetPressed;

  handleLEDs(currentMode);
}
