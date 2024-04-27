#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

// Define Kalman filter parameters
const double dt = 1.0; // Time step
const double R = 0.1; // Measurement noise covariance
const double Q = 0.01; // Process noise covariance
const double A = 1; // State transition matrix
const double B = 1; // Control matrix
const double C = 1; // Measurement matrix

// Function to update Kalman filter prediction step
void predict(double& x, double& P, double u) {
    // Prediction update equations
    x = A * x + B * u;
    P = A * P * A + Q;
}

// Function to update Kalman filter correction step
void correct(double& x, double& P, double z) {
    // Correction update equations
    double K = P * C / (C * P * C + R);
    x = x + K * (z - C * x);
    P = (1 - K * C) * P;
}

int main() {
    // Initial state and covariance estimation
    double x_hat = 0; // Initial state estimate
    double P = 1; // Initial covariance estimate

    // Simulated input (voltage measurement)
    vec measurements = {0.5, 0.6, 0.7, 0.8, 0.9}; // Simulated voltage measurements

    // Simulate Kalman filter operation
    for (int i = 0; i < measurements.n_elem; ++i) {
        // Prediction step
        predict(x_hat, P, 0); // No control input assumed

        // Correction step
        correct(x_hat, P, measurements(i));

        // Output estimated SoC as percentage
        cout << "Estimated SoC after measurement " << i+1 << ": " << (x_hat * 100) << "%" << endl;
    }

    return 0;
}
