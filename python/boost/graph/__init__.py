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
bellman_ford_shortest_paths.__doc__ = documentor.function('bellman_ford_shortest_paths') \
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
    .see_also('dijkstra_shortest_paths') \
    .cpp_docs("bellman_ford_shortest.html") \
    .__str__()

betweenness_centrality_clustering.__doc__ = documentor.function('betweenness_centrality_clustering') \
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

biconnected_components.__doc__ = documentor.function('biconnected_components') \
    .parameter('graph', 
               'the graph on which to compute biconnected components. The graph must be undirected.') \
    .parameter('component_map',
               'an edge -> int map that stores the component number for each edge in the graph. Edges with the same component number are in the same biconnected component.',
               'None') \
    .result('list') \
    .paragraph('Computes the biconnected components and articulation points in an undirected graph, assigning component numbers in the range [0, n) (where n is the number of biconnected components) to the edges in the graph. The set of articulation points, i.e., those vertices that separate biconnected components, will be returned.') \
    .see_also('connected_components') \
    .__str__()

brandes_betweenness_centrality.__doc__ = documentor.function('brandes_betweenness_centrality') \
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

breadth_first_search.__doc__ = documentor.function('breadth_first_search') \
    .parameter('graph', 
               'the graph on which the breadth-first search will run. It may be either a directed or undirected graph.') \
    .parameter('root_vertex', 'the vertex where the breadth-first search will originate.') \
    .parameter('buffer', 'the queue used by the breadth-first search. If not supplied, a simple FIFO queue will be used.', 'None') \
    .parameter('visitor', 'a visitor that will receive events as the breadth-first search progresses. Typically this visitor should be derived from boost.graph.bfs_visitor.', 'None') \
    .parameter('color_map', 'a vertex property map that stores the "color" of each vertex, which indicates whether is has not been seen (white), has been seen but not visited (grey), or has been visited (black).', 'None') \
    .paragraph('Performs a breadth-first search on the given graph starting at a particular vertex.') \
    .see_also('bfs_visitor') \
    .see_also('breadth_first_visit') \
    .see_also('depth_first_search') \
    .__str__()

central_point_dominance.__doc__ = documentor.function('central_point_dominance') \
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

circle_graph_layout.__doc__ = documentor.function('circle_graph_layout') \
    .parameter('graph', 'the graph on which to perform the layout.') \
    .parameter('position_map',
               'a vertex -> Point2D map that stores the position of each vertex.') \
    .parameter('radius', 'the radius of the circle.', '0.5') \
    .paragraph('Lays out the vertices of the graph along a circle of given radius.') \
    .see_also('fruchterman_reingold_force_directed_layout') \
    .see_also('kamada_kawai_spring_layout') \
    .__str__()
  
connected_components.__doc__ = documentor.function('connected_components') \
    .parameter('graph', 
               'the graph on which to compute connected components. The graph must be undirected.') \
    .parameter('component_map',
               'a vertex -> int map that stores the component number for each vertex in the graph. Vertices with the same component number are in the same connected component.',
               'None') \
    .result('int') \
    .paragraph('Computes the connected components in an undirected graph, assigning component numbers in the range [0, n) (where n is the number of connected components) to the vertices in the graph. Returns the number of connected components.') \
    .see_also('biconnected_components') \
    .__str__()

fruchterman_reingold_force_directed_layout.__doc__ = documentor.function('fruchterman_reingold_force_directed_layout') \
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

isomorphism.__doc__ = documentor.function('isomorphism') \
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

relative_betweenness_centrality.__doc__ = documentor.function('relative_betweenness_centrality') \
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

strong_components.__doc__ = documentor.function('strong_components') \
    .parameter('graph', 
               'the graph on which to compute strongly connected components. The graph must be directed.') \
    .parameter('component_map',
               'a vertex -> int map that stores the component number for each vertex in the graph. Vertices with the same component number are in the same strongly connected component.',
               'None') \
    .result('int') \
    .paragraph('Computes the strongly connected components in a directed graph, assigning component numbers in the range [0, n) (where n is the number of strongly connected components) to the vertices in the graph. Returns the number of strongly connected components.') \
    .__str__()
