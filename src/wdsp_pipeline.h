#pragma once
#include <complex.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void wdsp_init(size_t n_bins, float sample_rate);
void wdsp_set_wpsd_smoothing(float alpha_psd, float alpha_noise);

// Replace legacy dsp globally: always apply WPSD NR on FFT bins
void wdsp_process_bins(complex float* bins, size_t n_bins);

// Optional APF on time-domain audio
void wdsp_set_apf_enabled(int on);
void wdsp_set_apf_params(float f0_hz, float Q, float gain_db);
void wdsp_set_apf_tracking(int enable, int bin_lo, int bin_hi, float bin_hz);
void wdsp_process_audio(float* inout, size_t n);

#ifdef __cplusplus
}
#endif