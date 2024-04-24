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
    float SOC_prev, I, Q, dt;

    // Input from the user
    printf("Enter previous SOC: ");
    scanf("%f", &SOC_prev);

    printf("Enter charging or discharging current (A): ");
    scanf("%f", &I);

    printf("Enter battery cell capacity (Ah): ");
    scanf("%f", &Q);

    printf("Enter time step (hours): ");
    scanf("%f", &dt);

    // Calculate SOC
    float SOC = calculateSOC(SOC_prev, I, Q, dt);

    // Output SOC
    printf("Current SOC: %.2f%%\n", SOC);

    return 0;
}
