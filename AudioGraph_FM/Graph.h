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
    Graph() : _hasHead(false), _head(nullptr){};
    Graph(const Graph<TFloat>& g) : _head(make_shared<Node<TFloat>>(*g._head)){};
    Graph(Graph&& rhs) : _head(move(rhs._head)){};

    const Graph& operator= (Graph&& rhs){
        swap(*this, rhs);
        return *this;
    };
    
    ~Graph(){};
    
    shared_ptr<Node<TFloat>> createNode(shared_ptr<Node<TFloat>> n){
        if(n->_isHead){
            //assuming final node has as many outputs as the graph
            //TODO: validate connections (make sure there aren't any nullptrs)
            _hasHead = true;
            _head = n;
            for(int i = 0; i < _head->_numOuts; i++){
                *_head->_outputs[i] = 0.0;
            }
        }
        _nodes[n->_id] = n;
        return n;
    };
    
    void tick(){
        //unsigned long graphSize = _nodes.size();
        _head->traverse(std::chrono::high_resolution_clock::now());
    };
    
    TFloat output(unsigned int channel){
        return *_head->_outputs[channel];
    }
    
private:
    bool _hasHead;
    map<unsigned int, shared_ptr<Node<TFloat>>> _nodes;
    shared_ptr<Node<TFloat>> _head;
};

#endif /* defined(__AudioGraph_FM__Graph__) */
