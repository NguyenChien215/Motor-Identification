import serial
import csv
import re

# Fixed output filename
csv_filename = "motor_data.csv"

# Set your UART port and baud rate
UART_PORT = 'COM5'     # Use 'COM3' on Windows or '/dev/ttyUSB0' on Linux
BAUD_RATE = 115200             # Set to your device's baud rate

# Regex pattern to extract Sample, PWM, and Motor Speed
pattern = re.compile(r"Sample (\d+), PWM: (\d+) , Motor Speed: ([\d.]+) RPM")

try:
    ser = serial.Serial(UART_PORT, BAUD_RATE, timeout=1)
    print(f"Logging UART data to '{csv_filename}' (overwriting each run)...")

    with open(csv_filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Sample', 'PWM', 'Motor Speed (RPM)'])  # CSV header

        while True:
            try:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                match = pattern.search(line)
                if match:
                    sample = int(match.group(1))
                    pwm = int(match.group(2))
                    speed = float(match.group(3))
                    writer.writerow([sample, pwm, speed])
                    print(f"Sample {sample}, PWM {pwm}, Speed {speed} RPM")
            except KeyboardInterrupt:
                print("\nStopped logging.")
                break

except serial.SerialException as e:
    print(f"Error opening UART port: {e}")
