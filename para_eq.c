#include "para_eq.h"
//Parametric TX EQ implementation W2JON
//Set up nice and flat to begin with.
//Note: limit gain range -16 to +16 (further limitation testing required)
float read_value(FILE* file, const char* key, float default_value) {
    char line[256];
    char* found;
    float value = default_value;

    rewind(file); // Reset the file pointer to the beginning
    while (fgets(line, sizeof(line), file)) {
        if ((found = strstr(line, key)) != NULL) {
            sscanf(found + strlen(key) + 1, "%f", &value);
            break;
        }
    }
    return value;
}


void init_eq(ParametricEQ* eq) {
    char directory[200];	//dangerous, find the MAX_PATH and replace 200 with it
    char* path = getenv("HOME");
    strcpy(directory, path);
    strcat(directory, "/sbitx/data/user_settings.ini");
    FILE* file = fopen(directory, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char key[10];

    // Default values
    eq->bands[0].frequency = 100.0;
    eq->bands[0].gain = 0.0;
    eq->bands[0].bandwidth = 1.0;

    eq->bands[1].frequency = 250.0;
    eq->bands[1].gain = 0.0;
    eq->bands[1].bandwidth = 1.0;

    eq->bands[2].frequency = 1000.0;
    eq->bands[2].gain = 0.0;
    eq->bands[2].bandwidth = 1.0;

    eq->bands[3].frequency = 4000.0;
    eq->bands[3].gain = 0.0;
    eq->bands[3].bandwidth = 1.0;

    eq->bands[4].frequency = 8000.0;
    eq->bands[4].gain = 0.0;
    eq->bands[4].bandwidth = 1.0;

    for (int i = 0; i < NUM_BANDS; i++) {
        snprintf(key, sizeof(key), "#b%df", i);
        eq->bands[i].frequency = read_value(file, key, eq->bands[i].frequency);

        snprintf(key, sizeof(key), "#b%dg", i);
        eq->bands[i].gain = read_value(file, key, eq->bands[i].gain);

        snprintf(key, sizeof(key), "#b%db", i);
        eq->bands[i].bandwidth = read_value(file, key, eq->bands[i].bandwidth);
    }

    fclose(file);
}

typedef struct {
    double a0, a1, a2, b0, b1, b2;
    double x1, x2, y1, y2;
} Biquad;

void calculate_coefficients(EQBand* band, double sample_rate, Biquad* filter) {
    double A = pow(10.0, band->gain / 40.0);
    double omega = 2.0 * M_PI * band->frequency / sample_rate;
    double alpha = sin(omega) * sinh(log(2.0) / 2.0 * band->bandwidth * omega / sin(omega));

    filter->b0 = 1.0 + alpha * A;
    filter->b1 = -2.0 * cos(omega);
    filter->b2 = 1.0 - alpha * A;
    filter->a0 = 1.0 + alpha / A;
    filter->a1 = -2.0 * cos(omega);
    filter->a2 = 1.0 - alpha / A;

    // Normalize the coefficients
    filter->b0 /= filter->a0;
    filter->b1 /= filter->a0;
    filter->b2 /= filter->a0;
    filter->a1 /= filter->a0;
    filter->a2 /= filter->a0;
    filter->a0 = 1.0;
}

int32_t process_sample(Biquad* filter, int32_t sample) {
    double result = filter->b0 * sample + filter->b1 * filter->x1 + filter->b2 * filter->x2
        - filter->a1 * filter->y1 - filter->a2 * filter->y2;
    filter->x2 = filter->x1;
    filter->x1 = sample;
    filter->y2 = filter->y1;
    filter->y1 = result;
    return (int32_t)result;
}

void apply_eq(ParametricEQ* eq, int32_t* samples, int num_samples, double sample_rate) {
    Biquad filters[5];
    for (int i = 0; i < 5; i++) {
        calculate_coefficients(&eq->bands[i], sample_rate, &filters[i]);
    }

    for (int n = 0; n < num_samples; n++) {
        int32_t sample = samples[n];
        for (int i = 0; i < 5; i++) {
            sample = process_sample(&filters[i], sample);
        }
        samples[n] = sample;
    }
}

//---
