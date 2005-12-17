// Ben Martin
// October 11, 2005

// This currently works only on undirected graphs...
// Graph must model Adjacency Graph, Incidence Graph, VertexListGraph

#ifndef BOOST_GRAPH_SPECTRAL_EMBEDDING_LAYOUT_HPP
#define BOOST_GRAPH_SPECTRAL_EMBEDDING_LAYOUT_HPP

//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/named_function_params.hpp>
//#include <boost/graph/properties.hpp>
//#include <boost/vector_property_map.hpp>

#include <boost/graph/spectrum.hpp>
#include <utility> // for pair
#include <vector>

namespace boost {

  template <typename Graph, typename PositionMap>
  void spectral_embedding_layout(Graph& g, PositionMap pos_map, double rel_tol = 100, double abs_tol = 1000) {
    
    using std::vector;
    using std::pair;

    // typedef std::vector<double> Vector;
    // typedef std::vector<std::vector<double> > Matrix;
    typedef typename Graph::vertex_iterator VertexIterator;

    int N = num_vertices(g);
    //    std::vector<Vector> evecs(3);
    std::vector<std::vector<double> > evecs(3);
    evecs[0] = *(new std::vector<double>(N));
    evecs[1] = *(new std::vector<double>(N));
    evecs[2] = *(new std::vector<double>(N));
    //    spectrum<Graph, Vector>(g, 1, 3, evecs, rel_tol, abs_tol);
    //    spectrum<Graph, Matrix >(g, 1, 3, &evecs, rel_tol, abs_tol);
    spectrum(g, 1, 3, evecs, rel_tol, abs_tol);

    int i;
    VertexIterator v, vs, ve;
    std::pair<VertexIterator, VertexIterator> p;
    p = vertices(g);
    vs = p.first;
    ve = p.second;

    i = 0;
    for (v = vs; v != ve; ++v)
    {
      pos_map[*v][0] = evecs[0][i];
      i++;
    }
    i = 0;
    for (v = vs; v != ve; ++v)
    {
      pos_map[*v][1] = evecs[1][i];
      i++;
    }
    i = 0;
    for (v = vs; v != ve; ++v)
    {
      pos_map[*v][2] = evecs[2][i];
      i++;
    }

  } // end spectral_embedding_layout()

} // end namespace boost

#endif // BOOST_GRAPH_SPECTRAL_EMBEDDING_LAYOUT_HPP

