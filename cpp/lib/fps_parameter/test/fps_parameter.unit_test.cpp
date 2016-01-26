#define BOOST_TEST_MODULE fps__parameter 

#include "fps_parameter/fps_parameter.h"
#include <boost/test/unit_test.hpp>

using namespace fps ;

namespace test 
{
  /*
  enum class ParameterID
  { Capacity = 1 
  , Reverse  = 2
  } ;
  */
  
  namespace options 
  {
    //--------------------------------------------------------------
    struct ID_Capacity ;
    struct ID_Reverse ; 
    struct ID_Weight ;
    struct ID_Type ;

    //--------------------------------------------------------------
    template<uint32_t T_Default>
    struct Capacity 
      : parameter::Value<ID_Capacity, uint32_t, T_Default>
    {} ;
    
    /*
    //--------------------------------------------------------------
    struct Reverse
      : parameter::flag<ID_Reverse>
    {} ;

    //--------------------------------------------------------------
    template<int32_t T_Default>
    struct Weight 
      : parameter::value<ID_Weight, int32_t, T_Default>
    {} ;

    //--------------------------------------------------------------
    template<typename T>
    struct Type 
      : parameter::type<ID_Type, T>
    {} ;
    */
  }

  //--------------------------------------------------------------
  template<typename... Args>
  struct Example 
  {
    // static constexpr const auto Type = parameter::get_type<
    static constexpr const auto 
    Capacity = parameter::get_value< options::Capacity<1024>, Args... >::value ;
  } ;
}


BOOST_AUTO_TEST_CASE( fps_parameter__core ) 
{
  std::cout << "[ fps::parameter unit tests ]" << std::endl ;
    
  static const uint64_t Capacity_1 = 666 ;

  typedef test::Example<>                                        ex_0_t ;
  typedef test::Example< test::options::Capacity< Capacity_1 > > ex_1_t ;

  ex_0_t ex_0 ;
  ex_1_t ex_1 ;

  std::cout << "|--[ ex_0.Capacity : " << ex_0.Capacity << " ]" << std::endl ;
  std::cout << "|--[ ex_1.Capacity : " << ex_1.Capacity << " ]" << std::endl ;

  std::cout << std::endl ;
}
