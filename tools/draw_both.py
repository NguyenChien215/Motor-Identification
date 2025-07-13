import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import control as ctrl
from sklearn.metrics import mean_squared_error, mean_absolute_error
from scipy.interpolate import interp1d

# =============================
# Load Real Data from CSV
# =============================
df = pd.read_csv("motor_data_filter.csv")
df["Time (s)"] = df["Sample"] * 0.015
df = df.dropna(subset=["Filtered Motor Speed (RPM)"]).reset_index(drop=True)

time_real = df["Time (s)"].values
rpm_real = df["Filtered Motor Speed (RPM)"].values

# =============================
# Simulated Step Response
# =============================
K = 0.2995         # System gain
L = 0.03           # Delay (s)
tau = 0.075        # Time constant (s)
u_step = 80        # PWM step input

# Transfer function with Pade delay
G_nodelay = ctrl.TransferFunction([K], [tau, 1])
delay_num, delay_den = ctrl.pade(L, 1)
delay_tf = ctrl.TransferFunction(delay_num, delay_den)
G = G_nodelay * delay_tf

# Simulate for 7.5 seconds
time_sim = np.linspace(0, 7.5, 2000)
t_out, y_out = ctrl.step_response(G * u_step, T=time_sim)

# =============================
# Plot Both Real & Simulated Data
# =============================
plt.figure(figsize=(10, 5))

# Plot real data
plt.plot(time_real, rpm_real, label="Real Data", color="orange", linewidth=2)

# Plot simulation
plt.plot(t_out, y_out, label="Simulation", color="blue", linestyle="--", linewidth=2)

# Formatting
plt.xlabel("Time (s)")
plt.ylabel("Motor Speed (RPM)")
plt.title("Real vs Simulated Step Response (80 PWM Input)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("combined_real_vs_simulation.png")
plt.show()

sim_interp = interp1d(t_out, y_out, kind='linear', fill_value="extrapolate")
y_sim_aligned = sim_interp(time_real)  # align to real time points

# --- Calculate RMSE and MAE ---
rmse = np.sqrt(mean_squared_error(rpm_real, y_sim_aligned))
mae = mean_absolute_error(rpm_real, y_sim_aligned)

print(f"📏 RMSE (Root Mean Squared Error): {rmse:.4f} RPM")
print(f"📏 MAE  (Mean Absolute Error):     {mae:.4f} RPM")
