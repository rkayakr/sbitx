#include "wdsp_wpsd.h"
#include <math.h>
#include <string.h>
#ifndef MAX_BINS
#define MAX_BINS 2048
#endif

static inline float pwr_cpx(complex float x){ float re=crealf(x), im=cimagf(x); return re*re+im*im; }

void wpsd_init(WpsdState* s, size_t n_bins, float alpha_psd, float alpha_noise){
  memset(s, 0, sizeof(*s));
  s->n_bins = n_bins;
  s->alpha_psd = alpha_psd;
  s->alpha_noise = alpha_noise;
  s->initialized = 0;
}

void wpsd_update_from_bins(WpsdState* s, const complex float* bins, size_t n_bins){
  if (n_bins > s->n_bins) n_bins = s->n_bins;
  for (size_t i=0;i<n_bins;i++){
    float p = pwr_cpx(bins[i]);
    if (!s->initialized){
      s->psd[i] = p;
      s->noise[i] = fmaxf(1e-9f, p);
    } else {
      s->psd[i]   = s->alpha_psd   * s->psd[i]   + (1.f - s->alpha_psd)   * p;
      float n_in = fminf(p, s->noise[i]);
      s->noise[i] = s->alpha_noise * s->noise[i] + (1.f - s->alpha_noise) * n_in;
      s->noise[i] = fmaxf(1e-9f, s->noise[i]);
    }
  }
  s->initialized = 1;
}

float wpsd_bin_snr(const WpsdState* s, size_t i){
  if (i >= s->n_bins) return 0.f;
  float n = fmaxf(1e-9f, s->noise[i]);
  return s->psd[i] / n;
}

size_t wpsd_peak_bin_in_range(const WpsdState* s, size_t i0, size_t i1){
  if (i1 > s->n_bins) i1 = s->n_bins;
  size_t argmax = i0;
  float vmax = -1.f;
  for (size_t i=i0; i<i1; i++){
    if (s->psd[i] > vmax){ vmax = s->psd[i]; argmax = i; }
  }
  return argmax;
}

float wpsd_bin_power(const WpsdState* s, size_t i){
  if (i >= s->n_bins) return 0.f;
  return s->psd[i];
}