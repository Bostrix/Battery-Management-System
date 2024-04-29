#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

// Define Kalman filter parameters
const double dt = 1.0; // Time step
const double R = 0.1; // Measurement noise covariance
const double Q = 0.01; // Process noise covariance

// Matrices
const double A = 1; // State transition matrix
const double B = 1; // Control matrix
const double C = 1; // Measurement matrix

// Function to update Kalman filter prediction step
void predict(double& x_est, double& P_est, double u) {
    // Prediction update equations
    x_est = A * x_est + B * u;
    P_est = A * P_est * A + Q;
}

// Function to update Kalman filter correction step
void correct(double& x_est, double& P_est, double z) {
    // Correction update equations
    double K_gain = P_est * C / (C * P_est * C + R);
    x_est = x_est + K_gain * (z - C * x_est);
    P_est = (1 - K_gain * C) * P_est;
}

int main() {
    // Initial state and covariance estimation
    double x_est = 0; // Initial state estimate
    double P_estimation = 1; // Initial covariance estimate

    // Simulated input (voltage measurement)
    vec measurements = {0.5, 0.7, 0.9, 0.10, 0.12}; // Simulated voltage measurements

    // Simulate Kalman filter operation
    for (int i = 0; i < measurements.n_elem; ++i) {
        // Prediction step
        predict(x_est, P_estimation, 0); // No control input assumed

        // Correction step
        correct(x_est, P_estimation, measurements(i));

        // Output estimated SoC as percentage
        cout << "Estimated SoC after measurement " << i+1 << ": " << (x_est * 100) << "%" << endl;
    }

    return 0;
}
