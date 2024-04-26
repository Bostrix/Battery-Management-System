#include <stdio.h>

// Function to calculate State of Health (SoH)
float calculateSoH(float actual_capacity, float initial_capacity) {
    return (actual_capacity / initial_capacity) * 100.0;
}

int main() {
    float actual_capacity, initial_capacity;
    
    // Accepting user input for actual and initial capacities
    printf("Enter actual capacity of the battery (in Ah): ");
    scanf("%f", &actual_capacity);
    
    printf("Enter initial capacity of the battery (in Ah): ");
    scanf("%f", &initial_capacity);
    
    // Calculating State of Health (SoH)
    float soh = calculateSoH(actual_capacity, initial_capacity);
    
    // Displaying the calculated SoH
    printf("State of Health (SoH): %.2f%%\n", soh);
    
    return 0;
}
