// Ben Martin
// October 11, 2005

// This currently works only on undirected graphs...
// Graph must model Adjacency Graph, Incidence Graph, VertexListGraph



#ifndef BOOST_GRAPH_SPECTRAL_EMBEDDING_LAYOUT_HPP
#define BOOST_GRAPH_SPECTRAL_EMBEDDING_LAYOUT_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/properties.hpp>
#include <vecLib/clapack.h>
//#include "f2c.h"
//#include "cblas.h"
//#include "clapack.h"
#include <utility> // for pair
//#include <boost/graph/point2d.hpp>
//#include <boost/graph/point3d.hpp>
#include <iostream.h>

typedef long int integer;
typedef double doublereal;

namespace boost {

  template <typename Graph, typename PositionMap>
  void spectral_embedding_layout(Graph& g, PositionMap pos_map) {

    typedef typename property_map<Graph, vertex_index_t>::const_type IndexMap;
    typedef typename Graph::vertex_iterator VertexIterator;
    typedef typename boost::graph_traits<Graph>::adjacency_iterator AdjacencyIterator;
    
    IndexMap index_map = get(vertex_index, g);

    VertexIterator v, vs, ve;
    using std::pair;
    std::pair<VertexIterator, VertexIterator> p;
    p = vertices(g);
    vs = p.first;
    ve = p.second;
    
    integer N = num_vertices(g);

    doublereal *A;
    //    A = (doublereal*)malloc( N*N*sizeof(double) );
    A = new double[N * N];
    
    int i;
    
    AdjacencyIterator a, as, ae;
    std::pair<AdjacencyIterator, AdjacencyIterator> ap;
    
    for (i = 0; i < N * N; i++)
      A[i] = 0;
    i = 0;
    for (v = vs; v != ve; ++v) {
      A[(index_map[*v] * N) + index_map[*v]] = (doublereal)out_degree(*v, g);
      ap = adjacent_vertices(*v, g);
      as = ap.first;
      ae = ap.second;
      for (a = as; a != ae; ++a) {
	A[index_map[*v] + index_map[*a]*N] = (doublereal)(-1);
	A[index_map[*v]*N + index_map[*a]] = (doublereal)(-1);
      }

      pos_map[*v][0] = i;
      pos_map[*v][1] = i;
      
      i++;
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

    // Set up the 8 billion parameters for CLAPACK
    char   JOBZ   = 'v';
    char   RANGE  = 'i';
    char   UPLO   = 'l'; // arbitrary at the moment...
    // N is defined
    // A is defined
    integer    LDA    = N;
    doublereal VL     = 0; // not needed
    doublereal VU     = 0; // not needed
    integer    IL     = 2;// 1; // first NON-ZERO eigenvalue
    integer    IU     = 3;// N; // 4 for the 3d case
    //    char   dlamch_cmach = 's';

    doublereal ABSTOL = 0.01;// dlamch_(&dlamch_cmach); // most likely SEVERE OVERKILL
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

    //    dstegr_()
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

    // Set position based on eigenvectors
    i = 0;
    for (v = vs; v != ve; ++v)
    {
      pos_map[*v][0] = Z[0*LDZ + i];
      i++;
    }
    i = 0;
    for (v = vs; v != ve; ++v)
    {
      pos_map[*v][1] = Z[1*LDZ + i];
      i++;
    }
    
  } // end spectral_embedding_layout()
  
} // end namespace boost

#endif // BOOST_GRAPH_SPECTRAL_EMBEDDING_LAYOUT_HPP

