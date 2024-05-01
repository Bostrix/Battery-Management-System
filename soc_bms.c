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
void initializeBattery(struct Battery *battery, float capacity, float initialVoltage, float initialSoC) {
    battery->capacity_mAh = capacity;
    battery->voltage_V = initialVoltage;
    battery->soc = initialSoC;
    battery->current_mA = 0.0; // Default initial current is 0 mA
    battery->temperature_C = 25.0; // Default initial temperature is 25 Celsius
    battery->time_ms = 0.0; // Default initial time is 0 ms
}

// Function to update battery state
void updateBattery(struct Battery *battery, float current, float voltage, float temperature, float time) {
    battery->current_mA = current;
    battery->voltage_V = voltage;
    battery->temperature_C = temperature;
    battery->time_ms = time;
}

// Function to estimate State of Charge (SoC) using Coulomb counting
void estimateSoC(struct Battery *battery, float dt) {
    // Calculate change in charge
    float deltaQ = battery->current_mA * dt;

    // Update SoC
    battery->soc += (deltaQ / battery->capacity_mAh) * 100.0;

    // Ensure SoC is within 0 to 100%
    if (battery->soc < 0)
        battery->soc = 0;
    else if (battery->soc > 100)
        battery->soc = 100;
}

int main() {
    // Constants
    float capacity = 500.0; // Battery capacity in mAh
    float dt = 2.0; // Time step in milliseconds

    // Create and initialize battery
    struct Battery battery;
    initializeBattery(&battery, capacity, 4.2, 50.0); // Initial voltage and SoC assumed

    // Seed for random number generator (only once)
    srand(time(NULL));

    // Simulate running for 15 minutes
    for (int i = 0; i < 4500; i++) { // 15 minutes * 60 seconds / 2 ms = 4500 iterations
        // Randomly generate current
        float current = (((float)rand() / RAND_MAX) * 6.0) - 3.0; // Random current between -3 A and 3 A

        // Calculate time
        float time = (i + 1) * dt; // Time in milliseconds

        // Calculate voltage (example: linear decrease based on current)
        float voltage = battery.voltage_V - current * 0.01; // Linear decrease of 0.01 V for each A of current

        // Update battery state
        updateBattery(&battery, current, voltage, 25.0, time);

        // Estimate SoC
        estimateSoC(&battery, dt);
    }

    // Print estimated SoC after 15 minutes
    printf("Estimated SoC after 15 minutes: %.2f%%\n", battery.soc);

    return 0;
}

