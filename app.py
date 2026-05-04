import sys
import os
import serial
import time
import json
from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})

PORT = '/dev/cu.usbmodem21201'

try:
    # Use a long timeout to prevent the script from hanging indefinitely
    ser = serial.Serial(PORT, 9600, timeout=1)
    time.sleep(2)
    print(f"SUCCESS: Arduino connected on {PORT}")
except Exception as e:
    print(f"FATAL ERROR: Could not connect to Arduino on {PORT}. {e}")
    sys.exit(1)

@app.before_request
def log_request_info():
    print(f"Request: {request.method} {request.url}")
    print(f"Headers: {dict(request.headers)}")

@app.after_request
def add_cors_headers(response):
    response.headers['Access-Control-Allow-Origin'] = '*'
    response.headers['Access-Control-Allow-Methods'] = 'GET, POST, OPTIONS, PUT, DELETE'
    response.headers['Access-Control-Allow-Headers'] = 'Content-Type, Authorization'
    return response

@app.route('/tank-status', methods=['GET', 'OPTIONS'])
def get_tank_status():
    if request.method == 'OPTIONS':
        return '', 204
    global ser
    if not ser or not ser.is_open:
        return jsonify({"status": "error", "message": "Serial connection unavailable"}), 500

    try:
        ser.reset_input_buffer()
        ser.write(b'R')
        
        results = []
        start_time = time.time()
        timeout = 2  # Wait up to 2 seconds for all 5 responses
        
        while len(results) < 3 and (time.time() - start_time) < timeout:
            line = ser.readline().decode('utf-8').strip()
            if line:
                try:
                    data = json.loads(line)
                    # Only accept tank status messages
                    if 'tank' in data and 'gallons' in data:
                        results.append(data)
                    else:
                        print(f"Ignored non-tank message: {line}")
                except json.JSONDecodeError:
                    print(f"Ignored invalid JSON: {line}")

        if results:
            print(f"Sending tanks to frontend: {json.dumps(results, indent=2)}")
            return jsonify({"status": "success", "data": results}), 200
        else:
            return jsonify({"status": "error", "message": "Arduino did not respond with tank data"}), 408

    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500

@app.route('/pump-status', methods=['GET', 'OPTIONS'])
def get_pump_status():
    if request.method == 'OPTIONS':
        return '', 204
    global ser
    if not ser or not ser.is_open:
        return jsonify({"status": "error", "message": "Serial connection unavailable"}), 500

    try:
        ser.reset_input_buffer()
        ser.write(b'R')
        
        results = []
        start_time = time.time()
        timeout = 2  # Wait up to 2 seconds for all 5 responses
        
        while len(results) < 2 and (time.time() - start_time) < timeout:
            line = ser.readline().decode('utf-8').strip()
            if line:
                try:
                    data = json.loads(line)
                    # Only accept pump status messages
                    if 'pump' in data and 'status' in data:
                        results.append(data)
                    else:
                        print(f"Ignored non-pump message: {line}")
                except json.JSONDecodeError:
                    print(f"Ignored invalid JSON: {line}")

        if results:
            print(f"Sending pumps to frontend: {json.dumps(results, indent=2)}")
            return jsonify({"status": "success", "data": results}), 200
        else:
            return jsonify({"status": "error", "message": "Arduino did not respond with pump data"}), 408

    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == '__main__':
    # CRITICAL: debug=False prevents the port from being locked by
    # the Flask reloader process.
    app.run(debug=False, port=5000, use_reloader=False)