#include <stdio.h>

// Function to calculate SOC
float calculateSOC(float SOC_prev, float I, float Q, float dt) {
    // Calculate change in SOC
    float SOC_change = (I / Q) * dt;

    // Calculate new SOC
    float SOC = SOC_prev + SOC_change;

    // Ensure SOC remains within [0, 100] range
    if (SOC < 0) {
        SOC = 0;
    } else if (SOC > 100) {
        SOC = 100;
    }

    return SOC;
}

int main() {
    // Parameters
    float SOC_prev = 50.0;  // Previous SOC (%)
    float I = -5.0;         // Charging or discharging current (A), negative for discharge
    float Q = 100.0;        // Battery cell capacity (Ah)
    float dt = 1.0;         // Time step (hours)

    // Calculate SOC
    float SOC = calculateSOC(SOC_prev, I, Q, dt);

    // Output SOC
    printf("Current SOC: %.2f%%\n", SOC);

    return 0;
}
