#include "audio.h"
//#include "mapper.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t doRun = 1;
void handleSigint(int) {
    doRun = 0;
}

void die(int, void*)
{
    printf("\nDieing as expected...\n");
    audio_close();
    //mapper_close();
}
    
void signals_setup()
{
    signal(SIGINT, handleSigint);
    on_exit(die, NULL);
}

int main(int argc, char *argv[])
{

    (void)argc;
    (void)argv;
    //puts("initializing mapper...");
    //mapper_setup();
    puts("initializing audio...");
    audio_setup();
    puts("initializing signals...");
    signals_setup();
    puts("going into main loop...");

	while(doRun)
        sleep(1);

    puts("exiting normally...");
    return 0;
}

