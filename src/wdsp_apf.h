#pragma once
#include <stddef.h>
#include "dsp_biquad.h"
#include "wdsp_wpsd.h"

typedef struct {
  int enabled;
  float fs;          // sample rate (audio path)
  float f0;          // center frequency in Hz
  float Q;           // quality factor
  float gain_db;     // boost
  int track_peak;    // if 1, track WPSD peak in a bin range
  int bin_lo, bin_hi; // inclusive bin window to search
  Biquad biq;
} APFState;

void apf_init(APFState* s, float fs);
void apf_set_params(APFState* s, float f0, float Q, float gain_db);
void apf_set_tracking(APFState* s, int enable, int bin_lo, int bin_hi);
void apf_update_from_wpsd(APFState* s, const WpsdState* w, float bin_hz);
void apf_process(APFState* s, const float* in, float* out, size_t n);