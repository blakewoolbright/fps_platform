#ifndef FPS__UTIL__SINGLETON__H
#define FPS__UTIL__SINGLETON__H

namespace fps  {
namespace util {

  //-------------------------------------------------------
  // Myers-style singleton
  //-------------------------------------------------------
  template<typename T>
  struct Singleton
  {
  public :
    //-----------------------------------------------------
    static 
    inline 
    T & 
    instance()
    { 
      static T instance_ ;
      return instance_  ;
    } 

  private :
    //-----------------------------------------------------
    inline Singleton() {}

    //-----------------------------------------------------
    Singleton( const Singleton & ) ;
    ~Singleton() ;
    Singleton & operator=( const Singleton & ) ;
  } ;

}}

#endif

