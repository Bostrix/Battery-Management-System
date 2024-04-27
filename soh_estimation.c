#include <stdio.h>

// Function to calculate State of Health (SoH)
float calculateSoH(float presentCapacity, float initialCapacity) {
    return (presentCapacity / initialCapacity) * 100.0;
}

int main() {
    float presentCapacity, initialCapacity;

    // Input present capacity from the user
    printf("Enter the present capacity of the battery (in Ah): ");
    scanf("%f", &presentCapacity);

    // Input initial capacity from the user
    printf("Enter the initial capacity of the battery (in Ah): ");
    scanf("%f", &initialCapacity);

    // Calculate SoH
    float soh = calculateSoH(presentCapacity, initialCapacity);

    // Display the SoH
    printf("State of Health (SoH): %.2f%%\n", soh);

    return 0;
}
