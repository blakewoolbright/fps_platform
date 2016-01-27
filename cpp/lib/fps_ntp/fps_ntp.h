#ifndef FPS__NTP__H
#define FPS__NTP__H

#include <type_traits>
#include <cstdint>

//------------------------------------------------------------------------------------------------
// NTP :: Named Template Parameters
//
// This library implements the "named template parameter" idiom.  Instead of using traditional 
// c++ syntax like :
//   template< bool Some_Flag, uint32_t Some_Value >
//   struct SomeClass 
//   {} ;
//   ...
//   SomeClass< true, 1024 > instance ;
// 
// Users may define named parameters that can be passed in any order, which makes usage 
// more explicit, eg. :
//   SomeClass< options::SomeFlag, options::SomeValue< 1024 > > instance ;
//
// See the unit test in the 'test' subdirectory for an example.
//------------------------------------------------------------------------------------------------

namespace fps {
namespace ntp {

  namespace detail 
  {
    template< bool B, class T = void >
    using enable_if_t = typename std::enable_if<B,T>::type ;
  }

  //---------------------------------------------------------------------------------------------- 
  // Parameter type containers
  //---------------------------------------------------------------------------------------------- 
  template<typename T_ID> 
  struct Flag 
  { using id = T_ID ; 
  } ;

  template<typename T_ID, typename T>
  struct Type
  { using   id    = T_ID ;
    using   value = T ;
  } ;

  template<typename T_ID, template<typename> class T>
  struct Template
  { using id = T_ID ;
    template<typename U> using value = T<U> ;
  } ;

  template<typename T_ID, typename T, T default_value>
  struct Value  
    : std::integral_constant<T, default_value>
  { 
    using id = T_ID ;
  } ;

  //---------------------------------------------------------------------------------------------- 
  // Parameter value extractors
  //---------------------------------------------------------------------------------------------- 

  // Declaration
  template<typename T, typename... Args> struct get_value ;

  // Recursive search of variadic list
  template<typename T, typename T_Next, typename... Args> 
  struct get_value<T, T_Next, Args...>
  {
  private : 
    // TODO : Can we stash the type of the default value here?
    // typedef typename decltype( T) T_Next ;

    template<typename U, typename U_Type, typename U_Enable = void>
    struct unpack 
      : std::integral_constant< decltype( T::value ), get_value<T, Args...>::value >
    {} ;

    template<typename U, typename U_Type>
    struct unpack< U
                 , U_Type
                 , detail::enable_if_t<std::is_same<typename T::id, typename U_Type::id>::value>
                 >
      : std::integral_constant< decltype( T::value ), U_Type::value >
    {} ;
                                                    
  public :
    static constexpr const auto value = unpack<T, T_Next>::value ;
  } ;

  // Termination Condition
  template<typename T>
  struct get_value<T> 
    : std::integral_constant< decltype( T::value ), T::value > 
  {} ;

  //---------------------------------------------------------------------------------------------- 
  // Parameter type extractor 
  //---------------------------------------------------------------------------------------------- 
  // Declaration
  template<typename T, typename... Args> struct get_type ;

  // Recursive search of variadic list for matching type id
  template<typename T, typename T_Next, typename... Args>
  struct get_type<T, T_Next, Args...> 
  {
  private :
    template<typename U, typename U_Next, typename Enable = void>
    struct unpack 
    { using value = typename get_type<T, Args...>::value ; 
    } ;

    template<typename U, typename U_Next>
    struct unpack< U
                 , U_Next
                 , detail::enable_if_t< std::is_same<typename U::id, typename U_Next::id>::value>
                 > 
    { using value = typename U::value ; 
    } ;

  public :
    using value = typename unpack<T, T_Next>::value ;
  } ;

  // Termination condition
  template<typename T>
  struct get_type<T> { using value = typename T::value ; } ;  

  //---------------------------------------------------------------------------------------------- 
  // Parameter type extractor ( for template types )
  //---------------------------------------------------------------------------------------------- 
  template<typename T, typename... Args> struct get_template_type ;

  // Recursive search of variadic list
  template<typename T, typename T_Next, typename... Args>
  struct get_template_type<T, T_Next, Args...> 
  {
  private :
    template<typename U, typename U_Next, typename U_Enable = void>
    struct unpack
    { 
      template<typename V>
      using value = typename get_template_type<T, Args...>::template value<V> ;
    } ;

    template<typename U, typename U_Next>
    struct unpack< U
                 , U_Next
                 , detail::enable_if_t<std::is_same<typename U::id, typename U_Next::id>::value>
                 > 
    { 
      template<typename V> using value = typename U::template value<V> ;
    } ;

  public :

    template<typename U> using value = typename unpack<T, T_Next>::template value<U> ;
  } ;

  template<typename T>
  struct get_template_type<T> 
  { template<typename U> using value = typename T::template value<U> ;
  } ;

  //---------------------------------------------------------------------------------------------- 
  // Check template argument list to see whether a specific flag is present
  //---------------------------------------------------------------------------------------------- 
  // 
  // Declaration
  template<typename T, typename... Args> struct is_present ;

  // Recursive search of variadic list
  template<typename T, typename T_Next, typename... Args>
  struct is_present<T, T_Next, Args...> 
    : std::integral_constant< bool
                            , std::is_same<T, T_Next>::value || is_present<T, Args...>::value
                            > 
  {} ;

  // Termination condition
  template<typename T> struct is_present<T> : std::false_type {} ;


}}

#endif

