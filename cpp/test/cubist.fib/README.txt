CODE EXAMPLE : Fib Series Calculation 

  BUILD
    To build the solution, create an empty directory, copy the .cpp file into it, and run g++.

    $> mkdir bwoolbright.cubist_test 
    $> cp cubist.fib.cpp bwoolbright.cubist_test 
    $> cd bwoolbright.cubist_test
    $> g++ -std=c++17 -O3 -o cubist.fib *.cpp

    This will create an executable file called cubist.fib in the current directory. 
    
  RUN 
    The example program accepts three types of argument strings :

    To print usage text :
    $> ./cubist.fib --help (or -h)

    To run internal test routines :
    $> ./cubist.fib test 

    To calculate a single fib series value pass any number between 0 and 47 :
    $> ./cubist.fib [ 0, 47 ]

    (Since the example uses 32 bit integers to store result values, 47 is the maximum 
     series member that the application accepts as an argument.)

  PERFORMANCE :
    
    I've appended the output from the internal test routine ( ./cubist.fib test ) below.  
    The output clearly shows that iterative solution has linear time complexity requiring
    N-1 additions to compute fib values for non-degenerate cases.

    The recursive implementation is far less efficient.  For non-degenerate cases it exhibits 
    exponential time complexity requiring T( n-1 ) + T( n-2 ) additions to compute any 
    fib series member n.

    [ Test fib_iterative() ]
      fib_iterative( 0 ) => 0 (adds: 0)
      fib_iterative( 1 ) => 1 (adds: 0)
      fib_iterative( 2 ) => 1 (adds: 1)
      fib_iterative( 3 ) => 2 (adds: 2)
      fib_iterative( 4 ) => 3 (adds: 3)
      fib_iterative( 5 ) => 5 (adds: 4)
      fib_iterative( 6 ) => 8 (adds: 5)
      fib_iterative( 15 ) => 610 (adds: 14)
      fib_iterative( 30 ) => 832040 (adds: 29)
      fib_iterative( 47 ) => 2971215073 (adds: 46)
    [ Pass ]

    [ Test fib_recursive() ]
      fib_recursive( 0 ) => 0  (adds: 0)
      fib_recursive( 1 ) => 1  (adds: 0)
      fib_recursive( 2 ) => 1  (adds: 1)
      fib_recursive( 3 ) => 2  (adds: 2)
      fib_recursive( 4 ) => 3  (adds: 4)
      fib_recursive( 5 ) => 5  (adds: 7)
      fib_recursive( 6 ) => 8  (adds: 12)
      fib_recursive( 15 ) => 610  (adds: 986)
      fib_recursive( 30 ) => 832040  (adds: 1346268)
      fib_recursive( 47 ) => 2971215073  (adds: 512559679)
    [ Pass ]

  Thanks for your time, I enjoied our discussion.

  -Blake

