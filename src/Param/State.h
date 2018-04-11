
class State
{
    public:
        State(const char *utt[]);

        void tick(void);

        void next_word(void);

        //Positions within the current utterence, word, and phoneme
        int upos,wpos;
        float ppos;

        //Length of utterence, word, and phoneme
        int ulen,wlen,plen;

        //actual utterance data
        const char **data;

        float base_f;
};
