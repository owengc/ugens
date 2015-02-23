#include "Node.h"

using namespace std;

//Takes as input single floating point numbers, sums them, and distributes the result to each registered output
template <class TFloat>
class Add : public Node<TFloat>{
public:
    Add(const int numIns, const int numOuts) : Node<TFloat>(numIns, numOuts){
    
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
};


