#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

  string ifile = "MicroCodedHnet42.NET";
  string ofile = "MicroCodedHnet42.CSV";
  ifstream binFile;

  // Output not required but keep it in case of debug
  ofstream output;
  char* memblock ;

  short short_value;
  int interger_value;

int main()
{

    binFile.open(ifile,ios::in | ios::binary | ios::ate);
    output.open(ofile);

    if (binFile.is_open()) {
      ifstream::pos_type size;
      size = binFile.tellg();
      cout << "File size is: "<<  size << endl;
      binFile.seekg (0, ios::beg);
      int max_size= 2 ;                    // First 155 bytes contains fileName

      memblock = new char [max_size];
      binFile.read(memblock, max_size);
      cout << "First " << max_size << " Characters: " << memblock << endl;

     //  int npaths = (size-160)/8 ;

      binFile.read((char*)&interger_value, 4);
      cout << "Next 4 : " << interger_value << endl;
      binFile.read((char*)&interger_value, 4);
      cout << "Next 4 : " << interger_value << endl << endl;
/*
      for (int i = 0; i <npaths  ; i++) {
        binFile.read((char*)&interger_value, 4);
        output << i+1 <<","  << interger_value ;
        binFile.read((char*)&interger_value, 4);
        output << ","  << interger_value << endl;
      }
*/
    } else {
      cout << "Unable to open the file: " << ifile << endl;
    }

    delete[] memblock;
    binFile.close();
    output.close();

}
