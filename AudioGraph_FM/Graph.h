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

#include "Node.h"

using namespace std;

template <class TFloat>
class Graph {
public:
    Graph(const int numOuts) : _numOuts(numOuts){
        nodes.resize(0);
        outputs.resize(0);
        for(int i = 0; i < _numOuts; i++){
            shared_ptr<TFloat> out = make_shared<TFloat>();
            *out = 0.0;
            outputs.push_back(out);
        }
    };
    Graph(const Graph<TFloat>& g){
        nodes = g.nodes;
        outputs.resize(g.outputs.size());
    };
    
    const Graph& operator= (const Graph& rhs){
        nodes = rhs.nodes;
        outputs.resize(rhs.outputs.size());
    };
    ~Graph(){
        
    };
    
    void tick(){
        unsigned long graphSize = nodes.size();
        for (int i = 0; i < graphSize; i++){
            //nodes[i].tick();
            nodes.at(i)->tick();
        }
        //assuming final node has as many outputs as the graph
    };
    vector<shared_ptr<Node<TFloat>>> nodes;
    vector<shared_ptr<TFloat>> outputs;
private:
    int _numOuts;
};

#endif /* defined(__AudioGraph_FM__Graph__) */
