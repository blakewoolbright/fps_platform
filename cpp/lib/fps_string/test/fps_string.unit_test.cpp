#define BOOST_TEST_MODULE fps_string__core__test

#include "fps_string/fps_string.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>

using namespace fps ;

//-------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_string__convert ) 
{
  std::cout << "[ fps::string | to_<primitive> tests ]" << std::endl 
            << "|" << std::endl ;

  std::vector<std::string> s_vec = { "0", "1", "10", "100", "-1"
                                   , "2147483647", "-2147483646"
                                   , "21474836470"       
                                   } ;

  std::vector<int64_t> v_vec = { 0, 1, 10, 100, -1, 2147483647, -2147483646, 21474836470 } ;
  for( uint32_t idx = 0 ; idx < s_vec.size() ; ++idx ) 
  {
    const std::string & s_ref = s_vec[idx] ;

    int16_t i16 = string::convert::to<int16_t>( s_ref.c_str(), s_ref.length() ) ; 
    int32_t i32 = string::convert::to<int32_t>( s_ref.c_str(), s_ref.length() ) ; 
    int64_t i64 = string::convert::to<int64_t>( s_ref.c_str(), s_ref.length() ) ; 
  
    std::cout << "|--[ string::convert::to<>( " << s_ref << " ) ]" << std::endl 
              << "|  |--{ int16 : " << i16 << " }" << std::endl
              << "|  |--{ int32 : " << i32 << " }" << std::endl
              << "|  |--{ int64 : " << i64 << " }" << std::endl
              << "|" << std::endl ;

    BOOST_CHECK_MESSAGE( i64 == v_vec[ idx ]
                       , string::sprintf( "string::convert::to -> Mismatch %s != %ld"
                                        , s_ref.c_str()
                                        , i64 
                                        ).c_str()
                       ) ;

    static const uint32_t buf_sz = 128 ;
    char     u_buf[ buf_sz ] ;
    char     s_buf[ buf_sz ] ;
    uint32_t buf_len = buf_sz - 1 ;

    std::memset( u_buf, 0, buf_sz ) ;
    std::memset( s_buf, 0, buf_sz ) ;
    uint64_t u64 = ( i64 < 0 ? -i64 : i64 ) ;

    int32_t u64_rv = string::convert::write_unsigned( u64, u_buf, buf_len ) ;
    int32_t i64_rv = string::convert::write_signed  ( i64, s_buf, buf_len ) ;
    std::cout << "|--[ string::convert::write_unsigned( " << s_ref << " ) ]" << std::endl 
              << "|  |--[ result len : " << u64_rv << " ]" << std::endl 
              << "|  |--[ result str : " << u_buf  << " ]" << std::endl 
              << "|" << std::endl ;
            
    std::cout << "|--[ string::convert::write_signed( " << s_ref << " ) ]" << std::endl 
              << "|  |--[ result len : " << i64_rv << " ]" << std::endl 
              << "|  |--[ result str : " << s_buf  << " ]" << std::endl 
              << "|" << std::endl ;

    BOOST_CHECK_MESSAGE( u64_rv > 0
                       , string::sprintf( "string::convert::write_unsigned -> Return value %d <= 0", u64_rv ) 
                       ) ;

    BOOST_CHECK_MESSAGE( i64_rv > 0
                       , string::sprintf( "string::convert::write_signed -> Return value %d <= 0", u64_rv ) 
                       ) ;
  }

  double      v_dbl_1 = -123456789.12345 ;
  std::string s_dbl_1 = "-123456789.12345" ;
  double      d_dbl_1 = string::convert::to<double>( s_dbl_1.c_str(), s_dbl_1.size() ) ;
  float       f_dbl_1 = string::convert::to<float> ( s_dbl_1.c_str(), s_dbl_1.size() ) ;

  double      v_dbl_2 = 0.123456789 ;
  std::string s_dbl_2 = "0.123456789" ;
  double      d_dbl_2 = string::convert::to<double>( s_dbl_2.c_str(), s_dbl_2.size() ) ;
  float       f_dbl_2 = string::convert::to<float> ( s_dbl_2.c_str(), s_dbl_2.size() ) ;

  double      v_dbl_3 = -0.123456789 ;
  std::string s_dbl_3 = "-0.123456789" ;
  double      d_dbl_3 = string::convert::to<double>( s_dbl_3.c_str(), s_dbl_3.size() ) ;
  float       f_dbl_3 = string::convert::to<float> ( s_dbl_3.c_str(), s_dbl_3.size() ) ;

  std::cout << "|--[ string::to_double( " << s_dbl_1 << " ) ]" << std::endl 
            << "|  |--{ double : " << d_dbl_1 << " }" << std::endl
            << "|  |--{ float  : " << f_dbl_1 << " }" << std::endl
            << "|" << std::endl ;

  std::cout << "|--[ string::to_double( " << s_dbl_2 << " ) ]" << std::endl 
            << "|  |--{ double : " << d_dbl_2 << " }" << std::endl
            << "|  |--{ float  : " << f_dbl_2 << " }" << std::endl
            << "|" << std::endl ;

  std::cout << "|--[ string::to_double( " << s_dbl_3 << " ) ]" << std::endl 
            << "|  |--{ double : " << d_dbl_3 << " }" << std::endl
            << "|  |--{ float  : " << f_dbl_3 << " }" << std::endl
            << "|" << std::endl ;

  BOOST_CHECK_MESSAGE( v_dbl_1 == d_dbl_1
                     , string::sprintf( "fps::string -> Conversion error %d != %d"
                                      , v_dbl_1, d_dbl_1 
                                      ).c_str()
                     ) ;

  BOOST_CHECK_MESSAGE( v_dbl_2 == d_dbl_2
                     , string::sprintf( "fps::string -> Conversion error %d != %d"
                                      , v_dbl_2, d_dbl_2 
                                      ).c_str()
                     ) ;

  BOOST_CHECK_MESSAGE( v_dbl_3 == d_dbl_3
                     , string::sprintf( "fps::string -> Conversion error %d != %d"
                                      , v_dbl_3, d_dbl_3 
                                      ).c_str()
                     ) ;

  std::cout << "|--[ Result : Pass ]" << std::endl << std::endl << std::flush ;
}

//-------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_string__format ) 
{
  std::cout << "[ Test : fps::string::{format,append} ]" << std::endl 
            << "|" << std::endl ;

  std::string s1 = "abcdefg" ; 
  string::format( s1, "%-20s | %20s", "String Arg 1", "String Arg 2" ) ;

  BOOST_CHECK_MESSAGE( s1.size() == ( 20 * 2 + 3 )
                     , "string::format returned string with wrong length"
                     ) ;

  std::string s2 = s1 ;
  string::append( s2, ".%s( %u )", "append", 123 ) ;
  
  BOOST_CHECK_MESSAGE( s2.size() == ( 20 * 2 + 3 ) + ( strlen( "append" ) + 3 + 5 )
                     , "string::append returned string with wrong length"
                     ) ;
  std::cout << "|--[ Result : Pass ]" << std::endl << std::endl << std::flush ;
}

//-------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( fps_string__trim ) 
{
  std::cout << "[ Test : fps::string::*trim* ]" << std::endl 
            << "|" << std::endl ;

  std::string s1            = "  abc efg hij    " ;
  std::string s1_l_stripped = string::l_stripped( s1 ) ;
  std::string s1_r_stripped = string::r_stripped( s1 ) ;
  std::string s1_stripped   = string::stripped  ( s1 ) ;

  std::cout << "|--[ '" << s1 << "' ]" << std::endl 
            << "|  |--{ l_stripped : '" << s1_l_stripped << "' }" << std::endl 
            << "|  |--{ r_stripped : '" << s1_r_stripped << "' }" << std::endl 
            << "|  |--{   stripped : '" << s1_stripped   << "' }" << std::endl 
            << "|" << std::endl ;

  BOOST_CHECK_MESSAGE( s1_l_stripped.size() == (s1.size() - 2)
                     , string::sprintf( "string::l_stripped => '%s' not left stripped", s1_l_stripped.c_str() ).c_str() 
                     ) ;

  BOOST_CHECK_MESSAGE( s1_r_stripped.size() == (s1.size() - 4)
                     , string::sprintf( "string::r_stripped => '%s' not right stripped", s1_r_stripped.c_str() ).c_str() 
                     ) ;

  BOOST_CHECK_MESSAGE( s1_stripped.size() == (s1.size() - (4 + 2))
                     , string::sprintf( "string::stripped => '%s' not stripped", s1_stripped.c_str() ).c_str() 
                     ) ;

  std::cout << "|--[ Result : Pass ]" << std::endl << std::endl << std::flush ;
}

