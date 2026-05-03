import sys
import serial
import time
import json
from flask import Flask, request, jsonify
app = Flask(__name__)

PORT = ''

try:
    # Use a long timeout to prevent the script from hanging indefinitely
    ser = serial.Serial(PORT, 9600, timeout=1)
    time.sleep(2)
except Exception as e:
    print(f"FATAL ERROR: Could not connect to Arduino on {PORT}. {e}")
    sys.exit(1)

@app.route('/tank-status', methods=['GET'])
def get_tank_status():
    global ser
    if not ser or not ser.is_open:
        return jsonify({"status": "error", "message": "Serial connection unavailable"}), 500

    try:
        ser.reset_input_buffer()
        ser.write(b'R')
        line = ser.readline().decode('utf-8').strip()

        if line:
            data = json.loads(line)
            return jsonify({"status": "success", "data": data}), 200
        else:
            return jsonify({"status": "error", "message": "Arduino did not respond"}), 408

    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == '__main__':
    # CRITICAL: debug=False prevents the port from being locked by
    # the Flask reloader process.
    app.run(debug=False, port=5000, use_reloader=False)