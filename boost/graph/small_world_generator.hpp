// Copyright 2004 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Ben Martin
//           Andrew Lumsdaine


#ifndef BOOST_GRAPH_SMALL_WORLD_GENERATOR_HPP
#define BOOST_GRAPH_SMALL_WORLD_GENERATOR_HPP

#include <iterator>
#include <utility>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int.hpp>

#if !defined BOOST_NO_HASH
#  ifdef BOOST_HASH_SET_HEADER
#    include BOOST_HASH_SET_HEADER
#    define SET BOOST_STD_EXTENSION_NAMESPACE::hash_set
#  else
#    include <hash_set>
#    define SET std::hash_set
#  endif
#else
#  include <set>
#  define SET std::set
#endif

namespace boost {

  // Assumes undirected
  template<typename RandomGenerator, typename Graph>
  class small_world_iterator
  {
    typedef typename graph_traits<Graph>::vertices_size_type 
      vertices_size_type;
    typedef typename graph_traits<Graph>::edges_size_type 
      edges_size_type;

  public:
    typedef std::input_iterator_tag iterator_category;
    typedef std::pair<vertices_size_type, vertices_size_type> value_type;
    typedef SET<edges_size_type> edges_set;
    typedef const value_type& reference;
    typedef const value_type* pointer;
    typedef void difference_type;

    small_world_iterator() : gen(0) {}
    small_world_iterator(RandomGenerator& gen, vertices_size_type n, 
                         vertices_size_type k, double prob = 0.0, 
                         bool allow_self_loops = false, bool allow_multiple_edges = true) 
      : gen(&gen), n(n), k(k+k%2), prob(prob), source(0), 
        target(allow_self_loops? 0 : 1), 
        allow_self_loops(allow_self_loops),
        allow_multiple_edges(allow_multiple_edges), 
        current(0, allow_self_loops? 0 : 1),
#if !defined BOOST_NO_HASH
	edges(allow_multiple_edges? 0 : (k + k%2)*n/2)
#else
	edges()
#endif 
    {}

    reference operator*() const { return current; }
    pointer operator->() const { return &current; }
    
    small_world_iterator& operator++()
    { 
      current.first = source;

      uniform_01<RandomGenerator, double> rand01(*gen);
      uniform_int<vertices_size_type> rand_vertex_gen(0, n-1);
      double x = rand01();
      *gen = rand01.base(); // GRRRR
      vertices_size_type larger_v = 0;
      vertices_size_type smaller_v = 0;
      if (x < prob) {
        do {
          current.second = rand_vertex_gen(*gen);
	  if (!allow_multiple_edges) {
	    larger_v = current.first;
	    smaller_v = current.second;
	    if (current.first < current.second) {
	      smaller_v = current.first;
	      larger_v = current.second;
	    }
	  }
	  // std::cout << "swg " << current.first << " " << current.second << " " << smaller_v*n + larger_v << std::endl;
        } while ((!allow_self_loops && current.first == current.second)
		 ||
		 (!allow_multiple_edges && (current.first < k/2 && current.second >= n - k/2 + current.first))
		 ||
		 (!allow_multiple_edges && (edges.find(smaller_v*n + larger_v) != edges.end())));
      } else {
        current.second = target;
	if (!allow_multiple_edges) {
	  larger_v = current.first;
	  smaller_v = current.second;
	  if (current.first < current.second) {
	    smaller_v = current.first;
	    larger_v = current.second;
	  }
	}
      }

      if (!allow_multiple_edges) {
	edges.insert(smaller_v*n + larger_v);
	// std::cout << "added " << current.first << " " << current.second << " " << smaller_v*n + larger_v << std::endl;
      }

      target = (target + 1) % n;
      if (target == (source + k/2 + 1) % n) {
        ++source;
        if (allow_self_loops) target = source;
        else target = (source + 1) % n;
      }

      return *this;
    }

    small_world_iterator operator++(int)
    {
      small_world_iterator temp(*this);
      ++(*this);
      return temp;
    }

    bool operator==(const small_world_iterator& other) const
    {
      if (!gen && other.gen) return other == *this;
      else if (gen && !other.gen) return source == n;
      else if (!gen && !other.gen) return true;
      return source == other.source && target == other.target;
    }

    bool operator!=(const small_world_iterator& other) const
    { return !(*this == other); }

  private:
    RandomGenerator* gen;
    vertices_size_type n;
    vertices_size_type k;
    double prob;
    vertices_size_type source;
    vertices_size_type target;
    bool allow_self_loops;
    bool allow_multiple_edges;
    value_type current;
    edges_set edges;
  };

} // end namespace boost

#endif // BOOST_GRAPH_SMALL_WORLD_GENERATOR_HPP
