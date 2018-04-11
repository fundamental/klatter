
//Circular linear interpolation
float circ_interp(const float *buf, unsigned size, float pos);
void attenuate(unsigned nframes, float *dat, float attn);
void sum(unsigned nframes, float *dest, const float *src);
float limit(float val, float low, float high);
