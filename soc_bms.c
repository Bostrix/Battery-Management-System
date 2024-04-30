#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h> // for usleep()
#include <stdbool.h> // for boolean type

// Battery parameters
#define CAPACITY_MAH 2600
#define VOLTAGE_FULL 4.2f
#define VOLTAGE_EMPTY 2.5f
#define CURRENT_LEAK 0.1f
#define TEMPERATURE_FACTOR 0.02f
#define CHARGING_EFFICIENCY 0.95f // Charging efficiency (e.g., 95%)
#define DISCHARGING_EFFICIENCY 0.98f // Discharging efficiency (e.g., 98%)
#define TIME_STEP 0.002f // 2 ms

// Struct for battery state
typedef struct {
    float capacity_mah;
    float voltage_full;
    float voltage_empty;
    float current_leak;
    float current;
    float voltage;
    float temperature;
    float soc;
    float remaining_capacity;
    float time;
    bool dead; // changed from int to bool
} Batt;

// Initialize battery state
void batteryInitialize(Batt *b, float capacity_mah, float voltage_full, float voltage_empty, float current_leak) {
    b->capacity_mah = capacity_mah;
    b->voltage_full = voltage_full;
    b->voltage_empty = voltage_empty;
    b->current_leak = current_leak;
    b->current = 0;
    b->voltage = voltage_full;
    b->temperature = 25; // Initialize temperature to a default value
    b->soc = 1.0f;
    b->remaining_capacity = capacity_mah;
    b->time = 0;
    b->dead = false; // initialize dead as false
}

// Update battery state based on current and time
void batteryUpdate(Batt *b, float current, float time_step) {
    b->current = current;
    b->time += time_step;

    float absolute_current = fabs(current);
    float efficiency = (current > 0) ? CHARGING_EFFICIENCY : DISCHARGING_EFFICIENCY;

    // Coulomb Counting method for estimating SoC
    float delta_capacity = absolute_current * time_step * efficiency;
    if (current > 0) {
        // Charging
        b->remaining_capacity += delta_capacity;
        if (b->remaining_capacity > b->capacity_mah) {
            b->remaining_capacity = b->capacity_mah;
        }
    } else {
        // Discharging
        b->remaining_capacity -= delta_capacity;
        if (b->remaining_capacity < 0) {
            b->remaining_capacity = 0;
        }
    }
    b->soc = b->remaining_capacity / b->capacity_mah;

    b->voltage = b->voltage_full * powf(b->soc, 1.2f) + absolute_current * b->temperature * TEMPERATURE_FACTOR;
    if (b->voltage <= b->voltage_empty) {
        b->dead = true;
    }
}

int main() {
    srand(time(NULL)); // seed the random number generator

    // initialize battery state
    Batt battery;
    batteryInitialize(&battery, CAPACITY_MAH, VOLTAGE_FULL, VOLTAGE_EMPTY, CURRENT_LEAK);

    // input parameters
    float voltage, temperature;

    // prompt user to enter voltage and temperature once
    printf("Enter voltage (V): ");
    scanf("%f", &voltage);
    printf("Enter temperature (°C): ");
    scanf("%f",&temperature);

    // cycle counter
    int cycles = 0;

    // run simulation continuously until interrupted or desired number of cycles is reached
    while (cycles < 10000) {
        float current;
        if (cycles < 5000) {
            // Discharging
            current = -((float)rand() / RAND_MAX) * 5.0f;
        } else {
            // Charging
            current = ((float)rand() / RAND_MAX) * 5.0f;
        }

        // update battery state
        batteryUpdate(&battery, current, TIME_STEP);

        // set temperature
        battery.temperature = temperature;

        // print current and time
        printf("Current (A): %.2f\n", current);
        printf("Time (s): %.2f\n", battery.time);
        printf("\n");

        // increment cycle counter
        cycles++;

        // sleep for 2 milliseconds
        usleep(TIME_STEP * 1000000);
    }

    // print estimated SoC at the end of the simulation
    printf("Estimated state of charge: %.2f%%\n", battery.soc * 100);

    return 0;
}
