#include <Arduino.h>

class FuelTank {
  private:
    int _redPin, _greenPin, _bluePin;
    float _currentGallons;
    float _totalCapacity;
    float _temperature;
    unsigned long _lastUpdate;
    
    int _cycleCount;
    const int _maxCycles = 100;
    bool _isStopped = false; // Flag to permanently stop the simulation

  public:
    FuelTank(int r, int g, int b, float cap, float initialFill, float temp) {
      _redPin = r;
      _greenPin = g;
      _bluePin = b;
      _totalCapacity = cap;
      _currentGallons = initialFill;
      _temperature = temp;
      _lastUpdate = 0;
      _cycleCount = 0;
    }

    void begin() {
      pinMode(_redPin, OUTPUT);
      pinMode(_greenPin, OUTPUT);
      pinMode(_bluePin, OUTPUT);
    }

    void sendManualJson(const char* status) {
      float percentage = (_currentGallons / _totalCapacity) * 100.0;

      Serial.print("{");
      Serial.print("\"cycle\":");       Serial.print(_cycleCount);
      Serial.print(",\"tank\":\"Diesel\"");
      Serial.print(",\"gallons\":");    Serial.print(_currentGallons);
      Serial.print(",\"percent\":");    Serial.print(percentage);
      Serial.print(",\"temp\":");       Serial.print(_temperature);
      Serial.print(",\"status\":\"");   Serial.print(status);
      Serial.print("\"}");
      Serial.println(); 
    }

    void update(unsigned long currentTime) {
      // Stop if cycles are maxed OR if the stopped flag is true
      if (_cycleCount >= _maxCycles || _isStopped) {
        return; 
      }

      // Check for empty fuel
      if (_currentGallons <= 0) {
        _currentGallons = 0; // Clamp to zero
        sendManualJson("TANK_EMPTY");
        _isStopped = true;   // Set the flag to stop further updates
        analogWrite(_greenPin, 0);
        analogWrite(_redPin, 255); // Solid Red indicating empty/stopped
        return;
      }

      // 3-second simulation "Tick"
      if (currentTime - _lastUpdate >= 3000) {
        if (_currentGallons >= 50) {
          _currentGallons -= 50;
        } else {
          _currentGallons = 0;
        }
        
        _cycleCount++;
        _lastUpdate = currentTime;

        float percentage = (_currentGallons / _totalCapacity) * 100.0;
        if (percentage <= 20.0 && percentage > 0) {
           sendManualJson("WARNING_LOW");
        } else if (percentage > 20.0) {
           sendManualJson("OK");
        }
      }

      updateLights();
    }

    void updateLights() {
      if (_isStopped) return; // Don't change lights if simulation ended

      float percentage = (_currentGallons / _totalCapacity) * 100.0;

      if (percentage > 20.0) {
        analogWrite(_greenPin, 255);
        analogWrite(_redPin, 0);
      } 
      else {
        // Blinking Red for low fuel
        if ((millis() / 500) % 2 == 0) {
          analogWrite(_redPin, 255);
        } else {
          analogWrite(_redPin, 0);
        }
        analogWrite(_greenPin, 0);
      }
    }
};

FuelTank Diesel(6, 5, 3, 10000.0, 2400.0, 72.5);

void setup() {
  Serial.begin(9600);
  Diesel.begin();
}

void loop() {
  Diesel.update(millis());

  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'R') {
      Diesel.sendManualJson("REQUEST_SUCCESS"); 
    }
  }
}