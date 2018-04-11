class Kparam;

struct Phone
{
    Phone(const char *n);
    const Kparam *operator[](int i) const;

    const char *name;
    unsigned len;
};


struct Word
{
    const char *name;
    const char *seq;
    unsigned length(void) const;

    const Phone operator[](unsigned i) const;
};


const float daft_fq[] = {196, 196, 392, 392, 233, 233, 466, 466,0,
                         293, 261, 220, 146,0,
                         196, 196, 392, 392, 233, 233, 0, 
                         466, 466, 196, 261, 293, 110,0};

const Word wtable[] =
{
    {"hello",       "he-eee-l-ow"},
    {"world",       "w-ue-r-l-d"},
    {"how",         "ho-ow"},
    {"you",         "y-uw"},
    {"fricative",   "f-r-Ie-k-A-t-Ie-v"},
    {"difficult",   "d-Ie-f-Ie-k-e-l-t"},
    {"mary",        "m-a-r-y"},
    {"sil",         "sil-sil-sil-sil-sil"},
    {"sil1",        "sil"},

    //Comparison words
    {"sweeps",      "s-w-ey-p-s"},
    {"the",         "tht-ee"},
    {"floors",      "f-l-ow-r-s"},
    {"every",       "eee-v-r-j"},
    {"night",       "n-A-y-t"},

    //Daft punk
    {"work",        "w-ue-r-k"},
    {"it",          "Ie-t"},
    {"make",        "m-ey-k"},
    {"do",          "d-ue"},
    {"makes",       "m-ey-k-s"},
    {"us",          "A-s"},
    {"harder",      "he-a-hr-r-d-e-r"},
    {"better",      "b-eee-t-e-r"},
    {"faster",      "f-aee-s-t-e-r"},
    {"stronger",    "s-t-r-aw-g-e-r"},
    {"more",        "m-aw-r"},
    {"than",        "tht-a-n"},
    {"hour",        "ho-ow-ee-r"},
    {"our",         "ow-r"},
    {"never",       "n-n-eee-v-r"},
    {"ever",        "ee-v-r"},
    {"after",       "aee-f-t-ee-r"},
    {"over",        "ow-v-ee-r"},


    //PB list
    {"are",         "A-r-r"},
    {"bad",         "b-aee-d"},
    {"bar",         "b-A-r"},
    {"bask",        "b-a-s-k"},
    {"box",         "b-ow-k-s"},
    {"cane",        "chat-e1-y-n"},
    {"cleanse",     "k-l-e-n-s"},
    {"clove",       "k-l-oy-v-e"},
    {"crash",       "k-r-a-he-s"},
    {"creed",       "k-r-ey-d"},
    {"death",       "d-eee-t-he"},
    {"deed",        "d-Ie-d"},
    {"dike",        "d-a-A-hy-y-k-k"},
    {"dish",        "d-Ie-s-he"},
    {"end",         "he-n-d"},
    {"feast",       "f-ey-s-t"},
    {"fern",        "f-ee-r-n"},
    {"folk",        "f-ow-l-k"},
    {"ford",        "f-ow-r-d"},
    {"fraud",       "f-r-aw-d"},
    {"fuss",        "f-uw-s"},
    {"grove",       "g-r-ue-v-e"},
    {"heap",        "he-aee-p"},
    {"hid",         "he-Ie-d"},
    {"hive",        "he-a-hy-y-v"},
    {"hunt",        "he-uw-n-t"},
    {"is",          "Ie-s"},
};


#if 0
mange
no
nook
not
pan
pants
pest
pile
plush
rag
rat
ride
rise
rub
slip
smile
strife
such
then
there
toe
use
wheat
#endif

const Word *word_get(const char *w);
