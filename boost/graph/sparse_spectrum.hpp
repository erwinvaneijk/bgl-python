// Ben Martin
// January 31, 2006

// This currently works only on undirected graphs...
// Graph must model Adjacency Graph, Incidence Graph, VertexListGraph

#ifndef BOOST_GRAPH_SPARSE_SPECTRUM_HPP
#define BOOST_GRAPH_SPARSE_SPECTRUM_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/properties.hpp>
#include <boost/vector_property_map.hpp>
#include <utility> // for pair

//#include <iostream.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <ietl/vectorspace.h>
#include <ietl/lanczos.h>
#include <ietl/iteration.h>
#include <boost/random/linear_congruential.hpp>
#include <ietl/interface/ublas.h>

typedef long int integer;
typedef double doublereal;

namespace boost {

  template <typename Graph, typename EigenvectorMatrix >
  void sparse_spectrum(Graph& g, 
		       int first_eigenvector_index,
		       int num_eigenvectors,
		       EigenvectorMatrix &eigenvectors,
		       double rel_tol = 100, 
		       double abs_tol = 1000) 
  {
    std::vector<double> evals(num_eigenvectors);
    sparse_spectrum(g, first_eigenvector_index, num_eigenvectors, eigenvectors, evals, rel_tol, abs_tol);
  }

  // Parameters:
  //   first_eigenvector_index:
  //     Since the smallest eigenvector is not useful, often this will 
  //       be set to 1, for the "Fiedler vector," though if all 
  //       eigenvectors are desired, 0 may be a more logical value.
  //     Negative values are interpreted as allowing a default choice of 1.
  //   num_eigenvectors:
  //     The number of eigencectors to return.

  template <typename Graph, typename EigenvectorMatrix, typename EVector >
  void sparse_spectrum(Graph& g, 
		       int first_eigenvector_index,
		       int num_eigenvectors,
		       EigenvectorMatrix &eigenvectors,
		       EVector &eigenalues,
		       double rel_tol = 100, 
		       double abs_tol = 1000) 
  {

    //    Matrix eigenvectors = *(in_eigenvectors);

    if (first_eigenvector_index < 0)
      first_eigenvector_index = 1;
    
    typedef typename property_map<Graph, vertex_index_t>::const_type IndexMap;
    typedef typename Graph::vertex_iterator VertexIterator;
    typedef typename Graph::edge_iterator EdgeIterator;
    typedef typename boost::graph_traits<Graph>::adjacency_iterator AdjacencyIterator;
    
    IndexMap index_map = get(vertex_index, g);

    VertexIterator v, vs, ve;
    using std::pair;
    std::pair<VertexIterator, VertexIterator> p;
    p = vertices(g);
    vs = p.first;
    ve = p.second;

    EdgeIterator e, es, ee;
    using std::pair;
    std::pair<EdgeIterator, EdgeIterator> ep;
    ep = edges(g);
    es = ep.first;
    ee = ep.second;
    typename Graph::vertex_descriptor src, tgt;
    
    integer N = num_vertices(g);

    typedef boost::numeric::ublas::compressed_matrix<double> Matrix;
    Matrix A(N, N);
    
    int i;
    
    AdjacencyIterator a, as, ae;
    std::pair<AdjacencyIterator, AdjacencyIterator> ap;
    
    /*
    for (v = vs; v != ve; ++v) {
      A(index_map[*v], index_map[*v]) = (double)out_degree(*v, g);
    }
    */
    for (e = es; e != ee; ++e) {
      src = source(*e, g);
      tgt = target(*e, g);
      if (src != tgt && A(index_map[src], index_map[tgt]) != -1) {
	A(index_map[src], index_map[tgt]) = (double)(-1);
	A(index_map[tgt], index_map[src]) = (double)(-1);
	A(index_map[src], index_map[src]) += (double)1;
	A(index_map[tgt], index_map[tgt]) += (double)1;
      }
    }

    /*
    cout << "index_map = " << endl;
    for (v = vs; v != ve; ++v) {
      cout << index_map[*v] << " ";
    }
    cout << endl;
    cout << "A = " << endl;
    for (i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        cout << A[i*N + j] << " ";
      }
        cout << endl;
    }
    */

    using namespace ietl;

    vectorspace<boost::numeric::ublas::vector<double> > VS(N);
    lanczos<Matrix, vectorspace<boost::numeric::ublas::vector<double> > > LanczosObject(A, VS);
    
    double _rel_tol = 1000.*std::numeric_limits<double>::epsilon();
    double _abs_tol = 10000.*std::numeric_limits<double>::epsilon();
    
    //    _rel_tol = 100.*std::numeric_limits<double>::epsilon();
    //    _abs_tol = 100.*std::numeric_limits<double>::epsilon();

    // SO far these are the best I have found:
    //    _rel_tol = 1000.*std::numeric_limits<double>::epsilon();
    //    _abs_tol = 10000.*std::numeric_limits<double>::epsilon();

    _rel_tol = rel_tol*std::numeric_limits<double>::epsilon();
    _abs_tol = abs_tol*std::numeric_limits<double>::epsilon();


    lanczos_iteration_nlowest<double> LanczosIterationControl(1000000, first_eigenvector_index + num_eigenvectors, _rel_tol, _abs_tol);
    //    std::cout << std::numeric_limits<double>::epsilon() << std::endl;
    boost::minstd_rand gen(1);
    //boost::rand48 gen(1);
    LanczosObject.calculate_eigenvalues(LanczosIterationControl, gen);
    std::vector<double> evals = LanczosObject.eigenvalues();

    //    for (int i = 0; i < N; i++)
    //      std::cout << evals[i] << " ";
    //    std::cout << std::endl;

    /*
    std::vector<int> multiplicities = LanczosObject.multiplicities();
    int lowest_eval_multiplicity = multiplicities[1];
    int second_eval_multiplicity;
    if (lowest_eval_multiplicity == 1 && multiplicities[2] > 1)
      second_eval_multiplicity = 2;
    else
      second_eval_multiplicity = 1;

    for (int i = 0; i < N; i++)
      std::cout << multiplicities[i] << " ";
    std::cout << std::endl;
    */

    std::vector<boost::numeric::ublas::vector<double> > evecs(num_eigenvectors);
    for (int j = 0; j < num_eigenvectors; j++)
      evecs[j] = *(new boost::numeric::ublas::vector<double>(N));
    std::vector<boost::numeric::ublas::vector<double> >::iterator out_it = evecs.begin();
    
    Info<double> info;
    std::vector<double>::iterator ebegin, eend;
    ebegin = evals.begin();
    ebegin += first_eigenvector_index;
    eend = evals.begin();
    //    eend += 4 + (1 - lowest_eval_multiplicity) + (1 - second_eval_multiplicity);
    eend += first_eigenvector_index + num_eigenvectors;
    LanczosObject.eigenvectors(ebegin, eend, out_it, info, gen, 100000);
    //    LanczosObject.eigenvectors(ebegin, eend, out_it, info, gen);

    /*
    // If eigenvalues are repeated, we need to copy eigenvectors
    if (lowest_eval_multiplicity > 1) {
      for (int i = 0; i < N; i++)
	evecs[1][i] = evecs[0][i];
      if (lowest_eval_multiplicity > 2) {
	for(int i = 0; i < N; i++)
	  evecs[2][i] = evecs[0][i];
      }
    }
    if (second_eval_multiplicity > 1) {
      for (int i = 0; i < N; i++)
	evecs[2][i] = evecs[1][i];
    }
    */

    //    for (int i = 0; i < 3; i++)
    //      for (int j = 0; j < N; j++)
    //	std::cout << evecs[i][j] << " ";
    //    cout << std::endl;
    
    //    std::cout << A << std::endl;
    //    std::cout << LanczosIterationControl.error_code() << endl;;
    //    std::cout << info.error_info(1) << " " << info.error_info(2) << " " << info.error_info(3) << std::endl;

    //    std::vector<Vector> retval(num_eigenvectors);
    for (int j = 0; j < num_eigenvectors; j++) {
      // retval[j] = *(new Vector(N));
      i = 0;
      for (v = vs; v != ve; ++v) {
	//	  retval[j][i] = evecs[first_eigenvector_index+j-1][i];
	eigenvectors[j][i] = evecs[first_eigenvector_index+j-1][i];
	
	i++;
      }
    }

    //    return retval;
    
  } // end spectrum()
  
} // end namespace boost

#endif // BOOST_GRAPH_SPARSE_SPECTRUM_HPP

