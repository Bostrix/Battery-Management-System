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

// Function to estimate State of Charge (SoC) using Coulomb counting
void estimateSoC(struct Battery *b, float dt) {
    // Calculate change in charge
    float deltaQ = b->current_mA * dt;

    // Update SoC
    b->soc += (deltaQ / b->capacity_mAh) * 100.0;

    // Ensure SoC is within 0 to 100%
    if (b->soc < 0)
        b->soc = 0;
    else if (b->soc > 100)
        b->soc = 100;
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
    scanf("%d %c", &simulationTime, &unit);

    // Convert simulation time to milliseconds
    int milliseconds = convertToMilliseconds(simulationTime, unit);

    // Calculate number of iterations based on simulation time and time step
    int iterations = milliseconds / dt;

    // Create and initialize battery
    struct Battery b;
    initializeBattery(&b, capacity, 4.2, 50.0); // Initial voltage and SoC assumed

    // Seed for random number generator (only once)
    srand(time(NULL));

    // Simulate running for the specified time
    for (int i = 0; i < iterations; i++) {
        // Determine whether to discharge or charge
        float current;
        if (i < iterations/2) { // First half of simulation: discharging
            current = (((float)rand() / RAND_MAX) * -3.0) - 1.0; // Random current between -1 A and -4 A (discharging)
        } else if (i >= iterations - 100) { // Last 100 iterations: charging
        current = (((float)rand() / RAND_MAX) * 3.0) + 1.0; // Random current between 1 A and 4 A (charging)
    } else { // Second half of simulation (excluding last 10 iterations): no current flow
        current = 0.0; // No current flow
    }
        // Calculate time
        float time = (i + 1) * dt; // Time in milliseconds

        // Print current, time, and SoC
        printf("Iteration %d - Current: %.2f mA, Time: %.2f ms, SoC: %.2f%%\n", i+1, current, time, b.soc);

        // Calculate voltage (example: linear decrease based on current)
        float voltage = b.voltage_V - current * 0.01; // Linear decrease of 0.01 V for each A of current

        // Update battery state
        updateBattery(&b, current, voltage, 25.0, time);

        // Estimate SoC
        estimateSoC(&b, dt);

        // Sleep for 1 second (1000 milliseconds)
      //  sleep(1);
    }

    // Print estimated SoC after the specified time
    printf("Estimated SoC after %d %c: %.2f%%\n", simulationTime, unit, b.soc);

    return 0;
}

