
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

string  GetFileName( const string & prompt ) {
  cout << prompt;
  string name;
  getline(cin, name );
  return name;
}

int main( int argc, char *argv[] ) {

  string fname = GetFileName( "Number which file: " );
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
}