//to complie: >g++ browse_for_file.cpp libcomdlg32 -o browse

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;

string  GetFileName( const string & prompt ) { 
    const int BUFSIZE = 1024;
    char buffer[BUFSIZE] = {0};
    OPENFILENAME ofns = {0};
    ofns.lStructSize = sizeof( ofns );
    ofns.lpstrFile = buffer;
    ofns.nMaxFile = BUFSIZE;
    ofns.lpstrTitle = prompt.c_str();
    GetOpenFileName( & ofns );
    return buffer;
}
int main( int argc, char *argv[] ) {

  string fname = GetFileName( "Number which file: " );
  cout << fname;
  
  /*
  ifstream ifs( fname.c_str() );
  if ( ! ifs.is_open() ) {
    cerr << "cannot open " << fname << " for input" << endl;
  }
  else {
    string line;
    int lineno = 1;
    while( getline( ifs, line ) ) {
      cout << setw(5) << right << lineno++ << " : " << line << "\n";
    }
  }
  */
}