#include <boost/graph/spectral_embedding_3d_layout.hpp>
#include "graph_types.hpp"
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/point3d.hpp>
#include <boost/graph/python/graph.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

template <typename Graph>
void 
spectral_embedding_3d_layout
  (Graph& g,
   const vector_property_map<point3d, typename Graph::VertexIndexMap>* in_pos,
   double rel_tol = 1000,
   double abs_tol = 10000)
{
  using boost::python::object;

  typedef typename property_map<Graph, vertex_index_t>::const_type VertexIndexMap;
  typedef vector_property_map<point3d, VertexIndexMap> PositionMap;
  typedef vector_property_map<vertex_index_t, VertexIndexMap> IndexMap;

  PositionMap pos = *in_pos;

  boost::spectral_embedding_layout(g, pos);
}

void export_spectral_embedding_3d_layout()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

  def("spectral_embedding_3d_layout", 
      &spectral_embedding_3d_layout<Graph>,
      (arg("graph"), 
       arg("position") = 
       (vector_property_map<point3d, Graph::VertexIndexMap>*)0,
       arg("rel_tol") = (double)1000,
       arg("abs_tol") = (double)10000)
      );

}

} } } // end namespace boost::graph::python
