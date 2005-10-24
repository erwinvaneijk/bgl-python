// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

#if !defined(BGL_PYTHON_VISITOR)
#  error You must define BGL_PYTHON_VISITOR to the name of the visitor class.
#endif

#if !defined(BGL_PYTHON_EVENTS_HEADER)
#  error You must define BGL_PYTHON_EVENTS_HEADER to the events header file.
#endif

class BGL_PYTHON_VISITOR
{
public:
  explicit BGL_PYTHON_VISITOR(boost::python::object visitor) 
    : visitor(visitor) { }

#define BGL_PYTHON_EVENT(Name,Descriptor)                               \
  template<typename Graph>                                              \
  void                                                                  \
  Name(typename graph_traits<Graph>::BOOST_JOIN(Descriptor,_descriptor) x, \
       const Graph& g) const                                            \
  {                                                                     \
    using boost::python::object;                                        \
    if (visitor != object()) {                                          \
      object attr = visitor.attr(#Name);                                \
      if (attr != object())                                             \
        attr(x, ref(g));                                                \
    }                                                                   \
  }
#  include BGL_PYTHON_EVENTS_HEADER
#undef BGL_PYTHON_EVENT


protected:
  boost::python::object visitor;
};

