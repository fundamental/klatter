#include <mapper/mapper_types.h>
#include <mapper/mapper.h>
#include <unistd.h>
#include <stdio.h>

struct vtable
{
    public:
        const char *name;
        float *data;
};

void mapper_update(mapper_signal msig, mapper_db_signal props, mapper_timetag_t*, void *v)
{
    vtable *data = (vtable*)props->user_data;
    float res = *(float*)v;
    printf("Updating %s from %f to %f\n",data->name,*data->data,res);
    *data->data = res;
}


float float_data[8];



vtable vdata[8] = {
    {"/data/e0", float_data},
    {"/data/e1", float_data+1},
    {"/data/e2", float_data+2},
    {"/data/e3", float_data+3},
    {"/data/e4", float_data+4},
    {"/data/e5", float_data+5},
    {"/data/e6", float_data+6},
    {"/data/e7", float_data+7},
};

mapper_device mdev;
void mapper_setup(void)
{
    mdev = mdev_new("klatter", 0, 0);
    while(!mdev_ready(mdev))
        mdev_poll(mdev, 10);
    printf("libmapper ready...\n");
    
    for(int i=0; i<8; ++i) {
        mdev_add_input(mdev, vdata[i].name,
                        1, 'f', "ms",0, 0,
                        mapper_update, (void*)(vdata+i));
        printf("Adding %s...\n", vdata[i].name);
    }
}

void mapper_tick(void)
{
    mdev_poll(mdev, 0);
}
    
void mapper_close(void)
{
    mdev_free(mdev);
}
