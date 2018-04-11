#include "Mapper.h"
#include <unordered_map>
#include <cstdio>
using namespace std;


struct Mapping
{
    float *val, low, high;
};

//There may be some issues if "foo"!="foo"
unordered_map<const char *,Mapping> name_mapper;
unordered_map<int, const char *> port_mapper;


void Mapper::param(const char *name, float *val, float low, float high)
{
    Mapping map = {val, low, high};
    name_mapper[name] = map;
}
    
void Mapper::show(void)
{
    for(auto m:name_mapper)
        printf("%s = %f<%f,%f>\n",m.first, *m.second.val,m.second.low,
                m.second.high);
}

void Mapper::connect(int port, const char *name)
{
    port_mapper[port] = name;
}

void Mapper::handle(char ctl, char val)
{
    const char *handle = port_mapper[ctl];
    if(!handle) {
        printf("Unhandled control(%d)\n", ctl);
        return;
    }


    Mapping m = name_mapper[handle];
    float value = val/127.0f * (m.high-m.low) + m.low;
    *m.val = value;

    printf("updated %s to %f\n", handle, value);
}
