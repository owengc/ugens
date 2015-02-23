//
//  Node.h
//  AudioGraph_FM
//
//  Created by Owen Campbell on 2/16/15.
//
//

#ifndef __AudioGraph_FM__Node__
#define __AudioGraph_FM__Node__

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <memory>
#include <map>
#include <vector>

#include "Graph.h"
#include "Parameter.h"

using namespace std;

template<class TFloat>
class Graph;

template <class TFloat>
class Node {
    friend class Graph<TFloat>;
public:
    Node(const int numIns, const int numOuts) : _numIns(numIns), _numOuts(numOuts), _isHead(false){
        _inputs.resize(_numIns);
        for(int i = 0; i < _numIns; i++){
            shared_ptr<TFloat> in = make_shared<TFloat>();
            *in = 0.0;
            _inputs[i] = in;
        }
        _outputs.resize(_numOuts);
        for(int i = 0; i < _numOuts; i++){
            _outputs[i] = nullptr;
        }
        params.clear();
    };
    virtual ~Node(){
        
    };
    
    virtual void tick() = 0;
    
    void connect(const int outputID, const int inputID, shared_ptr<Node<TFloat>> destination){
        _outputs[outputID] = destination->_inputs[inputID];
    };
    
    void constant(const int inputID, TFloat source){
        *_inputs[inputID] = source;
    }
    
    virtual void setParameter(const string paramName, const TFloat newValue){
        try{
            params[paramName].setValue(newValue);
        }
        catch(exception){
            cout << "Attempting to setParameter with missing paramName" << endl;
        }
    };
    
    virtual TFloat getParameter(const string paramName){
        try{
            return params[paramName].getValue();
        }
        catch(exception){
            cout << "Attempting to getParameter with missing paramName" << endl;
        }
        return 0.0;
    };
    
    map< string, Parameter<TFloat> > params;
protected:
    vector<shared_ptr<TFloat>> _inputs;
    vector<shared_ptr<TFloat>> _outputs;
    const int _numIns, _numOuts;
    bool _isHead;
};






#endif /* defined(__AudioGraph_FM__Node__) */
