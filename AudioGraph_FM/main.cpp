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
#include <iostream>
#include <memory>

#include "Graph.h"
#include "ugens/sinosc.h"
#include "ugens/mult.h"
#include "ugens/add.h"
#include "ugens/expenv.h"
#include "ugens/graphhead.h"


//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}

typedef struct
{
    shared_ptr<Graph<float>> graph;
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
    unsigned int i, j, numOuts = data->numOuts;
    (void) in; /* Prevent unused variable warning. */
    for( i=0; i<framesPerBuffer; i++ ) {
        data->graph->tick();
        for(j = 0; j < numOuts; j++){
            *out++ = *data->graph->outputs[j];
        }
    }
    return 0;
}



int main(void)
{
    PaError err;
    int sr = 44100;
    paData data;
    
    
    double modindexamp = 3;
    double carrierfreq = 220;
    double cmratio = 1.414;
    double modfreq = carrierfreq * cmratio;

    data.numOuts = 2;
    data.graph = make_shared<Graph<float>>(data.numOuts);
    
    shared_ptr<Node<float>> modulator = make_shared<Sinosc<float>>(2, 1, sr, modfreq, 0.5);
    shared_ptr<Node<float>> carrier = make_shared<Sinosc<float>>(2, 1, sr, carrierfreq, 0.5);
    shared_ptr<Node<float>> adder = make_shared<Add<float>>(2, 1);
    shared_ptr<Node<float>> multiplier = make_shared<Mult<float>>(2, 1);
    shared_ptr<Node<float>> carrenv = make_shared<Expenv<float>>(1, 1, sr, 1, 0.05, 0.01, 1, 0.7, 3, 2);
    shared_ptr<Node<float>> modindex = make_shared<Expenv<float>>(1, 1, sr, modindexamp, 0.1, 0.1, 0.5, 1, 3, 1);
    shared_ptr<Node<float>> head = make_shared<GraphHead<float>>(1, 2, data.graph);
    
    //    carrier->ugen_instance = sinosc_construct(sr, carrierfreq, 0.5);
    //    carrier->tick = sinosc_process;
    //    carrier->destroy = sinosc_destroy;
    //
    //    multiplier->ugen_instance = mult_construct();
    //    multiplier->tick = mult_process;
    //    multiplier->destroy = mult_destroy;
    //
    //    adder->ugen_instance = add_construct();
    //    adder->tick = add_process;
    //    adder->destroy = add_destroy;
    //
    //    carrenv->ugen_instance = expenv_construct(sr, 1, 0.05, 0.01, 1, 0.7, 3, 2);
    //    carrenv->tick = expenv_process;
    //    carrenv->destroy = expenv_destroy;
    //
    //    modindex->ugen_instance = expenv_construct(sr, modindexamp, 0.1, 0.1, 0.5, 1, 3, 1);
    //    modindex->tick = expenv_process;
    //    modindex->destroy = expenv_destroy;
    
    
    shared_ptr<float> addvalue = make_shared<float>();
    *addvalue = carrierfreq;
    adder->connect(1, addvalue);

    modulator->connect(0, 0, multiplier);
    multiplier->connect(0, 0, adder);
    adder->connect(0, (int)Sinosc<float>::INPUTS::FREQ, carrier);
    carrenv->connect(0, (int)Sinosc<float>::INPUTS::AMP, carrier);
    modindex->connect(0, 1, multiplier);
    carrier->connect(0, 0, head);
    
    data.graph->nodes.push_back(modulator);
    data.graph->nodes.push_back(modindex);
    data.graph->nodes.push_back(multiplier);
    data.graph->nodes.push_back(adder);
    data.graph->nodes.push_back(carrenv);
    data.graph->nodes.push_back(carrier);
    data.graph->nodes.push_back(head);
    
    //    modulator->next = modindex;
    //    modindex->next = multiplier;
    //    multiplier->next = adder;
    //    adder->next = carrenv;
    //    carrenv->next = carrier;
    //    carrier->next = NULL;
    

    
//    sinosc_set_output(modulator->ugen_instance, 0,
//                      mult_get_input(multiplier->ugen_instance, 0));
//    mult_set_output(multiplier->ugen_instance, 0,
//                    add_get_input(adder->ugen_instance, 0));
//    add_set_output(adder->ugen_instance, 0,
//                   sinosc_get_input(carrier->ugen_instance, SINOSC_FREQ));
//    sinosc_set_output(carrier->ugen_instance, 0,
//                      &data.out1);
//    expenv_set_output(carrenv->ugen_instance, 0,
//                      sinosc_get_input(carrier->ugen_instance, SINOSC_AMP));
//    expenv_set_output(modindex->ugen_instance, 0,
//                      mult_get_input(multiplier->ugen_instance, 1));
    
   
    
    err = Pa_Initialize();
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    PaStream *stream;
    err = Pa_OpenDefaultStream( &stream,
                               2, /* no input channels */
                               2, /* stereo output */
                               paFloat32, /* 32 bit floating point output */
                               sr,
                               64, /* frames per buffer */
                               paCallback, /* this is your callback function */
                               &data ); /*This is a pointer that will be passed to
                                         your callback*/
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    err = Pa_StartStream( stream );
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    Pa_Sleep(5*1000);
    
    err = Pa_StopStream( stream );
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    err= Pa_CloseStream(stream);
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    err = Pa_Terminate();
    if( err != paNoError )
        printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    //    while (data.graphHead) {
    //        data.graphHead->destroy(data.graphHead->ugen_instance);
    //        graph_node_t *oldNode = data.graphHead;
    //        data.graphHead = data.graphHead->next;
    //        free(oldNode);
    //    }
    
    printf("Hello World!\n");
    return 0;
}
