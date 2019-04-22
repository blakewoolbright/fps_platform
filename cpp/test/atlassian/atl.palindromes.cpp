#include <cstring>
#include <cstdlib>
#include <iostream>


// Returns total number of palindrome substring of 
// length greater then equal to 2 
int count_palidromic_substrings(const char * str, int n) 
{ 
  // creat empty 2-D matrix that counts all palindrome 
  // substring. dp[i][j] stores counts of palindromic 
  // substrings in st[i..j] 
  int dp[n][n]; 
  std::memset(dp, 0, sizeof(dp)); 

  // P[i][j] = true if substring str[i..j] is palindrome, 
  // else false 
  bool P[n][n]; 
  std::memset(P, false , sizeof(P)); 

  // palindrome of single lenght 
  for (int i= 0; i< n; i++) 
      P[i][i] = true; 

  // palindrome of length 2 
  for (int i=0; i<n-1; i++) 
  { 
    if (str[i] == str[i+1]) 
    { 
        P[i][i+1] = true; 
        dp[i][i+1] = 1 ; 
    } 
  } 

  // Palindromes of length more than 2. This loop is similar 
  // to Matrix Chain Multiplication. We start with a gap of 
  // length 2 and fill the DP table in a way that gap between 
  // starting and ending indexes increases one by one by 
  // outer loop. 
  for (int gap=2 ; gap<n; gap++) 
  { 
    // Pick starting point for current gap 
    for (int i=0; i<n-gap; i++) 
    { 
        // Set ending point 
        int j = gap + i; 

        // If current string is palindrome 
        if (str[i] == str[j] && P[i+1][j-1] ) 
            P[i][j] = true; 

        // Add current palindrome substring ( + 1) 
        // and rest palinrome substring (dp[i][j-1] + dp[i+1][j]) 
        // remove common palinrome substrings (- dp[i+1][j-1]) 
        if (P[i][j] == true) 
            dp[i][j] = dp[i][j-1] + dp[i+1][j] + 1 - dp[i+1][j-1]; 
        else
            dp[i][j] = dp[i][j-1] + dp[i+1][j] - dp[i+1][j-1]; 
    } 
  } 

  // return total palindromic substrings 
  return dp[0][n-1]; 
} 

int main( int argc, char * argv[] ) 
{ 
  if( argc < 2 ) 
  { std::cout << "USAGE: " << argv[0] << " <input string>" << std::endl << std::endl ;
    return 0 ;
  }
  
  for( int32_t idx = 1 ; idx < argc ; ++idx ) 
  {
    std::size_t n = std::strlen(argv[idx]); 
    int32_t rv = count_palidromic_substrings( argv[idx], static_cast<int32_t>(n) ) ;
    std::cout << "  " << argv[idx] << " : " << rv << std::endl ;
  }
  std::cout << std::endl ;
  return 0; 
} 

