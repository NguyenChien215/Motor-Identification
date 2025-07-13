import pandas as pd

# Load the CSV file
df = pd.read_csv("motor_data.csv")

# Find the index where Sample == 1 first appears
start_index = df[df["Sample"] == 1].index[0]

# Slice the data from Sample 1 onwards
df_filtered = df.loc[start_index:].reset_index(drop=True)

# Keep only samples where Sample <= 502
df_filtered = df_filtered[df_filtered["Sample"] <= 502].reset_index(drop=True)

# Apply centered 3-sample moving average and round to 2 decimal places
df_filtered["Filtered Motor Speed (RPM)"] = (
    df_filtered["Motor Speed (RPM)"].rolling(window=3, center=True).mean().round(2)
)

# Save to new CSV
df_filtered.to_csv("motor_data_filter.csv", index=False)

print("Filtered data from Sample 1 to 502 saved to 'motor_data_filter.csv'")
