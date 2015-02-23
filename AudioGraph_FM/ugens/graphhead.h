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
#include "Graph.h"

using namespace std;

template <class TFloat>
class GraphHead : public Node<TFloat>{
public:
    GraphHead(const int numIns, const int numOuts) :
    Node<TFloat>(numIns, numOuts){
        Node<TFloat>::_isHead = true;
        for(int i = 0; i < numOuts; i++){
            Node<TFloat>::_outputs[i] = make_shared<TFloat>(0);
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
    
};

#endif /* defined(__AudioGraph_FM__graphhead__) */
