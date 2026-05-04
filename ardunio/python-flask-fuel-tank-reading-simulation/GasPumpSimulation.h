#ifndef GAS_PUMP_SIMULATION_H
#define GAS_PUMP_SIMULATION_H

#include <Arduino.h>

class FuelTank { 
  private:
    const char* _name;
    int _redPin, _greenPin, _bluePin;
    float _currentGallons;
    float _totalCapacity;
    float _temperature;
    unsigned long _lastUpdate;
    unsigned long _updateInterval;

    int _cycleCount;
    const int _maxCycles = 200; // Increased cycles
    bool _isStopped = false;

  public:
    FuelTank(const char* name, int r, int g, int b, float cap, float initialFill, float temp, unsigned long interval) {
      _name = name;
      _redPin = r;
      _greenPin = g;
      _bluePin = b;
      _totalCapacity = cap;
      _currentGallons = initialFill;
      _temperature = temp;
      _updateInterval = interval;
      _lastUpdate = millis(); // Initialize to current time
      _cycleCount = 0;
    }

    void begin() {
      pinMode(_redPin, OUTPUT);
      pinMode(_greenPin, OUTPUT);
      pinMode(_bluePin, OUTPUT);
    }

    const char* getName() { return _name; }

    void sendManualJson(const char* status) {
      float percentage = (_currentGallons / _totalCapacity) * 100.0;
      // Simulate slight temperature fluctuations
      _temperature += (float)(random(3) - 1) * 0.1; 

      Serial.print("{");
      Serial.print("\"cycle\":");       Serial.print(_cycleCount);
      Serial.print(",\"tank\":\"");     Serial.print(_name); Serial.print("\"");
      Serial.print(",\"gallons\":");    Serial.print(_currentGallons);
      Serial.print(",\"percent\":");    Serial.print(percentage);
      Serial.print(",\"temp\":");       Serial.print(_temperature);
      Serial.print(",\"status\":\"");   Serial.print(status);
      Serial.print("\"}");
      Serial.println();
    }

    void sendStatus() {
      if (_currentGallons <= 0) return; // TANK_EMPTY is already handled in takeFuel

      float percentage = (_currentGallons / _totalCapacity) * 100.0;
      if (percentage <= 20.0) {
        sendManualJson("WARNING_LOW");
      } else {
        sendManualJson("OK");
      }
    }

    float takeFuel(float amount) {
      if (_isStopped || _currentGallons <= 0) return 0;

      float reduction = amount;
      if (_currentGallons < reduction) {
        reduction = _currentGallons;
      }

      _currentGallons -= reduction;
      _cycleCount++;

      // Report updated tank inventory immediately after fuel removal
      sendStatus();

      if (_currentGallons <= 0) {
        _currentGallons = 0;
        sendManualJson("TANK_EMPTY");
        _isStopped = true;
        analogWrite(_greenPin, 0);
        analogWrite(_redPin, 255);
      }

      return reduction;
    }

    void update(unsigned long currentTime) {
      // Periodic inventory reporting moved to be triggered by FuelPump action
      // to avoid the appearance of the tank acting on its own.
    }

    void updateLights() {
      if (_isStopped) return;

      float percentage = (_currentGallons / _totalCapacity) * 100.0;

      if (percentage > 20.0) {
        analogWrite(_greenPin, 255);
        analogWrite(_redPin, 0);
      }
      else {
        if ((millis() / 500) % 2 == 0) {
          analogWrite(_redPin, 255);
        } else {
          analogWrite(_redPin, 0);
        }
        analogWrite(_greenPin, 0);
      }
    }
};

class FuelPump {
  private:
    const char* _name;
    FuelTank* _tank1;
    FuelTank* _tank2;
    unsigned long _lastUpdate;
    unsigned long _updateInterval;

  public:
    FuelPump(const char* name, FuelTank* t1, FuelTank* t2 = nullptr, unsigned long interval = 3000) {
      _name = name;
      _tank1 = t1;
      _tank2 = t2;
      _updateInterval = interval;
      _lastUpdate = millis();
    }

    void sendStatus() {
      Serial.print("{");
      Serial.print("\"pump\":\"");      Serial.print(_name);      Serial.print("\"");
      Serial.print(",\"status\":\"ONLINE\"");
      Serial.print("}");
      Serial.println();
    }

    void update(unsigned long currentTime) {
      if (currentTime - _lastUpdate >= _updateInterval) {
        _lastUpdate = currentTime;

        // Randomize fuel reduction
        float amount = 30.0 + (float)random(400) / 10.0;

        // If we have two tanks (like Regular/Premium), pick one to pump from
        FuelTank* activeTank = _tank1;
        if (_tank2 != nullptr && random(2) == 0) {
          activeTank = _tank2;
        }

        float taken = activeTank->takeFuel(amount);

        // After taking fuel, report the pump action
        Serial.print("{");
        Serial.print("\"pump\":\"");      Serial.print(_name);      Serial.print("\"");
        Serial.print(",\"tank\":\"");     Serial.print(activeTank->getName()); Serial.print("\"");
        Serial.print(",\"taken\":");      Serial.print(taken);
        Serial.print(",\"status\":\"PUMPING\"");
        Serial.print("}");
        Serial.println();
      }
    }
};

#endif
