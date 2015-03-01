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
class Node : public enable_shared_from_this<Node<TFloat>>{
    friend class Graph<TFloat>;
public:
    Node(const int numIns, const int numOuts) : _numIns(numIns), _numOuts(numOuts), _isHead(false){
        _inputs.resize(_numIns);
        _sources.resize(_numIns);
        for(int i = 0; i < _numIns; i++){
            _inputs[i] = nullptr;
            _sources[i] = nullptr;
        }
        _outputs.resize(_numOuts);
        for(int i = 0; i < _numOuts; i++){
            _outputs[i] = nullptr;
        }
        params.clear();
    };
    virtual ~Node(){
        
    };
    
    void traverse(){
        for(int i = 0; i < _numIns; i++){
            if(_inputs[i] == nullptr){
                //cout << "Input " << i << " of " + _name + " has no source!" << endl;
                abort();
            }
            else if(_sources[i] != nullptr){
                _sources[i]->traverse();
            }
        }
        tick();
    }

    virtual void tick() = 0;

    void connect(const int outputID, const int inputID, shared_ptr<Node<TFloat>> destination){
        _outputs[outputID] = destination->_inputs[inputID];
        destination->_sources[inputID] = shared_from_this();
    };
    
    void constant(const int inputID, TFloat value){
        _inputs[inputID] = make_shared<TFloat>(value);
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
    vector<shared_ptr<Node<TFloat>>> _sources;
    const int _numIns, _numOuts;
    bool _isHead;
    const string _name;
};






#endif /* defined(__AudioGraph_FM__Node__) */
