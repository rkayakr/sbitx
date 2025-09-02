#include "wdsp_pipeline.h"
#include "wdsp_wpsd.h"
#include "wdsp_apf.h"
#include <math.h>

#ifndef MAX_BINS
#define MAX_BINS 2048
#endif

static WpsdState g_wpsd;
static APFState  g_apf;
static size_t    g_bins = 0;
static float     g_fs = 48000.f;
static float     g_bin_hz = 0.f;

void wdsp_init(size_t n_bins, float sample_rate){
  g_bins = n_bins;
  g_fs = sample_rate;
  g_bin_hz = sample_rate / (float)n_bins;
  wpsd_init(&g_wpsd, n_bins, 0.90f, 0.995f);
  apf_init(&g_apf, sample_rate);
}

void wdsp_set_wpsd_smoothing(float a_psd, float a_noise){
  g_wpsd.alpha_psd = a_psd;
  g_wpsd.alpha_noise = a_noise;
}

void wdsp_process_bins(complex float* bins, size_t n_bins){
  if (n_bins > g_bins) n_bins = g_bins;

  // Update WPSD state
  wpsd_update_from_bins(&g_wpsd, bins, n_bins);

  // Global spectral subtraction using WPSD noise
  for (size_t i=0;i<n_bins;i++){
    float mag = cabsf(bins[i]);
    float ph  = cargf(bins[i]);
    float n   = g_wpsd.noise[i];

    // Sigmoid-based reduction (smoothness) + residual preservation
    float snr = mag / (n + 1e-6f);
    float reduction = 1.0f / (1.0f + expf(-5.0f * (snr - 0.5f)));
    float residual = 0.10f * n;
    float new_mag = fmaxf(residual, mag - reduction * n);

    bins[i] = new_mag * cexpf(I*ph);
  }

  // If APF is tracking, update center frequency from WPSD
  if (g_apf.enabled && g_apf.track_peak){
    apf_update_from_wpsd(&g_apf, &g_wpsd, g_bin_hz);
  }
}

void wdsp_set_apf_enabled(int on){ g_apf.enabled = on; }
void wdsp_set_apf_params(float f0_hz, float Q, float gain_db){ apf_set_params(&g_apf, f0_hz, Q, gain_db); }
void wdsp_set_apf_tracking(int enable, int bin_lo, int bin_hi, float bin_hz){
  (void)bin_hz; // implicit from init
  apf_set_tracking(&g_apf, enable, bin_lo, bin_hi);
}

void wdsp_process_audio(float* inout, size_t n){
  apf_process(&g_apf, inout, inout, n);
}