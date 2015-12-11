#ifndef FPS__UTIL__SIGNAL__H
#define FPS__UTIL__SIGNAL__H

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

namespace fps  {
namespace util {

  //------------------------------------------------------------------------------
  namespace signal
  {
    //----------------------------------------------------------------------------
    enum Enum
    {
      Sig_Int   = SIGINT,
      Sig_Child = SIGCHLD,
      Sig_Stop  = SIGSTOP,
      Sig_Cont  = SIGCONT,
      Sig_Usr1  = SIGUSR1,
      Sig_Usr2  = SIGUSR2,
      Sig_Term  = SIGTERM,
      Sig_Kill  = SIGKILL,
      Sig_Alarm = SIGALRM,
      Sig_Pipe  = SIGPIPE,
      Sig_Quit  = SIGQUIT,
      Sig_Hup   = SIGHUP,
      Sig_Abrt  = SIGABRT
    } ;

    //----------------------------------------------------------------------------
    inline 
    const char * 
    to_string( int32_t sig_num ) 
    {
      switch( sig_num ) 
      { 
        case Sig_Int   : return "SIGINT" ;
        case Sig_Child : return "SIGCHLD" ;
        case Sig_Stop  : return "SIGSTOP" ;
        case Sig_Cont  : return "SIGCONT" ;
        case Sig_Usr1  : return "SIGUSR1" ;
        case Sig_Usr2  : return "SIGUSR2" ;
        case Sig_Term  : return "SIGTERM" ;
        case Sig_Kill  : return "SIGKILL" ;
        case Sig_Alarm : return "SIGALRM" ;
        case Sig_Pipe  : return "SIGPIPE" ;
        case Sig_Quit  : return "SIGQUIT" ;
        case Sig_Hup   : return "SIGHUP" ;
        case Sig_Abrt  : return "SIGABRT" ;
        default        : return "Unknown" ;
      } 
    }

    //----------------------------------------------------------------------------
    inline 
    void 
    set_handler( Enum sig, void(*fn)(int) )
    { 
      ::signal( static_cast<int>( sig ), fn ) ;
    }

  } ;
}}

#endif

