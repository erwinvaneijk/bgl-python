#include <boost/graph/spectrum.hpp>
#include "graph_types.hpp"
#include <boost/graph/python/graph.hpp>
#include <boost/python.hpp>
#include <vector>

namespace boost { namespace graph { namespace python {

template <typename Graph>
boost::python::list
spectrum
  (Graph& g,
   int first_eigenvector_index,
   int num_eigenvectors,
   double rel_tol = 100,
   double abs_tol = 1000)
{
  using std::vector;
  using boost::python::object;

  typedef typename property_map<Graph, vertex_index_t>::const_type VertexIndexMap;
  typedef vector_property_map<vertex_index_t, VertexIndexMap> IndexMap;
  typedef std::vector<double> Vector;

  int N = num_vertices(g);
  std::vector<Vector> evecs(3);
  for (int i = 0; i < num_eigenvectors; i++)
    evecs[i] = *(new Vector(N));

  boost::spectrum<Graph, std::vector<Vector> >(g, first_eigenvector_index, num_eigenvectors, evecs, rel_tol, abs_tol);

  boost::python::list *evec;

  boost::python::list result;
  for(int i = 0; i < num_eigenvectors; i++) {
    evec = new boost::python::list();
    for (int j = 0; j < N; j++)
      evec->append(evecs[i][j]);
    result.append(*evec);
  }

  return result;

}

void export_spectrum()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

  def("spectrum", 
      &spectrum<Graph>,
      (arg("graph"), 
       arg("first_eigenvector_index") = (int)1,
       arg("num_eigenvectors") = (int)1,
       arg("rel_tol") = (double)100,
       arg("abs_tol") = (double)1000)
      );

}

} } } // end namespace boost::graph::python
