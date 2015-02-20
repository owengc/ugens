//
//  graphhead.h
//  AudioGraph_FM
//
//  Created by Owen Campbell on 2/19/15.
//
//

#ifndef __AudioGraph_FM__graphhead__
#define __AudioGraph_FM__graphhead__

#include <stdio.h>
#include "Node.h"

using namespace std;

template <class TFloat>
class GraphHead : virtual public Node<TFloat>{
public:
    GraphHead(const int numIns, const int numOuts, shared_ptr< Graph<TFloat> > graph) :
    Node<TFloat>(numIns, numOuts), _graph(graph){
        //connect head node outputs to graph outputs?
        for(int i = 0; i < Node<TFloat>::_numOuts; i++){
            Node<TFloat>::_outputs[i] = _graph->outputs[i];
        }
    };
    
    void tick(){
        if(Node<TFloat>::_numIns == Node<TFloat>::_numOuts){
            for(int i = 0; i < Node<TFloat>::_numOuts; i++){
                *Node<TFloat>::_outputs[i] = *Node<TFloat>::_inputs[i];
            }
        }
        else{
            TFloat sum = 0;
            for(int i = 0; i < Node<TFloat>::_numIns; i++){
                sum += *Node<TFloat>::_inputs[i];
            }
            sum /= (TFloat)Node<TFloat>::_numIns;
            for(int i = 0; i < Node<TFloat>::_numOuts; i++){
                *Node<TFloat>::_outputs[i] = sum;
            }
        }
    };
private:
    shared_ptr< Graph<TFloat> > _graph;
};

#endif /* defined(__AudioGraph_FM__graphhead__) */
