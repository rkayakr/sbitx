#include "wdsp_apf.h"

void apf_init(APFState* s, float fs){
  s->enabled = 0; s->fs = fs; s->f0 = 700.0f; s->Q = 5.f; s->gain_db = 6.f;
  s->track_peak = 1; s->bin_lo = 5; s->bin_hi = 150;
  biquad_init(&s->biq);
  biquad_design_peak(&s->biq, fs, s->f0, s->Q, s->gain_db);
}

void apf_set_params(APFState* s, float f0, float Q, float gain_db){
  s->f0=f0; s->Q=Q; s->gain_db=gain_db;
  biquad_design_peak(&s->biq, s->fs, s->f0, s->Q, s->gain_db);
}

void apf_set_tracking(APFState* s, int enable, int bin_lo, int bin_hi){
  s->track_peak = enable; s->bin_lo = bin_lo; s->bin_hi = bin_hi;
}

void apf_update_from_wpsd(APFState* s, const WpsdState* w, float bin_hz){
  if (!s->enabled || !s->track_peak) return;
  size_t k = wpsd_peak_bin_in_range(w, (size_t)s->bin_lo, (size_t)s->bin_hi);
  float f0 = k * bin_hz;
  apf_set_params(s, f0, s->Q, s->gain_db);
}

void apf_process(APFState* s, const float* in, float* out, size_t n){
  if (!s->enabled){ if (out!=in) for(size_t i=0;i<n;i++) out[i]=in[i]; return; }
  biquad_process(&s->biq, in, out, n);
}