
//A single formant filter
class Formant
{
    public:
        Formant(void);

        //parameters in normalized frequency (0..1)
        float f;
        float bw;

        void process(unsigned nframes, float *smps);
    private:
        float hist[2];
};
