#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h> // for usleep()

// Battery parameters
#define CAPACITY_MAH 2600
#define VOLTAGE_FULL 4.2
#define VOLTAGE_EMPTY 2.5
#define CURRENT_LEAK 0.1
#define TEMPERATURE_FACTOR 0.02

// Simulation parameters
#define TIME_STEP 0.002 // 2 ms

// Struct for battery state
typedef struct {
    double capacity_mah;
    double voltage_full;
    double voltage_empty;
    double current_leak;
    double current;
    double voltage;
    double temperature;
    double soc;
    double remaining_capacity;
    double time;
    int dead;
} Batt;

// Initialize battery state
void batteryInitialize(Batt *b, double capacity_mah, double voltage_full, double voltage_empty, double current_leak) {
    b->capacity_mah = capacity_mah;
    b->voltage_full = voltage_full;
    b->voltage_empty = voltage_empty;
    b->current_leak = current_leak;
    b->current = 0;
    b->voltage = voltage_full;
    b->temperature = 25; // Initialize temperature to a default value
    b->soc = 1.0;
    b->remaining_capacity = capacity_mah;
    b->time = 0;
    b->dead = 0;
}

// Update battery state based on current and time
void batteryUpdate(Batt *b, double current, double time_step) {
    b->current = current;
    b->time += time_step;
    b->remaining_capacity -= current * time_step;
    b->soc = b->remaining_capacity / b->capacity_mah;
    if (b->soc > 1.0) { // Ensure SoC doesn't exceed 100%
        b->soc = 1.0;
        b->remaining_capacity = b->capacity_mah;
    }
    b->voltage = b->voltage_full * pow(b->soc, 1.2) + b->current * b->temperature * TEMPERATURE_FACTOR;
    if (b->voltage <= b->voltage_empty) {
        b->dead = 1;
    }
}

// Estimate state of charge using Coulomb Counting
double coulombCounting(Batt *b) {
    double delta_capacity = b->current * TIME_STEP;
    b->remaining_capacity -= delta_capacity;
    b->soc = b->remaining_capacity / b->capacity_mah;
    if (b->soc > 1.0) { // Ensure SoC doesn't exceed 100%
        b->soc = 1.0;
        b->remaining_capacity = b->capacity_mah;
    }
    return b->soc;
}

int main() {
    srand(time(NULL)); // seed the random number generator

    // initialize battery state
    Batt battery;
    batteryInitialize(&battery, CAPACITY_MAH, VOLTAGE_FULL, VOLTAGE_EMPTY, CURRENT_LEAK);

    // input parameters
    double voltage, temperature;

    // prompt user to enter voltage and temperature once
    printf("Enter voltage (V): ");
    scanf("%lf", &voltage);
    printf("Enter temperature (°C): ");
    scanf("%lf", &temperature);

    // run simulation continuously until interrupted
    while (1) {
        // generate random current
        double current = (((double)rand() / RAND_MAX) * 10.0) - 5.0; // random current between -5 and 5 A

        // update battery state
        batteryUpdate(&battery, current, TIME_STEP);
        battery.temperature = temperature;

        // estimate state of charge
        double soc = coulombCounting(&battery);

        // print current, time, and estimated state of charge
        printf("Current (A): %.2f\n", current);
        printf("Time (s): %.2f\n", battery.time);
        printf("Estimated state of charge: %.2f%%\n", soc * 100);
        printf("\n");

        // sleep for 2 milliseconds
        usleep(TIME_STEP * 1000000);
    }

    return 0;
}
