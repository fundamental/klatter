#include "State.h"
#include "Words.h"
#include <cassert>

State::State(const char *utt[])
:upos(0),wpos(0),ppos(0),
    ulen(0),data(utt), base_f(196/2.0f)
{
    while(utt[ulen])
        ++ulen;
    assert(ulen);

    wlen = word_get(utt[0])->length();
    assert(wlen);

    plen = word_get(utt[0])[0][0].len;
}
    
void State::tick(void)
{
    ppos += 0.20;//0.03

    if(ppos >= plen) {
        wpos += 1;
        ppos -= plen;
    }

    if(wpos==wlen)
        next_word();
    plen = (*word_get(data[upos]))[wpos].len;
}
    
void State::next_word(void)
{
    upos = (upos+1)%ulen;
    wpos = 0;
    wlen = word_get(data[upos])->length();
    assert(wlen);
    //base_f = 110.0f;//dater[upos]/4.0f;
    base_f = daft_fq[upos]/2.0f;
}
