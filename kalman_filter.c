#include <stdio.h>

// Define Kalman filter parameters
#define DT 1.0       // Time step
#define R 0.1        // Measurement noise covariance
#define Q 0.01       // Process noise covariance

// Battery parameters
#define V_OC 4.2     // Open circuit voltage
#define R_INTERNAL 0.2 // Internal resistance

// Kalman filter variables
double x_est = 0.5;  // Initial SoC estimate
double P_est = 1.0;  // Initial covariance estimate

// Function to update Kalman filter prediction step
void predict(double u) {
    // Prediction update equations
    x_est = x_est + (DT / V_OC) * (u - x_est * R_INTERNAL);
    P_est = P_est + Q;
}

// Function to update Kalman filter correction step
void correct(double z) {
    // Correction update equations
    double K_gain = P_est / (P_est + R);
    x_est = x_est + K_gain * (z - V_OC + x_est * R_INTERNAL);
    P_est = (1 - K_gain) * P_est;

    // Clamp SoC 
    x_est = (x_est < 0.0) ? 0.0 : (x_est > 1.0) ? 1.0 : x_est;
}

int main() {
    // Simulated current input (charging and discharging)
    double currents[] = {1.0, -0.5, 0.8, -0.3, 0.6}; // Charging and discharging currents

    // Simulate Kalman filter operation
    for (int i = 0; i < sizeof(currents) / sizeof(currents[0]); ++i) {
        // Prediction step
        predict(currents[i]);

        // Simulate voltage measurement (for simplicity, assuming V_OC is constant)
        double voltage = V_OC - currents[i] * R_INTERNAL;

        // Correction step
        correct(voltage);

        // Output estimated SoC as percentage
        printf("Estimated SoC after step %d: %.2f%%\n", i + 1, x_est * 100);
    }

    return 0;
}
