#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to estimate state of health (SOH) based on cell capacity
float estimate_SOH(float AH, float AH_new, float AH_eol) {


    // Calculate the SOH using the capacity based formula
    float SOH = ((AH - AH_eol) / (AH_new - AH_eol)) * 100;
  // If SOH becomes negative, set SOH to 0.00
SOH = (SOH < 0.0) ? 0.0 : SOH;

    return SOH;
}

// Function to simulate Coulomb Counting and update cell capacity
float update_cell_capacity(float prev_capacity, float current, float time_interval) {
    // Calculate the charge passed during the time interval (current * time_interval)
    float charge_passed = current * time_interval;
    // Update the cell capacity by subtracting the charge passed
    float new_capacity = prev_capacity - charge_passed;
    return new_capacity;
}

int main() {
    // Seed the random number generator
    srand(time(NULL));
    
    // Example values for initial cell capacity, new capacity, and end-of-life capacity

    float AH_new = 35.0;    // New capacity
    float AH_eol = 25.0;    // End-of-life capacity 

 // Initialize the initial cell capacity to the new capacity
    float AH = AH_new;

    // Initialize the previous capacity
    float prev_capacity = AH;

    // Initialize the initial State of Health (SOH) to 100%
    float SOH = 100.0;
    
 

    // User input for simulation time in minutes
    int simulation_time_minutes;
    printf("Enter simulation time in minutes: ");
    scanf("%d", &simulation_time_minutes);
    
    // Convert simulation time from minutes to seconds
    int simulation_time_seconds = simulation_time_minutes * 60;
    
    // Example value for time increment (in seconds)
    float time_increment = 30.0;  // Time increment (in seconds)
    
    
    // Loop for simulation
    int elapsed_time_seconds = 0;
    while (elapsed_time_seconds < simulation_time_seconds && prev_capacity > AH_eol) {
       // Generate a random current within the range of 0.3 A to 0.5 A
float current = ((rand() % 101) + 100) / 1000.0;  // Random current between 0.1 A and 0.3 A
        
        // Update cell capacity using Coulomb Counting
        AH = update_cell_capacity(prev_capacity, current, time_increment / 3600.0); // Convert time increment from seconds to hours
        
        // Update the previous capacity for the next iteration
        prev_capacity = AH;
        
        // Increment elapsed time
        elapsed_time_seconds += time_increment;
        
        // Estimate the state of health (SOH) based on the updated cell capacity
        SOH = estimate_SOH(AH, AH_new, AH_eol);

        // If SOH becomes negative, set SOH and current to 0
        if (SOH < 0) {
           // SOH = 0;
            current = 0;
        }
        
        // Print the updated cell capacity, the generated current, and estimated SOH
        printf("Elapsed Time: %d seconds\n", elapsed_time_seconds);
        printf("Updated Cell Capacity: %.2f Ah\n", AH);
        printf("Generated Current: %.2f A\n", current);
        printf("Estimated State of Health (SOH): %.2f%%\n", SOH);
        printf("\n");
    }
    
    return 0;
}

