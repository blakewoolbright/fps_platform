#define BOOST_TEST_MODULE fps__ntp 

#include "fps_ntp/fps_ntp.h"
#include "fps_string/fps_string.h"
#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace fps ;

namespace opt 
{
  FPS_Declare_NTP_Value( Capacity, uint32_t ) ;
  FPS_Declare_NTP_Value( Reverse,  bool ) ;
  FPS_Declare_NTP_Value( Weight,   int32_t ) ;
  FPS_Declare_NTP_Flag ( Debug ) ;
  FPS_Declare_NTP_Type ( Type ) ;
}

//-----------------------------------------------------------------------------------------------
template<typename T, typename... Args>
struct Example 
{
  static const uint32_t Default_Capacity = 987654321 ;
  static const bool     Default_Reverse  = false ;
  static const int32_t  Default_Weight   = -1024 ;

  using Type = typename ntp::get_type<opt::Type<T>, Args...>::value ;

  static const uint32_t Capacity = ntp::get_value<opt::Capacity<Default_Capacity>, Args...>::value ;
  static const bool     Reverse  = ntp::get_value<opt::Reverse <Default_Reverse>,  Args...>::value ;
  static const int32_t  Weight   = ntp::get_value<opt::Weight  <Default_Weight>,   Args...>::value ;
  static const bool     Debug    = ntp::get_flag <opt::Debug, Args...>::value ;
} ;

//-----------------------------------------------------------------------------------------------
template<typename T>
void
to_stdout( const T & obj, const std::string & lbl ) 
{
  std::cout << "|--[ " << lbl << ".Capacity : " << obj.Capacity << " ]" << std::endl 
            << "|--[ " << lbl << ".Weight   : " << obj.Weight   << " ]" << std::endl 
            << "|--[ " << lbl << ".Reverse  : " << (obj.Reverse ? "true" : "false") << " ]" << std::endl 
            << "|--[ " << lbl << ".Debug    : " << (obj.Debug ? "true" : "false") << std::endl 
            << std::endl ;    

}

//-----------------------------------------------------------------------------------------------
template<typename T, typename T_Type = uint64_t>
inline
void
validate( const T & obj, uint32_t capacity, int32_t weight, bool reverse, const std::string & lbl ) 
{
  static const bool Type_OK = std::is_same< T_Type, typename T::Type >::value ;

  BOOST_CHECK_MESSAGE
  ( Type_OK
  , string::sprintf( "\n\t%s :: Argument 'Type' invalid ( Sizes : %u != %u )"
                   , lbl.c_str(), sizeof( T_Type ), sizeof( typename T::Type )
                   ) 
  ) ;


  BOOST_CHECK_MESSAGE
  ( obj.Capacity == capacity 
  , string::sprintf( "\n\t%s :: Argument 'Capacity' invalid (%u != %u)"
                   , lbl.c_str(), obj.Capacity, capacity 
                   ) 
  ) ;

  BOOST_CHECK_MESSAGE
  ( obj.Weight == weight 
  , string::sprintf( "\n\t%s :: Argument 'Weight' invalid (%lu != %lu)"
                   , lbl.c_str(), obj.Weight, weight 
                   ) 
  ) ;

  BOOST_CHECK_MESSAGE
  ( obj.Reverse == reverse 
  , string::sprintf( "\n\t%s :: Argument 'Reverse' invalid (%lu != %lu)"
                   , lbl.c_str(), obj.Weight, weight 
                   ) 
  ) ;
}

//-----------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_ntp__core ) 
{
  std::cout << "[ fps::ntp unit tests ]" << std::endl ;
    
  typedef uint16_t Type_0 ;
  typedef uint64_t Type_1 ;
  const   uint32_t Capacity_1 = 666 ;
  const   uint32_t Capacity_2 = 12345 ;
  const   int32_t  Weight_2   = -12345 ;
  const   bool     Reverse_2  = true ;
  typedef int32_t  Type_2 ;

  typedef Example< Type_0 >                            ex_0_t ;
  typedef Example< Type_1, opt::Capacity<Capacity_1> > ex_1_t ;
  typedef Example< Type_2
                 , opt::Capacity<Capacity_2>
                 , opt::Weight  <Weight_2>
                 , opt::Reverse <Reverse_2>
                 , opt::Debug
                 > ex_2_t ;

  ex_0_t ex_0 ;
  ex_1_t ex_1 ;
  ex_2_t ex_2 ;

  to_stdout( ex_0, "Example[0]" ) ;
  validate<ex_0_t, Type_0>( ex_0, ex_0.Default_Capacity, ex_0.Default_Weight, ex_0.Default_Reverse, "Example[0]" ) ;
  
  to_stdout( ex_1, "Example[1]" ) ;
  validate<ex_1_t, Type_1>( ex_1, Capacity_1, ex_1.Default_Weight, ex_1.Default_Reverse, "Example[1]" ) ;

  to_stdout( ex_2, "Example[2]" ) ;
  validate<ex_2_t, Type_2> ( ex_2, Capacity_2, ex_2.Weight, ex_2.Reverse, "Example[2]" ) ;

  std::cout << std::endl ;
}
