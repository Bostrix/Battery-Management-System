#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int main() {
    // Constants
    float capacity = 500.0; // Battery capacity in mAh
    float dt = 2.0; // Time step in milliseconds

    // Create and initialize battery
    struct Battery b;
    initializeBattery(&b, capacity, 4.2, 50.0); // Initial voltage and SoC assumed

    // Seed for random number generator (only once)
    srand(time(NULL));

    // Simulate running for 15 minutes
    for (int i = 0; i < 4500; i++) { // 15 minutes * 60 seconds / 2 ms = 4500 iterations
        // Randomly generate current
        float current = (((float)rand() / RAND_MAX) * 6.0) - 3.0; // Random current between -3 A and 3 A

        // Calculate time
        float time = (i + 1) * dt; // Time in milliseconds

        // Calculate voltage (example: linear decrease based on current)
        float voltage = b.voltage_V - current * 0.01; // Linear decrease of 0.01 V for each A of current

        // Update battery state
        updateBattery(&b, current, voltage, 25.0, time);

        // Estimate SoC
        estimateSoC(&b, dt);
    }

    // Print estimated SoC after 15 minutes
    printf("Estimated SoC after 15 minutes: %.2f%%\n", b.soc);

    return 0;
}

