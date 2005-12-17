// Ben Martin
// December 3, 2005

// This currently works only on undirected graphs...
// Graph must model Adjacency Graph, Incidence Graph, VertexListGraph

#ifndef BOOST_GRAPH_SPECTRUM_HPP
#define BOOST_GRAPH_SPECTRUM_HPP

#define USE_IETL 0

#if USE_IETL
#else
#define USE_LAPACK 1
#endif

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/properties.hpp>
#include <boost/vector_property_map.hpp>
#include <utility> // for pair

#include <iostream.h>
#ifdef USE_LAPACK
#include <vecLib/clapack.h>
//#include "f2c.h"
//#include "cblas.h"
//#include "clapack.h"
#elif USE_IETL
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
#endif

typedef long int integer;
typedef double doublereal;

#ifdef USE_LAPACK
extern "C" int dsyevr_(char *jobz, char *range, char *uplo, integer *n, 
		       doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *
		       il, integer *iu, doublereal *abstol, integer *m, doublereal *w, 
		       doublereal *z__, integer *ldz, integer *isuppz, doublereal *work, 
		       integer *lwork, integer *iwork, integer *liwork, integer *info);
#endif


namespace boost {

  // Parameters:
  //   first_eigenvector_index:
  //     Since the smallest eigenvector is not useful, often this will 
  //       be set to 1, for the "Fiedler vector," though if all 
  //       eigenvectors are desired, 0 may be a more logical value.
  //     Negative values are interpreted as allowing a default choice of 1.
  //   num_eigenvectors:
  //     The number of eigencectors to return.

  template <typename Graph, typename Matrix >
  void spectrum(Graph& g, 
		int first_eigenvector_index,
		int num_eigenvectors,
		//		std::vector<Vector> &eigenvectors,
		Matrix &eigenvectors,
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

#ifdef USE_LAPACK
    doublereal *A;
    A = new double[N * N];
#elif USE_IETL
    using namespace boost::numeric::ublas;
    typedef compressed_matrix<double> Matrix;
    Matrix A(N, N);
#endif
    
    int i;
    
    AdjacencyIterator a, as, ae;
    std::pair<AdjacencyIterator, AdjacencyIterator> ap;
    
#ifdef USE_LAPACK
    for (i = 0; i < N * N; i++)
      A[i] = 0;
#elif USE_IETL
#endif

    i = 0;
    for (v = vs; v != ve; ++v) {
#ifdef USE_LAPACK
      A[(index_map[*v] * N) + index_map[*v]] = (doublereal)out_degree(*v, g);
#elif USE_IETL
      A(index_map[*v], index_map[*v]) = (double)out_degree(*v, g);
#endif
    }
    for (e = es; e != ee; ++e) {
      src = source(*e, g);
      tgt = target(*e, g);
#ifdef USE_LAPACK      
      A[index_map[src] + index_map[tgt]*N] = (doublereal)(-1);
      A[index_map[src]*N + index_map[tgt]] = (doublereal)(-1);
#elif USE_IETL
      A(index_map[src], index_map[tgt]) = (double)(-1);
      A(index_map[tgt], index_map[src]) = (double)(-1);
#endif
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

#ifdef USE_LAPACK
    // Set up the 8 billion parameters for CLAPACK
    char   JOBZ   = 'v';
    char   RANGE  = 'i';
    char   UPLO   = 'l'; // arbitrary at the moment...
    // N is defined
    // A is defined
    integer    LDA    = N;
    doublereal VL     = 0; // not needed
    doublereal VU     = 0; // not needed
    integer    IL     = first_eigenvector_index + 1; //2 // first NON-ZERO eigenvalue
    integer    IU     = first_eigenvector_index + num_eigenvectors; // 4 for the 3d case
    //    char   dlamch_cmach = 's';

    //    doublereal ABSTOL = 0.001;// dlamch_(&dlamch_cmach); // most likely SEVERE OVERKILL
    doublereal ABSTOL = abs_tol*std::numeric_limits<double>::epsilon();
    integer    M = (IU - IL + 1); // N; // should come back 2 or 3 (for 2d, 3d respectively)
    doublereal *W = new double[N]; // the eigenvalues
    doublereal *Z = new double[N*M]; // the eigenvectors
    integer    LDZ = N;
    integer    *ISUPPZ = new integer[2*M];// (integer*)0; // = new int[2*(IU-IL+1)]; ??? Not needed?
    doublereal *WORK = new double[26*N]; // should this be allocated?
    integer    LWORK  = 26*N; // -1
    integer    *IWORK = new integer[10*N];
    integer    LIWORK = 10*N; // -1
    integer    INFO;

    dsyevr_(&JOBZ, &RANGE, &UPLO, &N, A, &LDA, &VL, &VU, &IL, &IU, &ABSTOL,
            &M, W, Z, &LDZ, ISUPPZ, WORK, &LWORK, IWORK, &LIWORK, &INFO);

    /*
    cout << "N = " << N << endl; 
    cout << "IL = " << INFO << endl; 
    cout << "IU = " << INFO << endl; 

    cout << "INFO = " << INFO << endl; 
    cout << "M = " << M << endl; 
    
    cout << "W = ";
    for (i = 0; i < M; i++)
      cout << W[i] << " ";
    cout << endl;
    for (i = 0; i < M; i++)
      for (int j = 0; j < N; j++)
      {
        cout << Z[i*N + j] << "";
        cout << endl;
      }
    */
#elif USE_IETL

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

    std::vector<boost::numeric::ublas::vector<double> > evecs(3);
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

#endif

    //    std::vector<Vector> retval(num_eigenvectors);
    for (int j = 0; j < num_eigenvectors; j++) {
      // retval[j] = *(new Vector(N));
      i = 0;
      for (v = vs; v != ve; ++v) {
#ifdef USE_LAPACK
	//	  retval[j][i] = Z[(first_eigenvector_index+j-1)*LDZ + i];
	eigenvectors[j][i] = Z[j*LDZ + i];
#elif USE_IETL
	//	  retval[j][i] = evecs[first_eigenvector_index+j-1][i];
	eigenvectors[j][i] = evecs[first_eigenvector_index+j-1][i];
	
#endif
	i++;
      }
    }

    //    return retval;
    
  } // end spectrum()
  
} // end namespace boost

#endif // BOOST_GRAPH_SPECTRUM_HPP

