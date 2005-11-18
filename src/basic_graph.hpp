// Copyright 2004-5 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_BASIC_GRAPH_HPP
#define BOOST_GRAPH_BASIC_GRAPH_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/vector_property_map.hpp>
#include <boost/dynamic_property_map.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/python.hpp>
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/iterator.hpp>

namespace boost { namespace graph { namespace python {

template<typename T, typename DirectedS>
struct basic_descriptor
{
  basic_descriptor() {}
  basic_descriptor(T base) : base(base) { }

  operator T() const { return base; }

  struct create 
  {
    typedef basic_descriptor result_type;
    basic_descriptor operator()(T base) const { return base; }
  };

  T base;
};

template<typename T, typename DirectedS>
inline bool 
operator==(const basic_descriptor<T, DirectedS>& u,
           const basic_descriptor<T, DirectedS>& v)
{ return u.base == v.base; }

template<typename T, typename DirectedS>
inline bool 
operator!=(const basic_descriptor<T, DirectedS>& u,
           const basic_descriptor<T, DirectedS>& v)
{ return u.base != v.base; }

template<typename Key, typename IndexMap>
struct basic_index_map
{
  typedef Key                                                  key_type;
  typedef typename property_traits<IndexMap>::value_type value_type;
  typedef typename property_traits<IndexMap>::reference  reference;
  typedef typename property_traits<IndexMap>::category   category;

  basic_index_map(const IndexMap& id = IndexMap())
    : id(id) { }

  IndexMap id;
};

template<typename Key, typename IndexMap>
inline typename basic_index_map<Key, IndexMap>::value_type
get(const basic_index_map<Key, IndexMap>& pm, 
    typename basic_index_map<Key, IndexMap>::key_type const& key)
{ return get(pm.id, key.base); }

class resizable_property_map : boost::noncopyable
{
 public:
  /* Invoked when a new value has been added to the end of the list of
     keys. The property map may resize it's internal data structure to
     accomodate this. Returns true so long as this property map is
     still "relevant" and should be kept. */ 
  virtual bool added_key() = 0;

  /* The key with the given index is being removed, and the value with
     the last index will replace it. Returns true so long as this
     property map is still "relevant" and should be kept. */
  virtual bool removed_key(std::size_t index) = 0;

  /* All of the keys in the graph have been shuffled. This vector maps
     from the old indices to the new indices. Returns true so long as
     this property map is still "relevant" and should be kept. */
  virtual bool shuffled_keys(const std::vector<std::size_t>& new_indices) = 0;

  virtual ~resizable_property_map() {}

 protected:
  resizable_property_map() {}
};

struct stored_minstd_rand
{
  stored_minstd_rand(int seed = 1) : gen(seed) { }

  minstd_rand gen;
};

template<typename DirectedS>
class basic_graph
  : public stored_minstd_rand,
    public adjacency_list<listS, listS, DirectedS,
                          property<vertex_index_t, std::size_t>,
                          property<edge_index_t, std::size_t> >
{
  typedef adjacency_list<listS, listS, DirectedS,
                         property<vertex_index_t, std::size_t>,
                         property<edge_index_t, std::size_t> > inherited;
  typedef graph_traits<inherited> traits;

  typedef typename traits::vertex_iterator           base_vertex_iterator;
  typedef typename traits::edge_iterator             base_edge_iterator;
  typedef typename traits::out_edge_iterator         base_out_edge_iterator;
  typedef typename traits::in_edge_iterator          base_in_edge_iterator;
  typedef typename traits::adjacency_iterator        base_adjacency_iterator;
  typedef typename property_map<inherited, vertex_index_t>::const_type
                                                     base_vertex_index_map;
  typedef typename property_map<inherited, edge_index_t>::const_type
                                                     base_edge_index_map;
  typedef typename traits::vertex_descriptor         base_vertex_descriptor;
  typedef typename traits::edge_descriptor           base_edge_descriptor;

 public:
  typedef basic_descriptor<base_vertex_descriptor, DirectedS>
                                                     Vertex;
  typedef Vertex                                     vertex_descriptor;
  typedef basic_descriptor<base_edge_descriptor, DirectedS>
                                                     Edge;
  typedef Edge                                       edge_descriptor;
  typedef basic_index_map<Vertex, base_vertex_index_map>
                                                     VertexIndexMap;
  typedef basic_index_map<Edge, base_edge_index_map> EdgeIndexMap;
  typedef std::size_t                                vertices_size_type;
  typedef std::size_t                                edges_size_type;
  typedef std::size_t                                degree_size_type;
  typedef typename traits::directed_category         directed_category;
  typedef typename traits::edge_parallel_category    edge_parallel_category;
  typedef typename traits::traversal_category        traversal_category; 
  typedef transform_iterator<typename Vertex::create, base_vertex_iterator>
                                                     vertex_iterator;
  typedef transform_iterator<typename Edge::create, base_edge_iterator>
                                                     edge_iterator;
  typedef transform_iterator<typename Edge::create, base_out_edge_iterator>
                                                     out_edge_iterator;
  typedef transform_iterator<typename Edge::create, base_in_edge_iterator>
                                                     in_edge_iterator;
  typedef transform_iterator<typename Vertex::create, base_adjacency_iterator>
                                                     adjacency_iterator;

  basic_graph();
  basic_graph(boost::python::object, 
              const std::string& name_map = std::string());

  template<typename InputIterator>
  basic_graph(InputIterator first, InputIterator last,
              vertices_size_type n)
    : inherited(first, last, n)
  {
    renumber_vertices();
    renumber_edges();
  }

  bool is_directed() const
  { return is_convertible<directed_category, directed_tag>::value; }

  Vertex add_vertex();
  void clear_vertex(Vertex vertex);
  void remove_vertex(Vertex vertex);
  
  Edge add_edge(Vertex u, Vertex v);
  void remove_edge(Edge edge);

  std::pair<Edge, bool> edge(Vertex u, Vertex v) const;
  
  // Property map handling
  void register_vertex_map(std::auto_ptr<resizable_property_map> map)
  { 
    vertex_maps.push_back(map.get()); 
    map.release();
  }

  void register_edge_map(std::auto_ptr<resizable_property_map> map)
  { 
    edge_maps.push_back(map.get()); 
    map.release();
  }

  inherited&       base()       { return *this; }
  const inherited& base() const { return *this; }
  
  dynamic_properties&       get_dynamic_properties()       { return dp; }
  const dynamic_properties& get_dynamic_properties() const { return dp; }

protected:
  void renumber_vertices();
  void renumber_edges();
  
private:
  /* Mapping from indices to descriptors, which allows us to provide
     edge and vertex removal while retaining O(1) lookup for external
     property maps. */
  std::vector<vertex_descriptor> index_to_vertex;
  std::vector<edge_descriptor>   index_to_edge;

  /* The lists of property maps "attached" to the graph, which will be
     updated to deal with vertex and edge removals. */
  std::list<resizable_property_map*> vertex_maps;
  std::list<resizable_property_map*> edge_maps;

  // DPG TBD: Currently an awful mess
  dynamic_properties dp;
};

// Vertex List Graph concept
template<typename DirectedS>
inline std::pair<typename basic_graph<DirectedS>::vertex_iterator, 
                 typename basic_graph<DirectedS>::vertex_iterator>
vertices(const basic_graph<DirectedS>& g)
{ 
  typedef typename graph_traits<basic_graph<DirectedS> >::vertex_iterator
    vertex_iterator;
  typedef typename graph_traits<basic_graph<DirectedS> >::vertex_descriptor
    Vertex;

  return std::make_pair(vertex_iterator(vertices(g.base()).first, 
                                        typename Vertex::create()),
                        vertex_iterator(vertices(g.base()).second, 
                                        typename Vertex::create()));
}

template<typename DirectedS>
inline std::size_t num_vertices(const basic_graph<DirectedS>& g) 
{ return num_vertices(g.base()); }

// Edge List Graph concept
template<typename DirectedS>
inline std::pair<typename basic_graph<DirectedS>::edge_iterator, 
                 typename basic_graph<DirectedS>::edge_iterator>
edges(const basic_graph<DirectedS>& g)
{
  typedef typename graph_traits<basic_graph<DirectedS> >::edge_iterator 
    edge_iterator;
  typedef typename graph_traits<basic_graph<DirectedS> >::edge_descriptor Edge;

  return std::make_pair(edge_iterator(edges(g.base()).first, 
                                        typename Edge::create()),
                        edge_iterator(edges(g.base()).second, 
                                        typename Edge::create()));  
}

template<typename DirectedS>
inline std::size_t num_edges(const basic_graph<DirectedS>& g) 
{ return num_edges(g.base()); }

// Incidence Graph concept
template<typename DirectedS>
inline typename basic_graph<DirectedS>::vertex_descriptor 
source(typename basic_graph<DirectedS>::edge_descriptor e, 
       const basic_graph<DirectedS>& g)
{ 
  typedef typename graph_traits<basic_graph<DirectedS> >::vertex_descriptor
    Vertex;
  return Vertex(source(e.base, g.base()));
}

template<typename DirectedS>
inline typename basic_graph<DirectedS>::vertex_descriptor 
target(typename basic_graph<DirectedS>::edge_descriptor e, 
       const basic_graph<DirectedS>& g)
{ 
  typedef typename graph_traits<basic_graph<DirectedS> >::vertex_descriptor
    Vertex;
  return Vertex(target(e.base, g.base()));
}

template<typename DirectedS>
inline std::pair<typename basic_graph<DirectedS>::out_edge_iterator, 
                 typename basic_graph<DirectedS>::out_edge_iterator>
out_edges(typename basic_graph<DirectedS>::vertex_descriptor u, 
          const basic_graph<DirectedS>& g)
{ 
  typedef typename graph_traits<basic_graph<DirectedS> >::out_edge_iterator
    out_edge_iterator;
  typedef typename graph_traits<basic_graph<DirectedS> >::edge_descriptor Edge;
  return std::make_pair(out_edge_iterator(out_edges(u.base, g.base()).first, 
                                          typename Edge::create()),
                        out_edge_iterator(out_edges(u.base, g.base()).second,
                                          typename Edge::create()));
}

template<typename DirectedS>
inline std::size_t 
out_degree(typename basic_graph<DirectedS>::vertex_descriptor u, 
           const basic_graph<DirectedS>& g)
{ return out_degree(u.base, g.base()); }

// Bidirectional Graph concept
template<typename DirectedS>
inline std::pair<typename basic_graph<DirectedS>::in_edge_iterator, 
                 typename basic_graph<DirectedS>::in_edge_iterator>
in_edges(typename basic_graph<DirectedS>::vertex_descriptor u, 
         const basic_graph<DirectedS>& g)
{ 
  typedef typename graph_traits<basic_graph<DirectedS> >::in_edge_iterator
    in_edge_iterator;
  typedef typename graph_traits<basic_graph<DirectedS> >::edge_descriptor Edge;
  return std::make_pair(in_edge_iterator(in_edges(u.base, g.base()).first, 
                                         typename Edge::create()),
                        in_edge_iterator(in_edges(u.base, g.base()).second,
                                         typename Edge::create()));
}

template<typename DirectedS>
inline std::size_t 
in_degree(typename basic_graph<DirectedS>::vertex_descriptor u, 
          const basic_graph<DirectedS>& g)
{ return in_degree(u.base, g.base()); }

// Adjacency Graph concept
template<typename DirectedS>
inline std::pair<typename basic_graph<DirectedS>::adjacency_iterator, 
                 typename basic_graph<DirectedS>::adjacency_iterator>
adjacent_vertices(typename basic_graph<DirectedS>::vertex_descriptor u, 
                  const basic_graph<DirectedS>& g)
{ 
  typedef typename graph_traits<basic_graph<DirectedS> >::adjacency_iterator
    adjacency_iterator;
  typedef typename graph_traits<basic_graph<DirectedS> >::vertex_descriptor
    Vertex;

  return std::make_pair
           (adjacency_iterator(adjacent_vertices(u.base, g.base()).first, 
                               typename Vertex::create()),
            adjacency_iterator(adjacent_vertices(u.base, g.base()).second, 
                               typename Vertex::create()));
}

// Mutable basic_graph<DirectedS> concept
template<typename DirectedS>
inline typename basic_graph<DirectedS>::vertex_descriptor 
add_vertex(basic_graph<DirectedS>& g)
{ return g.add_vertex(); }

template<typename DirectedS>
inline std::pair<typename basic_graph<DirectedS>::edge_descriptor, bool>
add_edge(typename basic_graph<DirectedS>::vertex_descriptor u, 
         typename basic_graph<DirectedS>::vertex_descriptor v, 
         basic_graph<DirectedS>& g)
{ return std::make_pair(g.add_edge(u, v), true); }

// Adjacency list extras
template<typename DirectedS>
inline std::pair<typename basic_graph<DirectedS>::edge_descriptor, bool>
edge(typename basic_graph<DirectedS>::vertex_descriptor u, 
     typename basic_graph<DirectedS>::vertex_descriptor v, 
     const basic_graph<DirectedS>& g)
{ 
  return g.edge(u, v);
}

template<typename DirectedS>
void export_basic_graph(const char* name);

template<typename DirectedS>
typename basic_graph<DirectedS>::VertexIndexMap
get(vertex_index_t, const basic_graph<DirectedS>& g)
{ return get(vertex_index, g.base()); }

template<typename DirectedS>
typename basic_graph<DirectedS>::EdgeIndexMap
get(edge_index_t, const basic_graph<DirectedS>& g)
{ return get(edge_index, g.base()); }

template<typename DirectedS>
struct graph_pickle_suite : boost::python::pickle_suite
{
  typedef basic_graph<DirectedS> Graph;
  typedef typename Graph::vertex_descriptor Vertex;
  typedef typename Graph::edge_descriptor Edge;

  static
  boost::python::tuple
  getstate(boost::python::object g_obj);
  
  static
  void
  setstate(boost::python::object g_obj, boost::python::tuple state);
};

class python_dynamic_property_map
{
 public:
  virtual ~python_dynamic_property_map() {}

  virtual void copy_value(const any& to, const any& from) = 0;
  virtual boost::python::object get_python(const any& key) = 0;
};

template<typename PropertyMap,
         typename ValueType = typename property_traits<PropertyMap>::value_type>
class python_dynamic_adaptor
  : public boost::detail::dynamic_property_map_adaptor<PropertyMap>, 
    public python_dynamic_property_map
{
  typedef boost::detail::dynamic_property_map_adaptor<PropertyMap> inherited;

public:
  typedef typename property_traits<PropertyMap>::key_type key_type;

  explicit python_dynamic_adaptor(const PropertyMap& property_map)
    : inherited(property_map) { }

  virtual void copy_value(const any& to, const any& from)
  { 
    boost::put(this->base(), any_cast<key_type>(to), 
               boost::get(this->base(), any_cast<key_type>(from)));
  }

  virtual boost::python::object get_python(const any& key)
  {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
    return boost::get(this->base(), any_cast<key_type>(key));
#else
    using boost::get;

    return boost::python::object(get(this->base(), any_cast<key_type>(key)));
#endif    
  }
};

template<typename PropertyMap>
class python_dynamic_adaptor<PropertyMap, boost::python::object>
  : public boost::detail::dynamic_property_map_adaptor<PropertyMap>, 
    public python_dynamic_property_map
{
  typedef boost::detail::dynamic_property_map_adaptor<PropertyMap> inherited;

public:
  typedef typename property_traits<PropertyMap>::key_type key_type;

  explicit python_dynamic_adaptor(const PropertyMap& property_map)
    : inherited(property_map) { }

  virtual void copy_value(const any& to, const any& from)
  { 
    boost::put(this->base(), any_cast<key_type>(to), 
               boost::get(this->base(), any_cast<key_type>(from)));
  }

  virtual std::string get_string(const any& key)
  {
    using boost::python::extract;
    using boost::python::str;
    return std::string(
             extract<const char*>(str(boost::get(this->base(),
                                                 any_cast<key_type>(key)))));
  }

  virtual boost::python::object get_python(const any& key)
  {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
    return boost::get(this->base(), any_cast<key_type>(key));
#else
    using boost::get;

    return get(this->base(), any_cast<key_type>(key));
#endif    
  }
};

extern const char* graph_init_doc;
extern const char* vertex_property_map_doc;
extern const char* edge_property_map_doc;

} } } // end namespace boost::graph::python

namespace boost {
  // Doing the right thing here (by unifying with vertex_index_t and
  // edge_index_t) breaks GCC.
  template<typename DirectedS, typename Tag>
  struct property_map<graph::python::basic_graph<DirectedS>, Tag>
  {
  private:
    typedef typename graph::python::basic_graph<DirectedS>::VertexIndexMap
      vertex_index_type;
    typedef typename graph::python::basic_graph<DirectedS>::EdgeIndexMap
      edge_index_type;

    typedef typename mpl::if_<is_same<Tag, edge_index_t>,
                              edge_index_type,
                              detail::error_property_not_found>::type
      edge_or_none;

  public:
    typedef typename mpl::if_<is_same<Tag, vertex_index_t>,
                              vertex_index_type,
                              edge_or_none>::type type;

    typedef type const_type;
  };
}

#endif // BOOST_GRAPH_BASIC_GRAPH_HPP
