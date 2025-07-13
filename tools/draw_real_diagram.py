import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# --- Load the CSV data ---
df = pd.read_csv("motor_data_filter.csv")

# Compute time in seconds
df["Time (s)"] = df["Sample"] * 0.015

# Drop rows with NaN in filtered RPM
df = df.dropna(subset=["Filtered Motor Speed (RPM)"]).reset_index(drop=True)

# Extract relevant columns
time = df["Time (s)"].values
rpm = df["Filtered Motor Speed (RPM)"].values

# --- Tốc độ ổn định (Steady-State Speed) ---
steady_state_rpm = round(rpm[-1], 2)
print(f"✅ Steady-State Speed (Tốc độ ổn định): {steady_state_rpm} RPM")

# --- Time Delay (L): time until RPM first exceeds 5% of steady-state value ---
threshold_L = 0.05 * steady_state_rpm
idx_delay = np.argmax(rpm > threshold_L)
time_delay = round(time[idx_delay], 3)
print(f"⏱️ Time Delay (L - Thời gian trễ): {time_delay} s")

# --- Rise Time (Tr): time from 10% to 90% of steady-state RPM ---
rpm_10 = 0.1 * steady_state_rpm
rpm_90 = 0.9 * steady_state_rpm

# Find time when RPM first crosses 10% and 90%
idx_10 = np.argmax(rpm > rpm_10)
idx_90 = np.argmax(rpm > rpm_90)
rise_time = round(time[idx_90] - time[idx_10], 3)
print(f"📈 Rise Time (Tr - Thời gian tăng): {rise_time} s")

# --- Settling Time (Ts): time when RPM enters and stays within ±5% band ---
lower_bound = steady_state_rpm * 0.95
upper_bound = steady_state_rpm * 1.05

# Find the last index where RPM leaves the ±5% band
idx_settle = 0
for i in range(len(rpm)):
    window = rpm[i:]
    if np.all((window >= lower_bound) & (window <= upper_bound)):
        idx_settle = i
        break
settling_time = round(time[idx_settle], 3)
print(f"🧘 Settling Time (Ts - Thời gian ổn định): {settling_time} s")

# --- Plot ---
plt.figure(figsize=(10, 5))
plt.plot(time, rpm, label="Filtered RPM", color="orange")

# Mark key points
plt.axhline(steady_state_rpm, color='gray', linestyle='--', label=f'Steady-State = {steady_state_rpm} RPM')
plt.axvline(time_delay, color='blue', linestyle='--', label=f'L = {time_delay}s')
plt.axvline(time[idx_10], color='green', linestyle='--', label=f'10% = {round(rpm_10,2)} RPM')
plt.axvline(time[idx_90], color='red', linestyle='--', label=f'90% = {round(rpm_90,2)} RPM')
plt.axvline(settling_time, color='purple', linestyle='--', label=f'Ts = {settling_time}s')

# Labels and layout
plt.title("Filtered Motor Speed (RPM) Over Time")
plt.xlabel("Time (s)")
plt.ylabel("Motor Speed (RPM)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("rpm_response_analysis.png")
plt.show()
