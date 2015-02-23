//
//  Graph.h
//  AudioGraph_FM
//
//  Created by Owen Campbell on 2/17/15.
//
//

#ifndef __AudioGraph_FM__Graph__
#define __AudioGraph_FM__Graph__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <assert.h>

#include "Node.h"

using namespace std;

template<class TFloat>
class Node;

template <class TFloat>
class Graph {
public:
    Graph(const int numOuts) : _numOuts(numOuts), _hasHead(false), _head(nullptr){
        _nodes.resize(0);
        outputs.resize(_numOuts);
    };
    Graph(const Graph<TFloat>& g){
        _nodes = g.nodes;
        outputs.resize(g.outputs.size());
    };
    
    const Graph& operator= (const Graph& rhs){
        _nodes = rhs.nodes;
        outputs.resize(rhs.outputs.size());
    };
    ~Graph(){
        
    };
    
    shared_ptr<Node<TFloat>> insertNode(shared_ptr<Node<TFloat>> n){
        if(n->_isHead){
            //assuming final node has as many outputs as the graph
            _hasHead = true;
            _head = n;
            for(int i = 0; i < _numOuts; i++){
                outputs[i] = _head->_outputs[i];
            }
        }
        _nodes.push_back(n);
        return *(_nodes.end()-1);
    };
    
    void tick(){
        assert(_hasHead);
        unsigned long graphSize = _nodes.size();
        for (int i = 0; i < graphSize; i++){
            _nodes[i]->tick();
        }
    };

    vector<shared_ptr<TFloat>> outputs;
private:
    int _numOuts;
    vector<shared_ptr<Node<TFloat>>> _nodes;
    bool _hasHead;
    shared_ptr<Node<TFloat>> _head;
};

#endif /* defined(__AudioGraph_FM__Graph__) */
