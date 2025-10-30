//=========================================//
//  NAME    : TAYYAB AWAN
//  TITLE   : TASK 2 
//  ROLL NO : 1288
//=========================================//

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED setup
#define OLED_W 128
#define OLED_H 64
Adafruit_SSD1306 screen(OLED_W, OLED_H, &Wire, -1);

// Pin mapping
const int LED_OUT = 18;
const int BUZZ_OUT = 27;
const int BTN_INPUT = 14;

// Timing variables
unsigned long holdStart = 0;
bool btnActive = false;
bool ledOn = false;

//==============================//
//        OLED FUNCTIONS
//==============================//
void showMessage(const String &line1, const String &line2 = "") {
  screen.clearDisplay();
  screen.setTextSize(1);
  screen.setTextColor(SSD1306_WHITE);
  screen.setCursor(10, 20);
  screen.println(line1);
  if (line2 != "") {
    screen.setCursor(10, 35);
    screen.println(line2);
  }
  screen.display();
}

//==============================//
//            SETUP
//==============================//
void setup() {
  pinMode(LED_OUT, OUTPUT);
  pinMode(BUZZ_OUT, OUTPUT);
  pinMode(BTN_INPUT, INPUT_PULLUP);

  screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showMessage("Device Booting...");
  delay(800);
  showMessage("Ready for Input");
}

//==============================//
//             LOOP
//==============================//
void loop() {
  bool btnState = digitalRead(BTN_INPUT);

  // detect press
  if (btnState == LOW && !btnActive) {
    btnActive = true;
    holdStart = millis();
  }

  // detect release
  if (btnState == HIGH && btnActive) {
    btnActive = false;
    unsigned long holdTime = millis() - holdStart;

    if (holdTime < 1500) {
      // short press - toggle LED
      ledOn = !ledOn;
      digitalWrite(LED_OUT, ledOn);
      showMessage("Short Press", ledOn ? "LED: ON" : "LED: OFF");
    } else {
      // long press - buzzer tone
      showMessage("Long Press", "Buzzer: ACTIVE");
      tone(BUZZ_OUT, 1200);
      delay(900);
      noTone(BUZZ_OUT);
      showMessage("Long Press", "Buzzer: OFF");
    }
  }
}
