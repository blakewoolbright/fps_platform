#ifndef FPS__JSON__ITERATOR__H
#define FPS__JSON__ITERATOR__H

#include "fps_json/detail/types.h"

#include <boost/iterator/iterator_facade.hpp>
#include <type_traits>

namespace fps  {
namespace json { 
  namespace detail {

    //-----------------------------------------------------------------------------
    template<typename T>
    class iterator_base
      : public boost::iterator_facade< iterator_base<T>
                                     , T 
                                     , boost::forward_traversal_tag
                                     >
    {
    private :
      //---------------------------------------------------------------------------
      friend class boost::iterator_core_access ;
      template <class> friend class iterator_base ;
      
      static constexpr bool Is_Const = std::is_const<T>::value ;

      //---------------------------------------------------------------------------
      typedef iterator_base<T> this_t ;
      typedef T value_t ;
      typedef typename std::remove_const<value_t>::type mutable_value_t ;

    public :

      //---------------------------------------------------------------------------
      typedef detail::tree_t::iterator itr_t ;
      itr_t                  itr_ ;
      mutable mutable_value_t val_ ;

      //---------------------------------------------------------------------------
      inline void increment() { ++itr_ ; val_.clear() ; }

      //---------------------------------------------------------------------------
      inline 
      T &
      dereference() const 
      { 
        if( !val_.is_valid() ) 
          val_.update( itr_->first, itr_->second ) ;
        return val_ ;
      }

      //---------------------------------------------------------------------------
      template<typename T_Other>
      inline 
      bool 
      equal( const iterator_base<T_Other> & rhs ) const 
      { return itr_ == rhs.itr_ ; 
      }

    public :
      //---------------------------------------------------------------------------
      inline iterator_base() {}
      inline iterator_base( const itr_t & itr ) : itr_( itr ) {}  

      //---------------------------------------------------------------------------
      template<typename T_Other>
      inline
      iterator_base( const iterator_base<T_Other> & rhs ) 
        : itr_( rhs.itr_ ) 
      {}
    } ;
  }

}}

#endif
