//
//  main.cpp
//  AudioGraph_FM
//
//  Created by Owen Campbell on 2/19/15.
//  Copyright (c) 2015 Owen Campbell. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "sndfile.h"
#include <iostream>
#include <memory>
#include <limits>
#include <math.h>
#include <assert.h>

#include "GraphTypes.h"


//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}

SNDFILE * sf;
const int BLOCK_SIZE = 64;
const int sr = 44100;
const float duration = 5;
const int sfSamples = duration * sr;
float sfBuffer[sfSamples];



float * writePos = sfBuffer;
//const float inc = 2.0 / sr;
//float val = -1.0;

typedef struct
{
    graph<float> g;
    int numOuts;
} paData;

static int paCallback( const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData )
{
    /* Cast data passed through stream to our structure. */
    paData *data = (paData*)userData;
    float *out = (float*)outputBuffer;
    float *in = (float*)inputBuffer;
    unsigned int i, j;
    float temp;
    (void) in; /* Prevent unused variable warning. */
    for( i=0; i<framesPerBuffer; i++ ){
        data->g->tick();
        for(j = 0; j < data->numOuts; j++){
            temp = *data->g->outputs[j];
            // assert(fabsf(temp) <= 1.0);
            *out++ = *(data->g->outputs[j]);
        }
//        if(val >= 1.0){
//            val = -1.0;
//        }
//        else{
//            val += inc;
//        }
//        *writePos++ = val;
        *writePos++ = *out;
    }
    return 0;
}



int main(void)
{
    PaError err;
    paData data;
      data.numOuts = 2;
    data.g = graph<float>(new Graph<float>(data.numOuts));
    char filename[16];

    
#define CIRCUIT 1
    
#if ( CIRCUIT == 1)
    sprintf(filename, "circuit1.wav");
    //declare constants
    float carrAmp = 0.5;
    float carrFreq = 100;
    float modAmp = 400;
    float modFreq =  100;
    
    //create ugen graph
    node_f modulator = data.g->insertNode(node_f(new sinosc_f(2, 1, sr, modFreq, modAmp)));
    node_f adder = data.g->insertNode(node_f(new add_f(2, 1)));
    node_f carrier = data.g->insertNode(node_f(new sinosc_f(2, 1, sr, carrFreq, carrAmp)));
    node_f head = data.g->insertNode(node_f(new graphhead_f(1, 2)));
    
    //initialize constants and connect ugens
    modulator->constant(SINOSC_FREQ, modFreq);
    modulator->constant(SINOSC_AMP, modAmp);
    
    adder->constant(0, carrFreq);
    modulator->connect(0, 1, adder);
    adder->connect(0, SINOSC_FREQ, carrier);
    
    carrier->constant(SINOSC_AMP, carrAmp);
    carrier->connect(0, 0, head);
    
#elif (CIRCUIT == 2)
    sprintf(filename, "circuit2.wav");
    //declare constants
    float modIndex = 0;
    float modIndexDev = 1;
    float carrAmp = 0.5;
    float carrFreq = 440;
    float modFreq =  220;
    
    //create ugen graph
    //Expenv(const int numIns, const int numOuts, const int sr, const TFloat amp, const TFloat att, const TFloat dec,
    //    const TFloat sus, const TFloat suslvl, const TFloat rel, const TFloat curve) :
    node_f carrenv = data.g->insertNode(node_f(new expenv_f(1, 1, sr, carrAmp,
                                                             0.35 * duration, 0.05 * duration, 0.2 * duration, 0.3 * duration,
                                                             0.7 * carrAmp, 2)));
    node_f modenv = data.g->insertNode(node_f(new expenv_f(1, 1, sr, modIndexDev,
                                                            0.05 * duration, 0.2 * duration, 0.2 * duration, 0.55 * duration,
                                                            0.5 * modIndexDev, 3)));
    node_f adder1 = data.g->insertNode(node_f(new add_f(2, 1)));
    node_f mult = data.g->insertNode(node_f(new mult_f(2, 1)));
    node_f modulator = data.g->insertNode(node_f(new sinosc_f(2, 1, sr, 1, 1)));
    node_f adder2 = data.g->insertNode(node_f(new add_f(2, 1)));

    node_f carrier = data.g->insertNode(node_f(new sinosc_f(2, 1, sr, carrFreq, 1.0)));
    node_f head = data.g->insertNode(node_f(new graphhead_f(1, 2)));
    
    //initialize constants and connect ugens

    adder1->constant(0, modIndex);
    modenv->connect(0, 1, adder1);
    adder1->connect(0, 1, mult);
    mult->constant(0, modFreq);
    mult->connect(0, SINOSC_AMP, modulator);
    
    modulator->constant(SINOSC_FREQ, modFreq);
    adder2->constant(0, carrFreq);
    modulator->connect(0, 1, adder2);
    
    adder2->connect(0, SINOSC_FREQ, carrier);
    carrenv->connect(0, SINOSC_AMP, carrier);

    carrier->connect(0, 0, head);

#elif (CIRCUIT == 3)
    sprintf(filename, "circuit3.wav");
    //declare constants
    float modIndex = 0;
    float modIndexDev = 1;
    float carrAmp = 0.5;
    float carrFreq1 = 300;
    float carrFreq2 = 2100;
    float modFreq =  300;
    
    //create ugen graph
    //Expenv(const int numIns, const int numOuts, const int sr, const TFloat amp, const TFloat att, const TFloat dec,
    //    const TFloat sus, const TFloat suslvl, const TFloat rel, const TFloat curve) :
    node_f carrenv = data.g->insertNode(node_f(new expenv_f(1, 2, sr, carrAmp,
                                                            0.35 * duration, 0.05 * duration, 0.2 * duration, 0.3 * duration,
                                                            0.7 * carrAmp, 2)));
    node_f modenv = data.g->insertNode(node_f(new expenv_f(1, 1, sr, modIndexDev,
                                                           0.05 * duration, 0.2 * duration, 0.2 * duration, 0.55 * duration,
                                                           0.5 * modIndexDev, 3)));
    node_f adder1 = data.g->insertNode(node_f(new add_f(2, 1)));
    node_f modulator = data.g->insertNode(node_f(new sinosc_f(2, 2, sr, 1, 1)));
    
    node_f mult1 = data.g->insertNode(node_f(new mult_f(2, 1)));
    node_f adder2 = data.g->insertNode(node_f(new add_f(2, 1)));
    node_f adder3 = data.g->insertNode(node_f(new add_f(2, 1)));

    node_f mult2 = data.g->insertNode(node_f(new mult_f(2, 1)));
    node_f mult3 = data.g->insertNode(node_f(new mult_f(2, 1)));
    node_f carrier1 = data.g->insertNode(node_f(new sinosc_f(2, 1, sr, carrFreq1, 1.0)));
    node_f carrier2 = data.g->insertNode(node_f(new sinosc_f(2, 1, sr, carrFreq2, 1.0)));

    node_f adder4 = data.g->insertNode(node_f(new add_f(2, 1)));
    node_f head = data.g->insertNode(node_f(new graphhead_f(1, 2)));
    
    //initialize constants and connect ugens
    
    adder1->constant(0, modIndex);
    modenv->connect(0, 1, adder1);
    adder1->connect(0, 1, mult1);
    mult1->constant(0, modFreq);
    mult1->connect(0, SINOSC_AMP, modulator);
    
    modulator->constant(SINOSC_FREQ, modFreq);
    adder2->constant(0, 80.0);
    modulator->connect(0, 1, adder2);

    modulator->connect(1, 0, mult2);
    mult2->constant(1, 0.5);
    
    mult2->connect(0, 0, adder3);
    adder3->constant(1, carrFreq2);

    adder2->connect(0, SINOSC_FREQ, carrier1);
    carrenv->connect(0, SINOSC_AMP, carrier1);
    
    carrenv->connect(1, 0, mult3);
    mult3->constant(1, 0.2);
    
    mult3->connect(0, SINOSC_AMP, carrier2);
    adder3->connect(0, SINOSC_FREQ, carrier2);
    
    carrier1->connect(0, 0, adder4);
    carrier2->connect(0, 1, adder4);
    
    adder4->connect(0, 0, head);

#endif
    
    
    SF_INFO sfinfo;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfinfo.samplerate = sr;
    sf = sf_open(filename, SFM_WRITE, &sfinfo);
    if (!sf) {
        printf("Error opening file!\n");
        return -2;
    }
    
    err = Pa_Initialize();
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    PaStream *stream;
    err = Pa_OpenDefaultStream( &stream,
                               0, /* no input channels */
                               2, /* stereo output */
                               paFloat32, /* 32 bit floating point output */
                               sr,
                               BLOCK_SIZE, /* frames per buffer */
                               paCallback, /* this is your callback function */
                               &data ); /*This is a pointer that will be passed to
                                         your callback*/
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    err = Pa_StartStream( stream );
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    Pa_Sleep(duration * 1000 + (2 * BLOCK_SIZE/sr));
    
    err = Pa_StopStream( stream );
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    err = Pa_CloseStream(stream);
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    err = Pa_Terminate();
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
  
    sf_writef_float(sf, sfBuffer, sfSamples);

    sf_close(sf);

    printf("Hello World!\n");
    return 0;
}
