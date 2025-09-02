#pragma once
#include <stddef.h>

typedef struct {
  float b0,b1,b2,a0,a1,a2;
  float z1,z2;
} Biquad;

void biquad_init(Biquad* q);
void biquad_design_peak(Biquad* q, float fs, float f0, float Q, float gain_db);
void biquad_process(Biquad* q, const float* in, float* out, size_t n);