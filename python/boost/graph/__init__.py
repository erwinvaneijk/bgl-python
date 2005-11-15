# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine
from _support import *
from _graph import *
from bfs_visitor import bfs_visitor
from bellman_ford_visitor import bellman_ford_visitor
from dfs_visitor import dfs_visitor
from dijkstra_visitor import dijkstra_visitor

# Introduce documentation for all of the algorithms in the module.
# Doing so here is *much* easier than doing it in the C++ code,
# because we don't need to recompile to change documentation and we
# have the option to do fancier things within the Python code to make
# our lives easier.
import docstring
documentor = docstring.docstring()

# The actual documentation
breadth_first_search.__doc__ = documentor.function('breadth_first_search') \
    .parameter('graph', 
               'the graph on which the breadth-first search will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the vertex where the breadth-first search will originate.') \
    .parameter('buffer', 'the queue used by the breadth-first search. If not supplied, a simple FIFO queue will be used.', 'None') \
    .parameter('visitor', 'a visitor that will receive events as the breadth-first search progresses. Typically this visitor should be derived from boost.graph.bfs_visitor.', 'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 
               'graph.vertex_property_map(\'color\')') \
    .paragraph('Performs a breadth-first search on the given graph starting at a particular vertex.') \
    .see_also('bfs_visitor') \
    .see_also('breadth_first_visit') \
    .see_also('depth_first_search') \
    .__str__()
  
