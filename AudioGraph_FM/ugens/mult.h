#include "Node.h"
//typedef enum {
//    MULT_IN1 = 0,
//    MULT_IN2,
//    MULT_NUM_PARAMS
//} MULT_PARAMS;
//
//typedef struct {
//    double *in1;
//    double *in2;
//    double *out;
//} MULT_DATA;
//
//void *mult_construct();
//
//void mult_process(void *data);
//
//void mult_destroy(void *data);
//
//void mult_set_param(void *data, int index, double value);
//
//double *mult_get_input(void *data, int index);
//
//void mult_set_output(void *data, int index, double *out_buffer);

//Takes as input single floating point numbers, sums them, and distributes the result to each registered output
using namespace std;

template <class TFloat>
class Mult : virtual public Node<TFloat>{
public:
    Mult(const int numIns, const int numOuts) : Node<TFloat>(numIns, numOuts){
        
    };
    
    void tick(){
        TFloat product = 1.0;
        for(int i = 0; i < Node<TFloat>::_numIns; i++){
            product *= *Node<TFloat>::_inputs[i];
        }
        for(int i = 0; i < Node<TFloat>::_numOuts; i++){
            *Node<TFloat>::_outputs[i] = product;
        }
    };
};



