#pragma once
#include <complex.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float psd[2048];      // Smoothed power spectrum (size must cover MAX_BINS)
  float noise[2048];    // Noise floor estimate per bin
  float alpha_psd;      // e.g., 0.90
  float alpha_noise;    // e.g., 0.995
  size_t n_bins;
  int initialized;
} WpsdState;

void wpsd_init(WpsdState* s, size_t n_bins, float alpha_psd, float alpha_noise);
void wpsd_update_from_bins(WpsdState* s, const complex float* bins, size_t n_bins);
float wpsd_bin_snr(const WpsdState* s, size_t i);
size_t wpsd_peak_bin_in_range(const WpsdState* s, size_t i0, size_t i1);
float wpsd_bin_power(const WpsdState* s, size_t i);

#ifdef __cplusplus
}
#endif