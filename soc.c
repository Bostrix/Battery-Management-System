#include <stdio.h>

// Function to estimate SoC
float estimateSoC(float SoC_prev, float I, float Qn, float dt) {
    // Calculate change in SoC
    float deltaSoC = I / Qn * dt;
    // Calculate current SoC
    float SoC = SoC_prev + deltaSoC;
    // Ensure SoC remains within 0-100% range
    if (SoC < 0) SoC = 0;
    else if (SoC > 100) SoC = 100;
    return SoC;
}

int main() {
    // Define parameters
    float SoC_prev, I, Qn, dt;

    // Get parameters from user input
    printf("Enter Previous SoC (%%): ");
    scanf("%f", &SoC_prev);

    printf("Enter Discharging current (A): ");
    scanf("%f", &I);

    printf("Enter Battery capacity (Ah): ");
    scanf("%f", &Qn);

    printf("Enter Time step (hours): ");
    scanf("%f", &dt);

    // Estimate SoC
    float SoC = estimateSoC(SoC_prev, I, Qn, dt);

    // Print estimated SoC
    printf("Estimated SoC: %.2f%%\n", SoC);

    return 0;
}
