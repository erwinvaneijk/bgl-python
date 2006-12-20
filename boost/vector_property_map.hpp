// Copyright (C) Vladimir Prus 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/graph/vector_property_map.html for
// documentation.
//

#ifndef VECTOR_PROPERTY_MAP_HPP_VP_2003_03_04
#define VECTOR_PROPERTY_MAP_HPP_VP_2003_03_04

#include <boost/property_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

namespace boost {
    template<typename T, typename IndexMap = identity_property_map>
    class vector_property_map
        : public boost::put_get_helper< 
              typename std::iterator_traits< 
                  typename std::vector<T>::iterator >::reference,
              vector_property_map<T, IndexMap> >
    {
    public:
        typedef typename property_traits<IndexMap>::key_type  key_type;
        typedef T value_type;
        typedef typename std::iterator_traits< 
            typename std::vector<T>::iterator >::reference reference;
        typedef boost::lvalue_property_map_tag category;
        
        vector_property_map(const IndexMap& index = IndexMap())
        : store(new std::vector<T>()), index(index)
        {}

        vector_property_map(unsigned initial_size, 
                            const IndexMap& index = IndexMap())
        : store(new std::vector<T>(initial_size)), index(index)
        {}

        vector_property_map(const shared_ptr<std::vector<T> >& store,
                            const IndexMap& index)
          : store(store), index(index) 
        {}

        typename std::vector<T>::iterator storage_begin()
        {
            return store->begin();
        }

        typename std::vector<T>::iterator storage_end()
        {
            return store->end();
        }

        typename std::vector<T>::const_iterator storage_begin() const
        {
            return store->begin();
        }

        typename std::vector<T>::const_iterator storage_end() const
        {
            return store->end();
        }
         
        shared_ptr< std::vector<T> >&       get_store()       { return store; }
        const shared_ptr< std::vector<T> >& get_store() const { return store; }

        IndexMap&       get_index_map()       { return index; }
        const IndexMap& get_index_map() const { return index; }

    public:
        // Copy ctor absent, default semantics is OK.
        // Assignment operator absent, default semantics is OK.
        // CONSIDER: not sure that assignment to 'index' is correct.
        
        reference operator[](const key_type& v) const {
            typename property_traits<IndexMap>::value_type i = get(index, v);
            if (static_cast<unsigned>(i) == store->size()) {
              store->push_back(T());
            } else if (static_cast<unsigned>(i) > store->size()) {
                store->resize(i + 1, T());
            }
            return (*store)[i];
        }
    private:
        // Conceptually, we have a vector of infinite size. For practical 
        // purposes, we start with an empty vector and grow it as needed.
        // Note that we cannot store pointer to vector here -- we cannot
        // store pointer to data, because if copy of property map resizes
        // the vector, the pointer to data will be invalidated. 
        // I wonder if class 'pmap_ref' is simply needed.
        shared_ptr< std::vector<T> > store;        
        IndexMap index;
    };
    
    template<typename T, typename IndexMap>
    vector_property_map<T, IndexMap>
    make_vector_property_map(IndexMap index)
    {
        return vector_property_map<T, IndexMap>(index);
    }

    /*
     * A weak reference to a vector_property_map. The
     * vector_property_map keeps a shared_ptr to its underlying
     * storage. To keep track of this shared_ptr without incrementing
     * its reference count, store a weak_vector_property_map (which
     * itself contains a weak_ptr), which can be converted to a normal
     * vector_property_map. Note that weak_vector_property_map is not
     * actually a property map (just like weak_ptr is not actually a
     * pointer).
     */
    template<typename T, typename IndexMap = identity_property_map>
    class weak_vector_property_map
    {
    public:
      explicit 
      weak_vector_property_map(const vector_property_map<T, IndexMap>& pmap)
        : store(pmap.get_store()), index(pmap.get_index_map()) { }

      operator vector_property_map<T, IndexMap>() const
      {
        return vector_property_map<T, IndexMap>(store, index);
      }

    private:
      weak_ptr<std::vector<T> > store;
      IndexMap index;
    };
}

#endif
