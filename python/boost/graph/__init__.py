# Copyright (C) 2005, 2006 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

# Filter out some annoying Boost.Python warnings that expose our dirty
# laundry.
import warnings
warnings.filterwarnings("ignore", category=RuntimeWarning, module="boost.graph");

from boost.graph._support import *
from boost.graph._graph import *
from astar_visitor import astar_visitor
from bfs_visitor import bfs_visitor
from bellman_ford_visitor import bellman_ford_visitor
from dfs_visitor import dfs_visitor
from dijkstra_visitor import dijkstra_visitor

try:
  from show import show
except:
  pass

# Documentation that is not part of the compiled extension modules.

__doc__ = """Python Bindings for the Boost Graph Library

This package provides Python access to the components of the Boost
Graph Library (written in C++). The primary classes are Graph and
Digraph, which are adjacency list data structures used to represent
graphs (or networks).

More information about the Boost Graph Library is available on several
web pages:

  The Boost Library Collection - http://www.boost.org
  The Boost Graph Library - http://www.boost.org/libs/graph/doc
  The BGL-Python Package - http://www.osl.iu.edu/~dgregor/bgl-python

Copyright (C) 2005-2007 The Trustees of Indiana University.

Use, modification and distribution is subject to the Boost Software
License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
http:#www.boost.org/LICENSE_1_0.txt)

Authors: Douglas Gregor
         Ben Martin
         Andrew Lumsdaine
"""

__license__ = 'Boost Software License, Version 1.0'
__version__ = '0.9.5'
__copyright__ = "Copyright (C) 2005-2007 The Trustees of Indiana University"
__author__ = "Douglas Gregor, Ben Martin, and Andrew Lumsdaine"

# Introduce documentation for all of the algorithms in the module.
# Doing so here is *much* easier than doing it in the C++ code,
# because we don't need to recompile to change documentation and we
# have the option to do fancier things within the Python code to make
# our lives easier.
import docstring
_documentor = docstring.docstring()

# Data structure documentation
Graph.__doc__ = """
An undirected graph data structure.

An undirected graph (also called a network) is a data structure that
contains a set of vertices and relationships between pairs of vertices,
called edges. Given two vertices u and v, an edge (u, v) indicates a
relationship between them. What vertices and edges mean depends on how
the data structure is used. For instance, vertices may be cities and
edges are the roads that connect them. Or, edges may represent direct
network connections between two routers or two computers. Since the
graph is undirected, the order of the vertices in the edge does not
matter: the relationship is mutual. For graphs where the edge direction
does matter, use the Digraph class.

Vertices and edges provide the structure of the graph, but most of the
interesting domain-specific information of graphs is actually stored on
the vertices and edges of the graph. For this reason, one can create
properties via the add_vertex_property and add_edge_property methods
to represent, for instance, the IP address of a vertex that represents
a router or the length of a road connecting two cities.

Complete C++ documentation for the adjacency list representation:
  http://www.boost.org/libs/graph/doc/adjacency_list.html
"""

Digraph.__doc__ = """
A directed graph data structure.

A directed graph (also called a network) is a data structure that
contains a set of vertices and relationships between pairs of
vertices, called edges. Given two vertices u and v, an edge (u, v)
indicates a relationship between them. What vertices and edges mean
depends on how the data structure is used. For instance, vertices may
be tasks and edges might mean that the source of the edge cannot be
completed until the target task has been completed. The ordering of
vertices in an edge is important For graphs where the edge direction
does not matter, use the Graph class.

Vertices and edges provide the structure of the graph, but most of the
interesting domain-specific information of graphs is actually stored on
the vertices and edges of the graph. For this reason, one can create
properties via the add_vertex_property and add_edge_property methods
to represent, for instance, the functionality of a vertex that represents
a task or the method of transmission for a dependency between tasks.

Complete C++ documentation for the adjacency list representation:
  http://www.boost.org/libs/graph/doc/adjacency_list.html
"""

# Algorithm documentation
astar_search.__doc__ = _documentor.function('astar_search') \
    .parameter('graph', 
               'the graph to search. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the starting vertex for the search.') \
    .parameter('heuristic', 'an heuristic function that maps a vertex to a floating-point value that estimates the distance from the source to that vertex.') \
     .parameter('visitor', 'a visitor that will receive events as the search progresses. Typically this visitor should be derived from boost.graph.astar_visitor.', 'None') \
    .parameter('predecessor_map',
               'a vertex -> vertex map that stores the predecessor of each vertex in the search tree. From a given vertex, one can follow the predecessor_map back to the root_vertex to reconstruct the path taken.',
               'None') \
    .parameter('cost_map',
               'a vertex -> float map that stores the distance from the root_vertex to each vertex in the tree plus the estimated cost to reach the goal.',
               'None') \
    .parameter('distance_map',
               'a vertex -> float map that stores the distance from the root_vertex to each vertex in the tree.',
               'None') \
    .parameter('weight_map',
               'an edge -> float map that stores the weight of each edge in the graph. Negative edge weights are not permitted. If no weight map is provided, each edge will be assumed to have a weight of 1.0.',
               'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .paragraph('Searches a weight, directed or undirected graph using an heuristic function as guidance.') \
    .see_also('astar_visitor') \
    .see_also('bellman_ford_shortest_paths') \
    .see_also('dag_shortest_paths') \
    .see_also('dijkstra_shortest_paths') \
    .__str__()

bellman_ford_shortest_paths.__doc__ = _documentor.function('bellman_ford_shortest_paths') \
    .parameter('graph', 
               'the graph on which to compute shortest paths will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the starting vertex for the shortest-path search.') \
    .parameter('predecessor_map',
               'a vertex -> vertex map that stores the predecessor of each vertex in the shortest paths tree. From a given vertex, one can follow the predecessor_map back to the root_vertex to reconstruct the shortest path.',
               'None') \
    .parameter('distance_map',
               'a vertex -> float map that stores the distance from the root_vertex to each vertex in the tree. A distance of infinity in this property map indicates that the vertex is unreachable from the root_vertex.',
               'None') \
    .parameter('weight_map',
               'an edge -> float map that stores the weight of each edge in the graph. Negative edge weights are permitted. If no weight map is provided, each edge will be assumed to have a weight of 1.0.',
               'None') \
    .parameter('visitor', 'a visitor that will receive events as the shortest-paths computation progresses. Typically this visitor should be derived from boost.graph.bellman_ford_visitor.', 'None') \
    .result('bool') \
    .paragraph('Computes the shortest paths from the root vertex to every other vertex in the graph. Edge weights may be either positive or negative, but if a negative weight cycle is found the algorithm will return False to indicate failure. Otherwise, the algorithm returns True on success.') \
    .see_also('bellman_ford_visitor') \
    .see_also('dag_shortest_paths') \
    .see_also('dijkstra_shortest_paths') \
    .cpp_docs("bellman_ford_shortest.html") \
    .__str__()

betweenness_centrality_clustering.__doc__ = _documentor.function('betweenness_centrality_clustering') \
    .parameter('graph', 
               'the graph that will be clustered.') \
    .parameter('done',
               'A Python function (or callable object) that determines when betweenness centrality clustering should terminate. It receives three parameters during each iteration: the maximum centrality, the edge descriptor that has that centrality, and the graph itself, and should return True when clustering is complete.') \
    .parameter('edge_centrality_map',
               'an edge -> float map that stores the centrality of each edge in the graph.',
               'None') \
    .paragraph('Clusters the graph by repeatedly removing the edge with the largest betweenness centrality until a certain stopping criteria is reached. This is a rather inefficient clustering algorithm and should only be used for small graphs.') \
    .see_also('brandes_betweenness_centrality') \
    .cpp_docs('bc_clustering.html') \
    .__str__()

biconnected_components.__doc__ = _documentor.function('biconnected_components') \
    .parameter('graph', 
               'the graph on which to compute biconnected components. The graph must be undirected.') \
    .parameter('component_map',
               'an edge -> int map that stores the component number for each edge in the graph. Edges with the same component number are in the same biconnected component.',
               'None') \
    .parameter('edge_hidden_map',
               'an edge -> int map that can be used to hide certain edges from the algorithm. When provided, each edge with a non-zero value will be filtered out and ignored.',
               'None') \
    .result('list') \
    .paragraph('Computes the biconnected components and articulation points in an undirected graph, assigning component numbers in the range [0, n) (where n is the number of biconnected components) to the edges in the graph. The set of articulation points, i.e., those vertices that separate biconnected components, will be returned.') \
    .see_also('connected_components') \
    .__str__()

brandes_betweenness_centrality.__doc__ = _documentor.function('brandes_betweenness_centrality') \
    .parameter('graph', 'the graph on which centrality should be computed.') \
    .parameter('vertex_centrality_map',
               'a vertex -> float map that stores the centrality of each vertex in the graph.',
               'None') \
    .parameter('edge_centrality_map',
               'an edge -> float map that stores the centrality of each edge in the graph.',
               'None') \
    .parameter('weight_map',
               'an edge -> float map that stores the weight of each edge in the graph. If no weight map is provided, each edge will be assumed to have a weight of 1.0.',
               'None') \
    .paragraph('Computes the betweenness centrality of the vertices and/or edges in the graph. The betweenness centrality of a vertex or edge is the ratio of shortest paths in the graph that pass through the vertex or edge.') \
    .see_also('betweenness_centrality_clustering') \
    .see_also('central_point_dominance') \
    .see_also('relative_betweenness_centrality') \
    .cpp_docs('betweenness_centrality.html') \
    .__str__()

breadth_first_visit.__doc__ = _documentor.function('breadth_first_visit') \
    .parameter('graph', 
               'the graph on which the breadth-first search will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the vertex where the breadth-first search will originate.') \
    .parameter('buffer', 'the queue used by the breadth-first search. If not supplied, a simple FIFO queue will be used.', 'None') \
    .parameter('visitor', 'a visitor that will receive events as the breadth-first search progresses. Typically this visitor should be derived from boost.graph.bfs_visitor.', 'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .paragraph('Performs a breadth-first search on the given graph starting at a particular vertex. Unlike breadth_first_search, the color map will NOT be reinitialized to all-white values before running the breadth-first search.') \
    .see_also('bfs_visitor') \
    .see_also('breadth_first_search') \
    .see_also('depth_first_search') \
    .__str__()

breadth_first_search.__doc__ = _documentor.function('breadth_first_search') \
    .parameter('graph', 
               'the graph on which the breadth-first search will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the vertex where the breadth-first search will originate.') \
    .parameter('buffer', 'the queue used by the breadth-first search. If not supplied, a simple FIFO queue will be used.', 'None') \
    .parameter('visitor', 'a visitor that will receive events as the breadth-first search progresses. Typically this visitor should be derived from boost.graph.bfs_visitor.', 'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .paragraph('Performs a breadth-first search on the given graph starting at a particular vertex. The color map will be initialized to contain all white values before the the breadth-first search runs.') \
    .see_also('bfs_visitor') \
    .see_also('breadth_first_visit') \
    .see_also('depth_first_search') \
    .__str__()

central_point_dominance.__doc__ = _documentor.function('central_point_dominance') \
    .parameter('graph', 'the graph on which the relative vertex centrality has been computed.') \
    .parameter('vertex_centrality_map',
               'a vertex -> float map that stores the relative centrality of each vertex in the graph.',
               'None') \
    .result('float') \
    .paragraph('Computes the central point dominance of a graph based on the relative betweeness centrality of the vertices. Returns a value between 0 and 1, where larger numbers indicate more star-like graphs with a single central node and 0 indicates a very decentralized graph.') \
    .see_also('betweenness_centrality_clustering') \
    .see_also('brandes_betweenness_centrality') \
    .see_also('relative_betweenness_centrality') \
    .cpp_docs('betweenness_centrality.html') \
    .__str__()

circle_graph_layout.__doc__ = _documentor.function('circle_graph_layout') \
    .parameter('graph', 'the graph on which to perform the layout.') \
    .parameter('position_map',
               'a vertex -> Point2D map that stores the position of each vertex.') \
    .parameter('radius', 'the radius of the circle.', '0.5') \
    .paragraph('Lays out the vertices of the graph along a circle of given radius.') \
    .see_also('fruchterman_reingold_force_directed_layout') \
    .see_also('kamada_kawai_spring_layout') \
    .__str__()
  
connected_components.__doc__ = _documentor.function('connected_components') \
    .parameter('graph', 
               'the graph on which to compute connected components. The graph must be undirected.') \
    .parameter('component_map',
               'a vertex -> int map that stores the component number for each vertex in the graph. Vertices with the same component number are in the same connected component.',
               'None') \
    .result('int') \
    .paragraph('Computes the connected components in an undirected graph, assigning component numbers in the range [0, n) (where n is the number of connected components) to the vertices in the graph. Returns the number of connected components.') \
    .see_also('biconnected_components') \
    .__str__()

cuthill_mckee_ordering.__doc__ = _documentor.function('cuthill_mckee_ordering') \
    .parameter('graph', 
               'the graph to be ordered.') \
    .parameter('comp', 
               'the comparison function that determines the order the neighbors are visited.', 'None') \
    .result('list') \
    .paragraph('Computes a new ordering for the vertices in the graph that reduces the bandwidth in a sparse matrix.') \
    .see_also('king_ordering') \
    .see_also('minimum_degree_ordering') \
    .see_also('sloan_ordering') \
    .__str__()

dag_shortest_paths.__doc__ = _documentor.function('dag_shortest_paths') \
    .parameter('graph', 
               'the graph on which to compute shortest paths will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the starting vertex for the shortest-path search.') \
    .parameter('predecessor_map',
               'a vertex -> vertex map that stores the predecessor of each vertex in the shortest paths tree. From a given vertex, one can follow the predecessor_map back to the root_vertex to reconstruct the shortest path.',
               'None') \
    .parameter('distance_map',
               'a vertex -> float map that stores the distance from the root_vertex to each vertex in the tree. A distance of infinity in this property map indicates that the vertex is unreachable from the root_vertex.',
               'None') \
    .parameter('weight_map',
               'an edge -> float map that stores the weight of each edge in the graph. Negative edge weights are permitted. If no weight map is provided, each edge will be assumed to have a weight of 1.0.',
               'None') \
    .parameter('visitor', 'a visitor that will receive events as the shortest-paths computation progresses. Typically this visitor should be derived from boost.graph.dijkstra_visitor.', 'None') \
    .paragraph('Computes the shortest paths from the root vertex to every other vertex in a directed acyclic graph.') \
    .see_also('bellman_ford_shortest_paths') \
    .see_also('dijkstra_visitor') \
    .see_also('dijkstra_shortest_paths') \
    .__str__()

depth_first_search.__doc__ = _documentor.function('depth_first_search') \
    .parameter('graph', 
               'the graph on which the depth-first search will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the vertex where the depth-first search will originate. If none is provided, the depth-first search will cover all vertices in the graph', 'None') \
    .parameter('visitor', 'a visitor that will receive events as the depth-first search progresses. Typically this visitor should be derived from boost.graph.dfs_visitor.', 'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .paragraph('Performs a depth-first search on the given graph, optionally starting at a particular vertex.') \
    .see_also('breadth_first_search') \
    .see_also('dfs_visitor') \
    .see_also('depth_first_visit') \
    .see_also('undirected_dfs') \
    .__str__()

depth_first_visit.__doc__ = _documentor.function('depth_first_visit') \
    .parameter('graph', 
               'the graph on which the depth-first search will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the vertex where the depth-first search will originate.') \
    .parameter('visitor', 'a visitor that will receive events as the depth-first search progresses. Typically this visitor should be derived from boost.graph.dfs_visitor.', 'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .paragraph('Performs a depth-first search on the given graph starting at a particular vertex. Unless depth_first_search, the root_vertex parameter is mandatory and the color_map, if provided, will NOT be reinitialized.') \
    .see_also('breadth_first_search') \
    .see_also('dfs_visitor') \
    .see_also('depth_first_search') \
    .__str__()

dijkstra_shortest_paths.__doc__ = _documentor.function('dijkstra_shortest_paths') \
    .parameter('graph', 
               'the graph on which to compute shortest paths will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the starting vertex for the shortest-path search.') \
    .parameter('predecessor_map',
               'a vertex -> vertex map that stores the predecessor of each vertex in the shortest paths tree. From a given vertex, one can follow the predecessor_map back to the root_vertex to reconstruct the shortest path.',
               'None') \
    .parameter('distance_map',
               'a vertex -> float map that stores the distance from the root_vertex to each vertex in the tree. A distance of infinity in this property map indicates that the vertex is unreachable from the root_vertex.',
               'None') \
    .parameter('weight_map',
               'an edge -> float map that stores the weight of each edge in the graph. Negative edge weights are not permitted. If no weight map is provided, each edge will be assumed to have a weight of 1.0.',
               'None') \
    .parameter('visitor', 'a visitor that will receive events as the shortest-paths computation progresses. Typically this visitor should be derived from boost.graph.dijkstra_visitor.', 'None') \
    .paragraph('Computes the shortest paths from the root vertex to every other vertex in a graph.') \
    .see_also('bellman_ford_shortest_paths') \
    .see_also('dag_shortest_paths') \
    .see_also('dijkstra_visitor') \
    .__str__()

fruchterman_reingold_force_directed_layout.__doc__ = _documentor.function('fruchterman_reingold_force_directed_layout') \
    .parameter('graph', 'the graph on which to perform the layout.') \
    .parameter('position',
               'a vertex -> Point2D map or vertex -> Point3D that stores the position of each vertex.') \
    .parameter('origin', 
               'the origin of the bounding box in which layout will occur', 
               'Point2D()/Point3D()') \
    .parameter('extent', 
               'the extent (size) of the bounding box in which layout will occur', 
               'Point2D(500, 500)/Point3D(500, 500, 500)') \
    .parameter('attractive_force',
               'A Python function (or callable object) that computes the attractive force along an edge. The function should accept four parameters: the edge descriptor, the parameter k = sqrt(bounding box volume/graph.num_vertices()), the distance between the two endpoints of the edge, and the graph g. If no attractive force is provided, the default of dist*dist/k will be used.',
               'None') \
    .parameter('repulsive_force',
               'A Python function (or callable object) that computes the repulsive force between two vertices. The function should accept five parameters: the two vertex descriptors, followed by the parameter k = sqrt(bounding box volume/graph.num_vertices()), the distance between the two endpoints of the edge, and the graph g. If no repulsive force is provided, the default of k*k/dist will be used.',
               'None') \
    .parameter('force_pairs',
               'A Python function (or callable object) that enumerates the pairs of vertices that should repulse each other. This function must accept two arguments: the graph and the apply_force function. It will traverse the graph and, for each pair of vertices (u, v) that should repulse each other, execute apply_force(u, v). If no force_pairs function is provided, the algorithm will use a 2k x 2k grid to provide a reasonably performance and accuracy tradeoff.',
               'None') \
    .parameter('cooling',
               'A Python function (or callable object) that provides the maximm displacement (i.e., the "temperature") for any vertex in each iteration, and terminates the algorithm by returning a temperature of 0.',
               'None') \
    .parameter('progressive',
               'when False, vertex positions are randomized before the algorithm is executed. Otherwise, the positions stored in the position property map are used as the starting positions.',
               'False') \
    .paragraph('Performs Fruchterman-Reingold force-directed layout in either two or three dimensions. This is a good general-purpose graph layout routine that handles both directed and undirected, connected and disconnected graphs.') \
    .see_also('circle_graph_layout') \
    .see_also('kamada_kawai_spring_layout') \
    .cpp_docs('fruchterman_reingold.html') \
    .__str__()

isomorphism.__doc__ = _documentor.function('isomorphism') \
    .parameter('graph1', 'the first graph') \
    .parameter('graph2', 'the second graph') \
    .parameter('isomorphism_map',
               'a vertex -> vertex map that will store the mapping from vertices in graph1 to vertices in graph2, if the graphs are indeed isomorphic.',
               'None') \
    .parameter('vertex_invariant', 
               'A Python function (or callable object) that produces a hash value given a vertex and its owning graph. This function must work for both vertices in graph1 and graph2, such that isomorphic vertices have the same hash value. If no vertex_invariant is provided, the degree of vertices will be used to aid matching.',
               'None') \
    .result('bool') \
    .paragraph('Determines if the two input graphs are isomorphic. If so, returns True and fills in the isomorphism_map (if provided). Otherwise, it returns false.') \
    .__str__()

kamada_kawai_spring_layout.__doc__ = _documentor.function('kamada_kawai_spring_layout') \
    .parameter('graph', 'the graph on which to perform the layout, which must be undirected.') \
    .parameter('position',
               'a vertex -> Point2D map that stores the position of each vertex.') \
    .parameter('weight',
               'an edge -> float map that stores the weight of each edge in the graph. If no weight map is provided, each edge will be assumed to have a weight of 1.0.',
               'None') \
    .parameter('side_length', 
               'the length of a side in the 2D bounding box in which layout will occur',
               '500.0') \
    .parameter('done',
               'A Python function (or callable object) that determines when the local and global phases of the algorithm will terminate. The function must take four arguments: delta_p, a measure of how far the point "p" has moved; "p" the vertex that has been moved most recently; the graph; and a boolean value that indicates whether the movement is local (True) or global (False). If omitted, the algorithm will terminate will it reaches a local minimum.',
               'None') \
    .parameter('spring_constant', 
               "A constant multiplied by each spring's strength. Larger values create more pleasing layouts (but take longer), smaller values produce layouts more rapidly (but they may not look as good).",
               '1.0') \
    .parameter('progressive',
               'when False, vertex positions are randomized before the algorithm is executed. Otherwise, the positions stored in the position property map are used as the starting positions.',
               'False') \
    .paragraph('Performs Kamada-Kawai spring embedding layout in two dimensions on an undirected, connected, possibly weighted graph. While this algorithm is considerably more expensive and limited than Fruchterman-Reingold, it tends to produce much better layouts.') \
    .see_also('circle_graph_layout') \
    .see_also('fruchterman_reingold_force_directed_layout') \
    .__str__()

king_ordering.__doc__ = _documentor.function('king_ordering') \
    .parameter('graph', 
               'the graph to be ordered.') \
    .result('list') \
    .paragraph('Computes a new ordering for the vertices in the graph that reduces the bandwidth of a sparse matrix.') \
    .see_also('cuthill_mckee_ordering') \
    .see_also('minimum_degree_ordering') \
    .see_also('sloan_ordering') \
    .__str__()

kruskal_minimum_spanning_tree.__doc__ = _documentor.function('kruskal_minimum_spanning_tree') \
    .parameter('graph', 'the graph whose spanning tree will be computed.') \
    .parameter('weight_map',
               'an edge -> float map that stores the weight of each edge in the graph.') \
    .result('list') \
    .paragraph('Computes a minimum spanning forest for the given undirected graph. The edges in the spanning forest will be returned as a list.') \
    .see_also('prim_minimum_spanning_tree') \
    .cpp_docs('kruskal_min_spanning_tree.html') \
    .__str__()

minimum_degree_ordering.__doc__ = _documentor.function('minimum_degree_ordering') \
    .parameter('graph', 'the graph to be ordered.') \
    .parameter('supernode_size', 
               'a vertex -> int that provides the supernode size of each vertex. If omitted, the supernode size of each vertex will be initialized to 1.', 'None') \
    .parameter('delta', 'the minimum difference between the minimum degree and the degree of vertices that will be eliminated by multiple elimination', '0') \
    .result('list') \
    .paragraph('Computes a new ordering for the vertices in the graph that reduces the fill-in of sparse matrices.') \
    .see_also('cuthill_mckee_ordering') \
    .see_also('king_ordering') \
    .see_also('sloan_ordering') \
    .__str__()

page_rank.__doc__ = _documentor.function('page_rank') \
    .parameter('graph', 'the directed graph whose vertices will be ranked.') \
    .parameter('rank_map', 
               'a vertex -> float that will contain the ranks of each vertex in the graph') \
    .parameter('done_or_iterations', 'this can either be an integer number of iterations to run or a Python function (or callable object) that accepts the rank map and the graph as arguments and returns True when the algorithm should terminate', '20') \
    .paragraph('Computes the PageRank of each vertex in a directed graph. For optimal results, the graph should be strongly connected.') \
    .__str__()

prim_minimum_spanning_tree.__doc__ = _documentor.function('prim_minimum_spanning_tree') \
    .parameter('graph', 
               'the graph for which the minimum spanning tree will be computed.') \
    .parameter('predecessor_map',
               'a vertex -> vertex map that stores the predecessor of each vertex in the minimum spanning tree. From a given vertex, one can follow the predecessor_map back to the root_vertex to reconstruct the shortest path.',
               'None') \
    .parameter('distance_map',
               'a vertex -> float map that stores the distance from the root_vertex to each vertex in the tree. A distance of infinity in this property map indicates that the vertex is unreachable from the root_vertex.',
               'None') \
    .parameter('weight_map',
               'an edge -> float map that stores the weight of each edge in the graph. Negative edge weights are not permitted. If no weight map is provided, each edge will be assumed to have a weight of 1.0.',
               'None') \
    .parameter('visitor', 'a visitor that will receive events as the shortest-paths computation progresses. Typically this visitor should be derived from boost.graph.dijkstra_visitor.', 'None') \
    .parameter('root_vertex', 
               'the starting vertex for the minimum spanning tree. If omitted a minimum spanning forest will be computed.', 
               'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .paragraph('Computes the minimum spanning tree from the root vertex, if provided; otherwise, computes a minimum spanning forest.') \
    .see_also('dfs_visitor') \
    .see_also('kruskal_minimum_spanning_tree') \
    .__str__()

relative_betweenness_centrality.__doc__ = _documentor.function('relative_betweenness_centrality') \
    .parameter('graph', 'the graph on which the vertex centrality has been computed.') \
    .parameter('vertex_centrality_map',
               'a vertex -> float map that stores the relative centrality of each vertex in the graph.',
               'None') \
    .paragraph('Transforms the raw vertex betweenness centrality produced by brandes_betweenness_centrality into a relative betweenness centrality that can be used by central_point_dominance or for comparison with other graphs.') \
    .see_also('betweenness_centrality_clustering') \
    .see_also('brandes_betweenness_centrality') \
    .see_also('central_point_dominance') \
    .cpp_docs('betweenness_centrality.html') \
    .__str__()

sequential_vertex_coloring.__doc__ = _documentor.function('sequential_vertex_coloring') \
    .parameter('graph', 'the graph whose vertices will be colored') \
    .parameter('color_map', 
               'a vertex -> int map that will contain the color of each vertex',
               'None') \
    .result('int') \
    .paragraph('Colors the vertices of the graph using a simple but relatively efficient algorithm. Returns the number "n" of colors used, and the color_map will contain colors for each vertex in the range [0, n).') \
    .__str__()

sloan_ordering.__doc__ = _documentor.function('sloan_ordering') \
    .parameter('graph', 'the graph to be ordered.') \
    .parameter('start', 'the vertex from which ordering should start') \
    .parameter('end', 'the vertex at which ordering should end') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .parameter('priority_map', 
               'a vertex -> float map used internally that tracks the priority of each vertex.', 'None') \
    .parameter('weight1', 'Relative weight applied to the global "degree".', '1.0') \
    .parameter('weight2', 'Relative weight applied to the global "degree".', '2.0') \
    .signature(('graph', 'start', 'end', 'color_map', 'priority_map', 
                'weight1', 'weight2'),
               'list') \
    .signature(('graph', 'color_map', 'priority_map', 'weight1', 'weight2'),
               'list') \
    .result('list') \
    .paragraph('Computes a new ordering for the vertices in the graph that reduces the profile and wavefront of sparse matrices. The start and end vertices are optional, but providing them can have a large (positive or negative) effect on the resulting ordering.') \
    .see_also('cuthill_mckee_ordering') \
    .see_also('king_ordering') \
    .see_also('minimum_degree_ordering') \
    .__str__()


strong_components.__doc__ = _documentor.function('strong_components') \
    .parameter('graph', 
               'the graph on which to compute strongly connected components. The graph must be directed.') \
    .parameter('component_map',
               'a vertex -> int map that stores the component number for each vertex in the graph. Vertices with the same component number are in the same strongly connected component.',
               'None') \
    .result('int') \
    .paragraph('Computes the strongly connected components in a directed graph, assigning component numbers in the range [0, n) (where n is the number of strongly connected components) to the vertices in the graph. Returns the number of strongly connected components.') \
    .__str__()

topological_sort.__doc__ = _documentor.function('topological_sort') \
    .parameter('graph', 
               'the graph to be ordered.') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .result('list') \
    .paragraph('Computes a reverse topological ordering of the vertices in the graph. Returns the list of vertices in their new order.') \
    .__str__()

transitive_closure.__doc__ = _documentor.function('transitive_closure') \
    .parameter('graph', 
               'the directed graph from which the transition closure will be computed.') \
    .parameter('orig_to_copy', 'a vertex -> vertex map that maps from the vertices of the input graph to the vertices of the resulting graph.', 'None') \
    .result('Digraph') \
    .paragraph('Computes the transitive closure of a directed graph and returns the resulting graph. The input graph is not modified.') \
    .__str__()

undirected_dfs.__doc__ = _documentor.function('undirected_dfs') \
    .parameter('graph', 
               'the undirected graph on which the depth-first search will run.') \
    .parameter('visitor', 'a visitor that will receive events as the depth-first search progresses. Typically this visitor should be derived from boost.graph.dfs_visitor.', 'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .parameter('edge_color_map', 'an edge property map that stores the "color" of each edge, allowing the algorithm to distinguish between tree and back edges in an undirected graph', 'None') \
    .paragraph('Performs an undirected depth-first search on the given graph.') \
    .see_also('dfs_visitor') \
    .see_also('depth_first_search') \
    .see_also('depth_first_visit') \
    .__str__()

#spectrum.__doc__ = _documentor.function('spectrum') \
#    .parameter('graph', 'an undirected graph.') \
#    .parameter('first_eigenvector_index', 'the index of the lowest eigenvalue and eigenvector to return.') \
#    .parameter('num_eigenvectors', 'the number of eigenvalues and eigenvectors to return.') \
#    .parameter('rel_tol', 'the relative tolerance to use when computing the eigenvectors.') \
#    .parameter('abs_tol', 'the absolute tolerance to use when computing the eigenvectors.') \
#    .result('(eigenvalues, eigenvectors)') \
#    .paragraph('Finds the eigenvalues and eigenvectors of an undirected graph within the specified range. Returns a tuple of a list of the eigenvalues and a list of the eigenvectors. (Note that multiple edges are considered as single edges, and self-loops are ignored.) This function performs better than sparse_spectrum() on sufficiently small or dense graphs.') \
#    .see_also('sparse_spectrum') \
#    .__str__()

#sparse_spectrum.__doc__ = _documentor.function('spectrum') \
#    .parameter('graph', 'an undirected graph.') \
#    .parameter('first_eigenvector_index', 'the index of the lowest eigenvalue and eigenvector to return.') \
#    .parameter('num_eigenvectors', 'the number of eigenvalues and eigenvectors to return.') \
#    .parameter('rel_tol', 'the relative tolerance to use when computing the eigenvectors.') \
#    .parameter('abs_tol', 'the absolute tolerance to use when computing the eigenvectors.') \
#    .result('(eigenvalues, eigenvectors)') \
#    .paragraph('Finds the eigenvalues and eigenvectors of an undirected graph within the specified range. Returns a tuple of a list of the eigenvalues and a list of the eigenvectors. (Note that multiple edges are considered as single edges, and self-loops are ignored.) This function performs better than spectrum() on large, sparse graphs.') \
#    .see_also('sparse_spectrum') \
#    .__str__()

def read_graphviz(filename, node_id = 'node_id'):
    """
    Loads a graph (which may be either directed or undirected) written
    in the GraphViz DOT format from the file filename. Once loaded,
    all of the vertex and edge properties of the graph will be
    available in the vertex_properties and edge_properties of the
    graph. If you already know whether your graph will be undirected
    or directed, use Graph.read_graphviz or Digraph.read_graphviz,
    respectively.
    
    Parameters:
      filename  The name of the file to load.
      node_id   The name given to the property map that will store the
                identifier associated with each vertex in the DOT file.

    The GraphViz DOT language is described here:
      http://www.graphviz.org/doc/info/lang.html
      
    Complete C++ documentation is available at:
      http://www.boost.org/libs/graph/doc/read_graphviz.html
    """
    try:
        graph = Digraph.read_graphviz(filename, node_id)
    except UndirectedGraphError:
        graph = Graph.read_graphviz(filename, node_id)
    return graph

def read_graphml(filename):
    """
    Loads a graph (which may be either directed or undirected) written
    in the GraphML format from the file filename. Once loaded,
    all of the vertex and edge properties of the graph will be
    available in the vertex_properties and edge_properties of the
    graph. If you already know whether your graph will be undirected
    or directed, use Graph.read_graphml or Digraph.read_graphml,
    respectively.
    
    Parameters:
      filename  The name of the file to load.

    The GraphML format is described here:
      http://graphml.graphdrawing.org/
    """
    try:
        graph = Digraph.read_graphml(filename)
    except UndirectedGraphError:
        graph = Graph.read_graphml(filename)
    return graph
