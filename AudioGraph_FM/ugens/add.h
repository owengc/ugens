//
//  add.h
//  AudioGraph_FM
//
//  Created by Owen Campbell on 2/19/15.
//
//

#ifndef __AudioGraph_FM__add__
#define __AudioGraph_FM__add__

#include "Node.h"

using namespace std;

//Takes as input single floating point numbers, sums them, and distributes the result to each registered output
template <class TFloat>
class Add : public Node<TFloat>{
public:
    Add(const int numIns, const int numOuts) : Node<TFloat>("add_" + to_string(_instanceCounter++), numIns, numOuts){
    
    };
    
    void tick(){
        TFloat sum = 0;
        for(int i = 0; i < Node<TFloat>::_numIns; i++){
            sum += *Node<TFloat>::_inputs[i];
        }
        for(int i = 0; i < Node<TFloat>::_numOuts; i++){
            *Node<TFloat>::_outputs[i] = sum;
        }
    };
    static unsigned int _instanceCounter;
};

template<typename TFloat>
unsigned int Add<TFloat>::_instanceCounter = 0;

using add_f = Add<float>;
using add_d = Add<double>;

#endif /* defined(__AudioGraph_FM__add__) */