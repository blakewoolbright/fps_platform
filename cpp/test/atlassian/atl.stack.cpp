#include <bits/stdc++.h>
#include <limits>
#include <stdexcept>

using namespace std;


class IStack
{
public :
    typedef std::vector<int32_t> int_vec_t ;
    int_vec_t v_ ;

    IStack() : v_() { v_.reserve( 1000 ) ; }
    IStack( std::size_t capacity ) : v_() { v_.reserve( capacity + 1 ) ; } 
    ~IStack() {} 
    
    std::size_t size() const { return v_.size() ; } 
    bool        empty() const { return v_.empty() ; }

    typedef int_vec_t::reverse_iterator iterator ;
        iterator begin() { return v_.rbegin() ; } 
    iterator end  () { return v_.rend() ; }
    
    
    
    void 
    push( int32_t val )
    { 
        v_.push_back( val ) ; 
    }
    
    int32_t 
    pop() 
    {
        int32_t rv = v_[ v_.size()-1 ] ;
        v_.pop_back() ;
        return rv ;
    }
    
    int32_t 
    top() const 
    {
        return v_.empty() ? std::numeric_limits<int32_t>::max() : v_[ v_.size() - 1 ] ;
    }
    
    void 
    inc_e( std::size_t count, int32_t increment )
    {
        if( count > v_.size() )
           throw std::runtime_error( "oops - bad count" ) ;
           
        for( std::size_t idx = 0 ; idx < count ; ++idx )
            v_[ idx ] += increment ;
    }
    
} ;

/*
void test()
{
    IStack s ;
    s.push( 1 ) ;
    s.push( -1 ) ;
    s.push( 100 ) ;
    
    
    for( auto itr = s.begin() ; itr != s.end() ; ++itr )
        std::cout << "  " << *itr << std::endl ;
    
    std::cout << "[ increment( 2,1 ) ] " << std::endl ;
    s.inc_e( 2, 1 ) ;
    for( auto itr = s.begin() ; itr != s.end() ; ++itr )
        std::cout << "  " << *itr << std::endl ;
 }
*/

void 
superStack(vector <string> operations) 
{
    IStack s( operations.size() ) ;
    for( std::size_t idx = 0 ; idx < operations.size() ; ++idx )
    {
        std::string op = operations[ idx ] ;
        // std::cout << "> " << op << std::endl ;
        
        int32_t find_rv = op.find( "push" ) ;
        if( find_rv < op.size() )
        {
            int32_t arg_pos = op.rfind( " " ) ;
            if( arg_pos >= op.size() )
              throw std::runtime_error( "push - No argument" ) ;
            
            int32_t arg_val = std::atoi( op.c_str() + arg_pos ) ;
            s.push( arg_val ) ;
        }
        
        else if( op.find( "pop" ) < op.size() )
        {
            s.pop() ;
        }
        
        else
        { 
            int32_t arg_pos = op.rfind( " " ) ;
            int32_t k_val   = std::atoi( op.c_str() + arg_pos ) ;
            
            arg_pos = op.rfind( " ", arg_pos-1 ) ;
            std::size_t e_val = std::atoll( op.c_str() + arg_pos ) ;
            
            s.inc_e( e_val, k_val ) ;
        }
        
        // for( auto itr = s.begin() ; itr != s.end() ; ++itr )
        //    std::cout << "  " << *itr << std::endl ;

        if( s.empty() )
            std::cout << "EMPTY" << std::endl ;
        else 
            std::cout << s.top() << std::endl ;        
    }
}

int 
main()
{
    // test() ;
    // return 0 ;
    
    int operations_size = 0;
    cin >> operations_size;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    vector<string> operations;
    for(int i = 0; i < operations_size; i++) 
    {
        string operations_item;
        getline(cin, operations_item);
        operations.push_back(operations_item);
    }

    superStack(operations);
    
    return 0;
}

