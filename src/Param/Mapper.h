namespace Mapper
{
    void param(const char *name, float *val, float low, float high);
    void connect(int port, const char *name);
    void show(void);
    void handle(char ctl, char val);
};
