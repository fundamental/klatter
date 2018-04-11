
//Full Text to speech
const char *utterence[] = 
{
    //"sweeps", "the", "floors", "every", "night", 0,
    "work", "it", "make", "it", "do", "it", "makes", "us","sil1",
    "harder", "better", "faster", "stronger","sil1",
    "more", "than", "hour", "our", "never", "ever","sil1",
    "after", "work", "is", "never", "over", "sil1", 0,
    0,
    "hello", "world", "sil",
    "how","are", "you", "sil",
    "difficult", "sil", 
    "are", "bad", "bar", "bask", "box", "cane", "cleanse", "clove", "crash",
    "creed", "death", "deed", "dike", "dish", "end", "feast", "fern", "folk",
    "ford", "fraud", "fuss", "grove", "heap", "hid", "hive", "hunt", "is",
    "sil", 0};

//const char *word_test[] = {"cane", "sil", 0};

class Utterance
{
    public:
        Utterance(const char **_dat)
            :dat(_dat)
        {}

        const Word &operator[](unsigned i) const
        {
            return *word_get(dat[i]);
        }
        const char **dat;
};
