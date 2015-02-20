#include "Node.h"
//typedef enum {
//    ADD_IN1 = 0,
//    ADD_IN2,
//    ADD_NUM_PARAMS
//} ADD_PARAMS;
//
//typedef struct {
//    double *in1;
//    double *in2;
//    double *out;
//} ADD_DATA;
//
//void *add_construct();
//
//void add_process(void *data);
//
//void add_destroy(void *data);
//
//void add_set_param(void *data, int index, double value);
//
//double *add_get_input(void *data, int index);
//
//void add_set_output(void *data, int index, double *out_buffer);

using namespace std;

//Takes as input single floating point numbers, sums them, and distributes the result to each registered output
template <class TFloat>
class Add : virtual public Node<TFloat>{
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


