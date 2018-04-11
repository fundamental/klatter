
class Kparam;
class State;
//Source for voiced and unvoiced phonemes
class Source
{
    public:
        Source(void);

        void process(unsigned nframes, float *smps);
        void process_par(unsigned nframes, float *smps);
        void update(const Kparam *oparam, const Kparam *param, const State *state);

        float ah_scale;
        float av, avs, ah, af, f;
    private:
        //Generate lookup table for voiced source
        void generate(unsigned size);
        //position of the sinasoids
        float pos;

        float    *osc_buf;
        unsigned  osc_size;

        float lpf;
        float plpf;
        int   i_pos;
};
