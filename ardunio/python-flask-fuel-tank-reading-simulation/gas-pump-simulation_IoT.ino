#include <Arduino.h>
#include "PumpAndTankInstances.h"

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // Seed random number generator
  
  for (int i = 0; i < 4; i++) {
    regularTanks[i].begin();
    dieselTanks[i].begin();
  }
  for (int i = 0; i < 2; i++) {
    premiumTanks[i].begin();
  }
}

void loop() {
  unsigned long now = millis();
  
  for (int i = 0; i < 4; i++) {
    regularTanks[i].update(now);
    regularTanks[i].updateLights();
    dieselTanks[i].update(now);
    dieselTanks[i].updateLights();
  }
  for (int i = 0; i < 2; i++) {
    premiumTanks[i].update(now);
    premiumTanks[i].updateLights();
  }

  for (int i = 0; i < 10; i++) {
    pumps[i].update(now);
  }

  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'R') {
      for (int i = 0; i < 4; i++) {
        regularTanks[i].sendManualJson("REQUEST_SUCCESS");
        dieselTanks[i].sendManualJson("REQUEST_SUCCESS");
      }
      for (int i = 0; i < 2; i++) {
        premiumTanks[i].sendManualJson("REQUEST_SUCCESS");
      }
      for (int i = 0; i < 10; i++) {
        pumps[i].sendStatus();
      }
    }
  }
}