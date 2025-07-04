import serial
import time

# Open the correct port and baud rate
ser = serial.Serial('/dev/ttyUSB0', 115200)
time.sleep(2)  # Let ESP32 finish resetting

# Your enrollment data (JSON string)
payload = '{"name":"John", "matric": "AFIT/001"}\n'

# Write the string (encode to bytes)
ser.write(payload.encode())

# Optional: flush to ensure it's sent
ser.flush()

# Done
ser.close()

