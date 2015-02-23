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
const float inc = 2.0 / sr;


float * writePos = sfBuffer;
float val = -1.0;

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
            assert(fabsf(temp) <= 1.0);
            *out++ = *(data->g->outputs[j]);
        }
        if(val >= 1.0){
            val = -1.0;
        }
        else{
            val += inc;
        }
        *writePos++ = val;
    }
    return 0;
}



int main(void)
{
    PaError err;
    paData data;
    SF_INFO sfinfo;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfinfo.samplerate = sr;
    char filename[16];
    sprintf(filename, "output.wav");
    sf = sf_open(filename, SFM_WRITE, &sfinfo);
    if (!sf) {
        printf("Error opening file!\n");
        return -2;
    }
    
    float modindexamp = 2;
    float carrierfreq = 220;
    float cmratio = 1.414;
    float modfreq = carrierfreq * cmratio;

    data.numOuts = 2;
    data.g = graph<float>(new Graph<float>(data.numOuts));

    //TODO: check envelope to see if it's clipping
    
    node<float> modulator = data.g->insertNode(node<float>(new Sinosc<float>(2, 1, sr, modfreq, 0.5)));
    node<float> modindex = data.g->insertNode(node<float>(new Expenv<float>(1, 1, sr, modindexamp, 0.1, 0.1, 0.5, 1, 3, 1)));
    node<float> multiplier = data.g->insertNode(node<float>(new Mult<float>(2, 1)));
    node<float> adder = data.g->insertNode(node<float>(new Add<float>(2, 1)));
    node<float> carrenv = data.g->insertNode(node<float>(new Expenv<float>(1, 1, sr, 1, 0.05, 0.01, 1, 0.7, 3, 2)));
    node<float> carrier = data.g->insertNode(node<float>(new Sinosc<float>(2, 1, sr, carrierfreq, 0.5)));
    node<float> head = data.g->insertNode(node<float>(new GraphHead<float>(1, 2)));

    modulator->connect(0, 0, multiplier);
    modindex->connect(0, 1, multiplier);
    multiplier->connect(0, 0, adder);
    adder->constant(1, carrierfreq);
    adder->connect(0, (int)Sinosc<float>::INPUTS::FREQ, carrier);
    carrenv->connect(0, (int)Sinosc<float>::INPUTS::AMP, carrier);
    carrier->connect(0, 0, head);
    
    
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
    
    Pa_Sleep(duration * 1000);
    
    err = Pa_StopStream( stream );
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    err= Pa_CloseStream(stream);
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
