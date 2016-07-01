#include <windows.h>
int main( int argc, char *argv[] ) {
  MessageBox( 0, 
              argc > 1 ? argv[1] : "",
              argc > 2 ? argv[2] : "Message" , 0 );
}