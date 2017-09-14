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

  //
  // Note: Workaround because gcc 4.9.x's c++11 implementation doesn't support std::enable_if_t.
  //
  namespace detail 
  {
    template< bool B, class T = void >
    using enable_if_t = typename std::enable_if<B,T>::type ;
  }

  //---------------------------------------------------------------------------------------------- 
  // Parameter type container definitions
  //---------------------------------------------------------------------------------------------- 
  // 
  // 'Flag' type arguments are boolean values that are implicitly true if they are present. 
  //
  template<typename T_ID> 
  struct Flag 
  { 
    using id = T_ID ; 
  } ;

  // 
  // 'Type' type arguments are typenames 
  //
  template<typename T_ID, typename T>
  struct Type
  { 
    using id    = T_ID ;
    using value = T ;
  } ;

  //
  // 'Template' type arguments are template typenames
  // 
  template<typename T_ID, template<typename> class T>
  struct Template
  { 
    using id = T_ID ;
    template<typename U> using value = T<U> ;
  } ;

  //
  // 'Value' type arguments are primitive values
  //
  template<typename T_ID, typename T, T default_value>
  struct Value  
    : std::integral_constant<T, default_value>
  { 
    using id = T_ID ;
  } ;

  //---------------------------------------------------------------------------------------------- 
  // Parameter value extraction structure, 'get_value' declaration and specializations
  //---------------------------------------------------------------------------------------------- 
  template<typename T, typename... Args> 
  struct get_value ;

  template<typename T, typename T_Next, typename... Args> 
  struct get_value<T, T_Next, Args...>
  {
  private : 
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

  template<typename T>
  struct get_value<T> 
    : std::integral_constant< decltype( T::value ), T::value > 
  {} ;

  //---------------------------------------------------------------------------------------------- 
  // Parameter type extraction functor, 'get_template_type' & specializations
  //---------------------------------------------------------------------------------------------- 
  template<typename T, typename... Args> struct get_type ;

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

  template<typename T>
  struct get_type<T> 
  { using value = typename T::value ; 
  } ;  

  //---------------------------------------------------------------------------------------------- 
  // Parameter template type extraction functor, 'get_template_type' & specializations
  //---------------------------------------------------------------------------------------------- 
  template<typename T, typename... Args> struct get_template_type ;

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
  // Boolean flag parameter extraction functor, 'get_flag' & specializations
  //---------------------------------------------------------------------------------------------- 
  template<typename T, typename... Args> 
  struct get_flag ;

  template<typename T, typename T_Next, typename... Args>
  struct get_flag<T, T_Next, Args...> 
    : std::integral_constant< bool
                            , std::is_same<T, T_Next>::value || get_flag<T, Args...>::value
                            > 
  {} ;

  template<typename T> 
  struct get_flag<T> 
    : std::false_type 
  {} ;

  //---------------------------------------------------------------------------------------------- 
  // Helper macros to simplify named argument declaration.
  //---------------------------------------------------------------------------------------------- 
  #define FPS_Declare_NTP_Value(NAME, TYPE) \
    struct NAME##_NTP_ID; \
    template< int64_t T_Default > \
    struct NAME \
      : ntp::Value<NAME##_NTP_ID, TYPE, T_Default> \
    {} ; 

  #define FPS_Declare_NTP_Flag(NAME) \
    struct NAME##_NTP_ID; \
    struct NAME \
      : ntp::Flag<NAME##_NTP_ID> \
    {} ; 

  #define FPS_Declare_NTP_Type(NAME) \
    struct NAME##_NTP_ID; \
    template< typename T> \
    struct NAME \
      : ntp::Type<NAME##_NTP_ID, T> \
    {} ; 
}}

#endif

