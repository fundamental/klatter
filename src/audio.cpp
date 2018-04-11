#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <cassert>
#include <unistd.h>
#include <err.h>

#include <jack/jack.h>
#include <jack/midiport.h>

float SAMPLE_RATE=0;
float interp_time = 0.7;


#include "mapper.h"
#include "Param/Formants.h"
#include "Param/Words.h"
#include "Param/Utterance.h"
#include "DSP/Formant.h"
#include "DSP/Util.h"
#include "Param/State.h"
#include "DSP/Source.h"
//#include "Param/Mapper.h"

Utterance utt(utterence);
Source  source;
Formant formants[6];
Formant forpar[6];
float attn[6];

jack_port_t *left;
jack_port_t *iport;
jack_client_t *client;


const Kparam *get_param(State *state)
{
    //Increment state
    state->tick();

#if 0
    //Show changes in phonems
    printf(" <%d %d %f:%d %d %d>[%f %f %f %f %f][%d/%d %d/%d %d/%d][%s](%s)             \r",
            state->upos, state->wpos, state->ppos,
            state->ulen, state->wlen, state->plen,
            source.av,source.avs,source.ah, source.af,source.f,
            (int)formants[0].f,(int)formants[0].bw,
            (int)formants[1].f,(int)formants[1].bw,
            (int)formants[2].f,(int)formants[2].bw,
            utt[state->upos].name,
            utt[state->upos][state->wpos][(int)state->ppos]->name);
    fflush(stdout);
#endif

    state->wpos %= 100;
    
    return utt[state->upos][state->wpos][(int)state->ppos];
}

void formant_update(const Kparam *oparam, const Kparam *param, const State *state, bool force)
{
    //static higher formants
    forpar[3].f  = formants[3].f  = 3300;
    forpar[3].bw = formants[3].bw = 250;
    forpar[4].f  = formants[4].f  = 3750;
    forpar[4].bw = formants[4].bw = 200;
    forpar[5].f  = formants[5].f  = 4900;
    forpar[5].bw = formants[5].bw = 1000;
    
    //Update formants
    if(strcmp(param->name,"sil")!=0) {
        for(int i=0; i<3; ++i) {
            if(fmodf(state->ppos,1.0f) > interp_time || force) {
                formants[i].f  = param->f[i];
                formants[i].bw = param->b[i];
            }
            else { //linearly interpolate
                const float tmp = fmodf(state->ppos, 1.0f)/interp_time;
                formants[i].f  = oparam->f[i]*(1.0f-tmp) + param->f[i]*tmp;
                formants[i].bw = oparam->b[i]*(1.0f-tmp) + param->b[i]*tmp;
            }
            forpar[i].f    = formants[i].f;
            forpar[i].bw   = formants[i].bw;
        }
    }
}

float attenuation = 128.0f;

int synthesize(unsigned nframes, float *smps, State *state)
{
    static bool was_sil = false;
    const Kparam *param = get_param(state);

    static const Kparam *oparam = param;
    static const Kparam *pparam = param;
    static int pw=-1,pp=-1;
    if(pw != state->wpos || pp != (int)state->ppos) {
        pw = state->wpos;
        pp = (int) state->ppos;
        oparam = pparam;
        pparam = param;
    }
    
    formant_update(oparam, param, state, was_sil && strcmp(param->name,"sil")!=0);
    source.update(oparam, param, state);
    
    was_sil = strcmp(param->name,"sil")==0;
    
    //Generate sound
    source.process(nframes, smps);

    //generate noise for parallel filter
    float parallel[6][nframes];
    source.process_par(nframes, parallel[0]);
    for(int i=1; i<6; ++i)
        memcpy(parallel[i], parallel[0], nframes*sizeof(float));

    //Process 
    for(int i=0; i<4; ++i)
        formants[i].process(nframes, smps);

    //Process the parallel elements
    if(fmodf(state->ppos,1.0f) < interp_time) {
        const float tmp = fmodf(state->ppos, 1.0f)/interp_time;
        attn[0] = oparam->a[0]*(1.0-tmp) + param->a[0]*tmp;
    } else
        attn[0] = param->a[0];
    attenuate(nframes, parallel[0], attn[0]/attenuation);
    sum(nframes, smps, parallel[0]);//bypass

    for(int i=2; i<6; ++i) {
        if(fmodf(state->ppos,1.0f) < interp_time) {
            const float tmp = fmodf(state->ppos, 1.0f)/interp_time;
            attn[i] = oparam->a[i]*(1.0-tmp) + param->a[i]*tmp;
        } else
            attn[i] = param->a[i];
        attenuate(nframes, parallel[i], attn[i]/128.0f);
        forpar[i].process(nframes, parallel[i]);
        sum(nframes, smps, parallel[i]);
    }

	return 0;      
}

int process(unsigned nframes, void *_state)
{
    State *state = static_cast<State*>(_state);
    
    //Handle midi first
    void *midi_buf = jack_port_get_buffer(iport, nframes);
    jack_midi_event_t ev;
    jack_nframes_t event_idx = 0;
    while(jack_midi_event_get(&ev, midi_buf, event_idx++) == 0) {
        //Note ON
        if((ev.buffer[0]&0xf0) == 0x90)
            state->base_f = 440.0f * powf(2.0f, (ev.buffer[1]-69.0f)/12.0f);
        //Controller event
        //if((ev.buffer[0]&0xf0) == 0xB0)
        //    Mapper::handle(ev.buffer[1], ev.buffer[2]);
    };


    //Tick inputs
    //mapper_tick();
    
    //Get samples
	float *smps = (float*)jack_port_get_buffer(left, nframes);

    //clear samples
    memset(smps,0,sizeof(float)*nframes);

    return synthesize(nframes, smps, state);
}

void jack_shutdown(void *)
{
    puts("shutting down from within jack...");
	exit (1);
}

void audio_close(void)
{
    printf("\nClosing Jack...\n");
    jack_client_close(client);
}


State state(utterence);
void audio_setup(void)
{
    //Mapper::param("attenuation", &attenuation, 64.0f, 512.0f);
    //Mapper::param("asperation", &source.ah_scale, 0.0f, 1.0f);
    ////Mapper::param("interpolation_time", &interp_time, 0.001f, 1.0f);
    //Mapper::connect(0,"interpolation_time");
    //Mapper::connect(1,"asperation");
    //Mapper::connect(2,"attenuation");
	const char *client_name = "klatter";
	jack_status_t status;

	client = jack_client_open(client_name, JackNullOption, &status, NULL);
    if(!client) {
        if(status & JackServerFailed)
            errx(1, "Failed to connect to JACK server");
        else
            errx(1, "jack_client_open() error, status 0x%2.0x", status);
    }

	if(status & JackServerStarted)
		warn("JACK server started"), sleep(1);
	if(status & JackNameNotUnique)
        errx(1, "%s is already in use", client_name);

	jack_set_process_callback(client, process, &state);
	jack_on_shutdown(client, jack_shutdown, 0);

    printf("engine sample rate: %u\n", jack_get_sample_rate(client));
    SAMPLE_RATE = jack_get_sample_rate(client);

	left = jack_port_register(client, "left",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

	if(!left)
		errx(1, "no more JACK ports available");


    //Make midi port
    iport = jack_port_register(client, "input",
            JACK_DEFAULT_MIDI_TYPE,
            JackPortIsInput | JackPortIsTerminal, 0);


    puts("activating jack...");
	if(jack_activate(client))
		errx(1, "cannot activate client");
    
    //connect to output
    const char **outPorts = jack_get_ports(client, NULL, NULL, 
            JackPortIsPhysical | JackPortIsInput);
    jack_connect(client, jack_port_name(left), outPorts[0]);
    jack_connect(client, jack_port_name(left), outPorts[1]);
}
