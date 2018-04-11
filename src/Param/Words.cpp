#include "Formants.h"
#include "Words.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <err.h>

static const Kparam *param_get_by_name(const char *c)
{
    for(int i=0; i<PHONES; ++i)
        if(strcmp(c,ptable[i].name)==0)
            return ptable+i;

    //failed to find phone
    errx(1, "There must be a phone with name %s", c);
}


static int min(int a, int b)
{
    return a<b?a:b;
}

Phone::Phone(const char *n)
{
    name = param_get_by_name(n)->name;
    len  = 1;
    for(const Kparam &k:ptable) {
        int ml = min(strlen(name),strlen(k.name));
        if(strncmp(name, k.name, ml) == 0 && k.name[ml]=='1')
            len  = 2; 
    }
}
    
const Kparam *Phone::operator[](int i) const
{
    if(i==0) {
        for(const Kparam &k:ptable)
            if(strcmp(name, k.name) == 0)
                return &k;
    }
    else if(i==1) {
        for(const Kparam &k:ptable) {
            int ml = min(strlen(name),strlen(k.name));
            if(strncmp(name, k.name, ml) == 0
                    && k.name[ml]=='1')
                return &k;
        }
    }
    fprintf(stderr,"%d %d %s\n", i,len,name);
    assert(NULL);
}
    
unsigned Word::length(void) const
{
    int len=1;
    const char *s = seq;
    while((s=strstr(s+1,"-")))
        len++;
    return len+2;//and a sil
}
    
const Phone Word::operator[](unsigned i) const
{
    if(i>=(length()-2))
        return "sil";
    static char phone_data[6];
    const char *s = seq;

    //Get to phoneme start
    while(i--)
        s = strstr(s+1, "-");
    if(*s=='-')
        s++;

    //Copy data to static buffer
    char *itr=phone_data; 
    for(;*s&&*s!='-'; ++s,++itr)
        *itr = *s;
    *itr = 0;
    return phone_data;
}

const Word *word_get(const char *w)
{
    for(const Word &wd : wtable)
        if(strcmp(wd.name,w)==0)
            return &wd;

    errx(1, "word %s is undefined", w);
    return 0;
}
