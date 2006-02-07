#include <boost/graph/spectrum.hpp>
#include "graph_types.hpp"
#include <boost/graph/python/graph.hpp>
#include <boost/python.hpp>
#include <vector>

namespace boost { namespace graph { namespace python {

template <typename Graph>
boost::python::tuple
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
  std::vector<Vector> evecs(num_eigenvectors);
  for (int i = 0; i < num_eigenvectors; i++)
    evecs[i] = *(new Vector(N));
  std::vector<double> evals(num_eigenvectors);

  boost::spectrum<Graph, std::vector<Vector> >(g, first_eigenvector_index, num_eigenvectors, evecs, evals, rel_tol, abs_tol);

  boost::python::list *evec;

  boost::python::list eigenvectors;
  for(int i = 0; i < num_eigenvectors; i++) {
    evec = new boost::python::list();
    for (int j = 0; j < N; j++)
      evec->append(evecs[i][j]);
    eigenvectors.append(*evec);
  }

  boost::python::list eigenvalues;
  for(int i = 0; i < num_eigenvectors; i++) {
      eigenvalues.append(evals[i]);
  }
  
  return boost::python::make_tuple(eigenvalues, eigenvectors);
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
