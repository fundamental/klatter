#include "Formant.h"
#include <complex>

extern float SAMPLE_RATE;

Formant::Formant(void)
:f(0.0f), bw(0.0f)
{
    //clear history
    hist[0] = 0.0f;
    hist[1] = 0.0f;
}

void Formant::process(unsigned nframes, float *smps)
{
    const float _f  = f/SAMPLE_RATE;
    const float _bw = bw/SAMPLE_RATE;
    //Pole location
    std::complex<float> pole = std::polar<float>(expf(-M_PI*_bw), 2*M_PI*_f);
    //Calculate coefficents
    const float B= real(pole+conj(pole));
    const float C= -real(pole*conj(pole));

    //Process frame
    for(unsigned i=0; i<nframes; ++i) {
        smps[i]  = smps[i]*(1.0-B-C)+B*hist[0]+C*hist[1];
        hist[1]  = hist[0];
        hist[0]  = smps[i];
    }
}

