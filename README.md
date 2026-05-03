# Gas Pump IoT Readings

Flask app to read gas tank status from Arduino via serial.

## Setup

1. **Install:** `pip install -r requirements.txt`
2. **Configure:** Set `PORT` in `app.py` (e.g., `COM3` or `/dev/ttyUSB0`).
3. **Run:** `python app.py`

## API

- **GET `/tank-status`**
  - **Success:** `{"status": "success", "data": {"level": 75, "unit": "percent"}}`
  - **Error:** 408/500 if serial fails.
