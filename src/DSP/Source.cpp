#include "Source.h"
#include "Util.h"
#include "../Param/Formants.h"
#include "../Param/State.h"
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <cstddef>
#include <cstring>

extern float SAMPLE_RATE;
extern float interp_time;
        
Source::Source(void)
{
    ah_scale = 1.0f;
    f   = 100.0;
    pos = 0;
    av = 1.0f;
    ah = 0.0f;
    i_pos = 0;
    lpf = plpf = 0;
    generate(2048*16);
}
        
void Source::process(unsigned nframes, float *smps)
{
    float step = f/SAMPLE_RATE*osc_size;
    const int diff = SAMPLE_RATE/f;
    for(unsigned i=0; i<nframes; ++i, pos = fmodf(pos+step,(float)osc_size)) {
        smps[i] += avs*circ_interp(osc_buf, osc_size, pos);
        smps[i] += ah*lpf*ah_scale;
        lpf = lpf*7.0f/8.0f + (rand()*1.0f/RAND_MAX-0.5f)/4/8.0f;
        smps[i] += av*(i_pos ? 0.0f : 0.5f);

        i_pos = (i_pos+1)%diff;
    }
}
        
void Source::process_par(unsigned nframes, float *smps)
{
    for(unsigned i=0; i<nframes; ++i) {
        plpf = plpf*31.0f/32.0f + (rand()*1.0f/RAND_MAX-0.5f)/4/32.0f;
        smps[i] = af*plpf;
    }
}

        
void Source::update(const Kparam *oparam, const Kparam *param, const State *state)
{
    const float base_f = state->base_f;

    if(fmodf(state->ppos,1.0f) < interp_time) {
        const float tmp = fmodf(state->ppos, 1.0f)/interp_time;
        avs = oparam->avs*(1.0f-tmp) + param->avs*tmp;
        av  = oparam->av*(1.0f-tmp)  + param->av*tmp;
        ah  = oparam->ah*(1.0f-tmp)  + param->ah*tmp;
        af  = oparam->af*(1.0f-tmp)  + param->af*tmp;
        f   = (strstr(oparam->name,"1") ? base_f+15 :  base_f)*(1.0f-tmp)
            + (strstr(param->name,"1") ? base_f+15 :  base_f)*tmp;
    }
    else {
        avs = param->avs;
        av  = param->av;
        ah  = param->ah;
        af  = param->af;
        f   = (strstr(param->name,"1") ? base_f+15 :  base_f);
    }
    f = state->base_f;
}

void Source::generate(unsigned size)
{
    const unsigned harmonics=128;
    osc_size = size;
    osc_buf  = new float[osc_size];

    memset(osc_buf,0,sizeof(float)*osc_size);

    //radians per sample
    const float d_w = 2*M_PI/osc_size;

    float _pos = 0.0f;
    //populate with harmonics
    for(unsigned i=0; i<osc_size; ++i) {
        _pos = fmodf(_pos+d_w, 2*M_PI);
        for(unsigned j=0; j<harmonics; ++j)
            osc_buf[i] += cos(j*_pos)/harmonics;
    }
}
