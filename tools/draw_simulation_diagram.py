import numpy as np
import matplotlib.pyplot as plt
import control as ctrl

# --- System parameters ---
K = 0.2995         # system gain
L = 0.03           # time delay (s)
tau = 0.075        # time constant (s)
u_step = 80        # step input (PWM)

# --- Transfer function without delay ---
G_nodelay = ctrl.TransferFunction([K], [tau, 1])

# --- Delay approximation using Padé (order 1) ---
delay_num, delay_den = ctrl.pade(L, 1)
delay_tf = ctrl.TransferFunction(delay_num, delay_den)

# --- Full transfer function ---
G = G_nodelay * delay_tf

# --- Simulate step response for 80 PWM input over 7.5 seconds ---
time = np.linspace(0, 7.5, 2000)
t_out, y_out = ctrl.step_response(G * u_step, T=time)

# --- Steady state value ---
steady_rpm = round(K * u_step, 2)  # ≈ 23.96

# --- Plot ---
plt.figure(figsize=(10, 5))
plt.plot(t_out, y_out, label='Motor Speed (RPM)', color='blue')
plt.axhline(steady_rpm, color='gray', linestyle='--', label=f'Steady-State ≈ {steady_rpm} RPM')

plt.xlabel("Time (s)")
plt.ylabel("Motor Speed (RPM)")
plt.title("Step Response with 80 PWM Input (7.5s Duration)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("step_response_80pwm_7_5s.png")
plt.show()
