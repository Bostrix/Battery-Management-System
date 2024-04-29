#include <stdio.h>
#include <stdlib.h> // For generating random numbers

// Structure to hold battery parameters and state
typedef struct {
    double capacity_mAh;  // Battery capacity in mAh
    double current_mA;    // Current in mA
    double voltage_V;     // Voltage in V
    double temperature_C; // Temperature in Celsius
    double time_ms;       // Time in milliseconds
    double soc;           // State of Charge (SoC)
} Battery;

// Initialize the battery with initial parameters
void initializeBattery(Battery *battery, double capacity_mAh, double initialVoltage_V, double initialSoC) {
    battery->capacity_mAh = capacity_mAh;
    battery->current_mA = 0.0; // Initial current is 0
    battery->voltage_V = initialVoltage_V;
    battery->temperature_C = 2.0; // Initial temperature is 25 Celsius
    battery->time_ms = 0.0; // Initial time is 0 milliseconds
    battery->soc = initialSoC;
}

// Update battery state based on current, voltage, temperature, and time
void updateBattery(Battery *battery, double current_mA, double voltage_V, double temperature_C, double time_ms) {
    // Update current, voltage, temperature, and time
    battery->current_mA = current_mA;
    battery->voltage_V = voltage_V;
    battery->temperature_C = temperature_C;
    battery->time_ms = time_ms;
}

// Estimate SoC using Coulomb counting method for charging scenario
void estimateSoC(Battery *battery) {
    // Calculate charge accumulated over time (in Coulombs)
    double charge_C = (battery->current_mA / 1000.0) * (battery->time_ms / 3600.0); // mAh to Ah, ms to hours

    // Calculate SoC as a percentage of total capacity
    battery->soc = battery->soc + (charge_C / battery->capacity_mAh) * 100.0;

    // Ensure SoC is within valid range [0, 100]
    if (battery->soc < 0.0) {
        battery->soc = 0.0;
    } else if (battery->soc > 100.0) {
        battery->soc = 100.0;
    }
}

int main() {
    // Initialize battery with capacity of 1000mAh, initial voltage of 4.2V, and initial SoC of 50%
    Battery battery;
    initializeBattery(&battery, 1000.0, 100.2, 50.0);

    // Define parameters for measurement updates
    double voltage_V = 4.1;       // Measured voltage in V
    double temperature_C = 30.0;  // Measured temperature in Celsius
    double time_ms = 0.0;         // Time elapsed in milliseconds
    double time_step_ms = 1.0;    // Time step for each iteration in milliseconds

    // Simulate battery discharge
    while (time_ms <= 800.0) { // Iterate for 800 milliseconds
        // Generate random current value for each iteration
        double current_mA = (rand() % 1000) - 500.0; // Random current between -500mA to +500mA

        // Update battery state
        updateBattery(&battery, current_mA, voltage_V, temperature_C, time_ms);

        // Estimate SoC using Coulomb counting
        estimateSoC(&battery);

	// Print estimated SoC
	printf("Time: %.1f ms, Current: %.2f mA\n", time_ms, current_mA);
	printf("Estimated SoC: %.9f%%\n", battery.soc);

        // Increment time by time step
        time_ms += time_step_ms;
    }

    return 0;
}
