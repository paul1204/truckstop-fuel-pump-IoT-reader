# Gas Pump IoT Readings

Flask app to read gas tank status from an **Arduino Uno** via serial. This project uses the Arduino to replicate an IoT device that monitors a **Diesel tank**.

## Setup

1. **Hardware:** Connect an Arduino Uno with the provided `.ino` sketch loaded.
2. **Install:** `pip install -r requirements.txt`
3. **Configure:** Set `PORT` in `app.py` (e.g., `COM3` or `/dev/ttyUSB0`).
4. **Run:** `python app.py`

## API

- **GET `/tank-status`**
  - **Success:** `{"status": "success", "data": {"tank": "Diesel", "gallons": 2400.0, "percent": 24.0, ...}}`
  - **Error:** 408/500 if serial fails.
