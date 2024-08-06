#include <Arduino.h>
#include <Keyboard.h>
#include "buttonDebounce.h"
#include "serialHelpers.h"
#include "blinker.h"

const int ledPin = A1;
const int ledGndPin = A0;
const int coinPin = 5;
const int buttonPin = 6;

ButtonDebounce coinState;
ButtonDebounce buttonState;

Blinker blinker;

uint8_t coinKey = 'c';
uint8_t buttonKey = KEY_RETURN; 

int ledState = LOW;
unsigned long coinsCount = 0;

void blinkN(int n);
void sendKey(uint8_t key);
void updateKey(ButtonDebounce state, uint8_t key); 

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(ledGndPin, OUTPUT);
  pinMode(coinPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  blinkN(3);
  blinker.setBlink(0, 1, 0);
  blinker.update();
  digitalWrite(ledPin, !blinker.state);
}

void loop() {
  coinState.updateState(digitalRead(coinPin));
  buttonState.updateState(digitalRead(buttonPin));

  if (coinState.isBtnPressed) {
    coinsCount++;
    //blinkN(2);
    //blinkN(coinsCount);
    blinker.setBlink(300, 300, 2);
    sendKey(coinKey);
    if (isSerialPortOpened()) {
      Serial.println(coinsCount);
    }
    delay(100);  
  }

  updateKey(buttonState, buttonKey);
  if (buttonState.isBtnPressed) {
    blinker.setBlink(60, 60, 0);
  } else if (buttonState.isBtnReleased) {
    blinker.setBlink(0, 1, 0);
  }

  blinker.update();
  digitalWrite(ledPin, !blinker.state);
}

void blinkN(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(ledPin, LOW);
    delay(300);
    digitalWrite(ledPin, HIGH);
    delay(300);
  }
}

void sendKey(uint8_t key) {
  Keyboard.press(key);
  delay(100);
  Keyboard.release(key);
}

void updateKey(ButtonDebounce state, uint8_t key) {
  if (state.isBtnPressed) {
    Keyboard.press(key);
  } else if (state.isBtnReleased) {
    Keyboard.release(key);
  }
}