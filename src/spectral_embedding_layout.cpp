#include <boost/graph/spectral_embedding_3d_layout.hpp>
#include "graph_types.hpp"
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/point3d.hpp>
#include <boost/graph/python/graph.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

template <typename Graph>
void 
spectral_embedding_layout
  (Graph& g,
   const vector_property_map<point3d, typename Graph::VertexIndexMap>* in_pos)
{
  using boost::python::object;

  typedef typename property_map<Graph, vertex_index_t>::const_type VertexIndexMap;
  typedef vector_property_map<point3d, VertexIndexMap> PositionMap;
  typedef vector_property_map<vertex_index_t, VertexIndexMap> IndexMap;

  PositionMap pos = *in_pos;

  boost::spectral_embedding_layout(g, pos);
}

void export_spectral_embedding_layout()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

  def("spectral_embedding_layout", 
      &spectral_embedding_layout<Graph>,
      (arg("graph"), 
       arg("position") = 
         (vector_property_map<point2d, Graph::VertexIndexMap>*)0));

}

} } } // end namespace boost::graph::python
