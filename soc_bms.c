#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <unistd.h> // For sleep()

// Structure to hold battery parameters and state
struct Battery {
    float capacity_mAh; // Battery capacity in mAh
    float current_mA;   // Current in mA
    float voltage_V;    // Voltage in V
    float temperature_C; // Temperature in Celsius
    float time_ms;      // Time in milliseconds
    float soc;          // State of Charge (SoC)
};

// Function to initialize the battery with initial parameters
void initializeBattery(struct Battery *b, float capacity, float initialVoltage, float initialSoC) {
    b->capacity_mAh = capacity;
    b->voltage_V = initialVoltage;
    b->soc = initialSoC;
    b->current_mA = 0.0; // Default initial current is 0 mA
    b->temperature_C = 25.0; // Default initial temperature is 25 Celsius
    b->time_ms = 0.0; // Default initial time is 0 ms
}

// Function to update battery state
void updateBattery(struct Battery *b, float current, float voltage, float temperature, float time) {
    b->current_mA = current;
    b->voltage_V = voltage;
    b->temperature_C = temperature;
    b->time_ms = time;
}

void estimateSoC(struct Battery *b, float dt) {
    // Calculate change in charge
    float deltaQ = b->current_mA * dt;

    // Calculate new SoC
    float newSoC = (b->soc + (deltaQ / b->capacity_mAh) * 100.0);

    // Ensure SoC to 100%
    b->soc = (newSoC > 100.0) ? 100.0 : newSoC;

    // Ensure SoC doesn't go below 0%
 b->soc = (newSoC < 0.0) ? 0.0 : newSoC;

}



// Function to convert time input to milliseconds
int convertToMilliseconds(int time, char unit) {
    switch (unit) {
        case 'h': // hours
            return time * 3600000;
        case 'm': // minutes
            return time * 60000;
        case 's': // seconds
            return time * 1000;
        default: // milliseconds
            return time;
    }
}

int main() {
    // Constants
    float capacity = 500.0; // Battery capacity in mAh
    float dt = 2.0; // Time step in milliseconds

   // User input for simulation time
int simulationTime;
char unit;
printf("Enter simulation time and unit (s/m/h): ");
// Read simulation time and unit
if (scanf("%d %c", &simulationTime, &unit) != 2 || (unit != 's' && unit != 'm' && unit != 'h')) {
    printf("Invalid input. Please enter a valid simulation time and unit (s/m/h).\n");
    return 1; // Exit program with error status
}

// Convert simulation time to milliseconds
int milliseconds = convertToMilliseconds(simulationTime, unit);

// Calculate number of iterations based on simulation time and time step
// Calculate number of iterations based on simulation time and time step
int iterations = (milliseconds + dt - 1) / dt;


    // Create and initialize battery
    struct Battery b;
    initializeBattery(&b, capacity, 4.2, 50.0); // Initial voltage and SoC assumed

    // Seed for random number generator (only once)
    srand(time(NULL));

    // Simulate running for the specified time
        // Simulate running for the specified time
    int i;
    for (i = 0; i < iterations; i++) {
        // Determine whether to discharge or charge
        float current = 0.0; // Initialize current to 0.0
        if (i < iterations / 2) { // First half of simulation: discharging
            // Check if SoC is already at 0%, if yes, stop discharging
            if (b.soc <= 0.0) {
                current = 0.0; // No current flow
            } else {
                // Random current between -3 A and -1 A (discharging)
current = (((float)rand() / RAND_MAX) * -2.0) - 1.0;

            }

            // Check if battery is undercharged before switching off
            if (b.soc < 10 && b.soc > 0) {
                printf("Warning: Battery is low. Charge it before it switches off.\n");
            }
        } else { // Second half of simulation: charging
            if (b.soc < 100.00) { // Charge only if SoC is below 100%
// Adjust charging current range
current = (((float)rand() / RAND_MAX) * 0.4) + 0.1; // Charging current between 0.1 A and 0.5 A

                b.soc += current * (dt / 3600.0); // Update SoC based on charging current
             
            }
        }

        // Calculate time
        float time = (i + 1) * dt; // Time in milliseconds

        // Print current, time, and SoC
        printf("Iteration %d - Current: %.2f mA, Time: %.2f ms, SoC: %.2f%%\n", i + 1, current, time, b.soc);

        // Calculate voltage (example: linear decrease based on current)
        float voltage = b.voltage_V - current * 0.01; // Linear decrease of 0.01 V for each A of current

        // Update battery state
        updateBattery(&b, current, voltage, 25.0, time);

        // Estimate SoC
        estimateSoC(&b, dt);

        // Sleep for 1 second (1000 milliseconds)
        //  sleep(1);
    }

    // If battery is fully charged, continue printing remaining iterations with time updated
    if (b.soc >= 100.00) {
        for (; i < iterations; i++) {
            float time = (i + 1) * dt; // Update time
            printf("Iteration %d - Current: 0.00 mA, Time: %.2f ms, SoC: %.2f%%\n", i + 1, time, b.soc);
        }
    }


    // Print estimated SoC after the specified time
    printf("Estimated SoC after %d %c: %.2f%%\n", simulationTime, unit, b.soc);

    return 0;
}



