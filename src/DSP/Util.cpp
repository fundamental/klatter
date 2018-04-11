float circ_interp(const float *buf, unsigned size, float pos)
{
    const unsigned low  = pos;
    const unsigned high = (low+1)%size; 
    const float    rem  = pos-low;
    return buf[low]*rem+buf[high]*(1-rem);
}

void attenuate(unsigned nframes, float *dat, float attn)
{
    for(unsigned i=0; i<nframes; ++i)
        dat[i] *= attn;
}

void sum(unsigned nframes, float *dest, const float *src)
{
    for(unsigned i=0; i<nframes; ++i)
        dest[i] += src[i];
}

float limit(float val, float low, float high)
{
    if(val>high)
        return high;
    if(val<low)
        return low;
    return val;
}
