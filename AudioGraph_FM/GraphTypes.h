//
//  GraphTypes.h
//  AudioGraph_FM
//
//  Created by Owen Campbell on 2/21/15.
//  Copyright (c) 2015 Owen Campbell. All rights reserved.
//

#ifndef AudioGraph_FM_GraphTypes_h
#define AudioGraph_FM_GraphTypes_h

#include <memory>
#include "Node.h"
#include "Graph.h"
#include "Parameter.h"
#include "Ugens.h"

//TODO: update with appropriate smart pointer type

template<class TFloat> using node = shared_ptr<Node<TFloat>>;
using node_f = shared_ptr<Node<float>>;
using node_d = shared_ptr<Node<double>>;
template<class TFloat> using graph = shared_ptr<Graph<TFloat>>;
using graph_f = shared_ptr<Graph<float>>;
using graph_d = shared_ptr<Graph<double>>;
template<class TFloat> using parameter = shared_ptr<Parameter<TFloat>>;
using parameter_f = shared_ptr<Parameter<float>>;
using parameter_d = shared_ptr<Parameter<double>>;

#endif
