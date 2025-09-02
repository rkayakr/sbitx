#include "dsp_biquad.h"
#include <math.h>

void biquad_init(Biquad* q){ q->b0=1;q->b1=q->b2=q->a1=q->a2=0;q->a0=1;q->z1=q->z2=0; }

void biquad_design_peak(Biquad* q, float fs, float f0, float Q, float gain_db){
  const float A = powf(10.f, gain_db/40.f);
  const float w0 = 2.f*(float)M_PI*(f0/fs);
  const float alpha = sinf(w0)/(2.f*Q);
  const float cosw0 = cosf(w0);

  float b0 = 1 + alpha*A;
  float b1 = -2*cosw0;
  float b2 = 1 - alpha*A;
  float a0 = 1 + alpha/A;
  float a1 = -2*cosw0;
  float a2 = 1 - alpha/A;

  q->b0 = b0/a0; q->b1 = b1/a0; q->b2 = b2/a0;
  q->a0 = 1.f;   q->a1 = a1/a0; q->a2 = a2/a0;
}

void biquad_process(Biquad* q, const float* in, float* out, size_t n){
  float z1=q->z1, z2=q->z2;
  for(size_t i=0;i<n;i++){
    float x = in[i];
    float y = q->b0*x + z1;
    z1 = q->b1*x - q->a1*y + z2;
    z2 = q->b2*x - q->a2*y;
    out[i] = y;
  }
  q->z1=z1; q->z2=z2;
}