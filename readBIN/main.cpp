#include <iostream>     // open() - log file
#include <fstream>      // open() - log file
#include <string.h>
#include <vector>       // std::vector
#include <iomanip>      // std::setw - log file
#include <time.h>       // system time
#include "DCBFile.h"    // dcb class file
#include <windows.h>    // windows libraries to browse

using namespace std;

// CLASS: Binary file contents
class BINFile
{
  public:
    BINFile(int nfields);
    void exportToCSV(string fname);
    void createFields(vector<string> type, vector<string> fldName);
    void addField(string type);
    void getField(string name);
    // void printFieldNumber();

  private:
    string fname;
    vector<string> string_field;
    vector<int> int_field;
    vector<float> float_field;
    vector<double> double_field;
};


void BINFile::createFields(vector<string> type, vector<string> fldName){
   for(int f=0; f < fldName.size(); f++){
        if(type[f] == "C"){
           vector<string>fldName[f];
        }else if(type[f]=="I"){
           vector<int>fldName[f];
        }else if(type[f]=="F"){
           vector<float>fldName[f];
        }else if(type[f]=="R"){
           vector<double>fldName[f];
        }else{
           cout<< "Error: invalid datatype in field " << fldName[f] << endl;
        }
    }
}

// Browse for file
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

//=============================================================================
//                   MAIN PROGRAM
//=============================================================================
  // Input file (make it a user entry)
  string file;
//  string file = "FBRMPO.bin";

  // Output not required but keep it in case of debug
  ofstream output;
  ofstream outfile;

  // TCAD NULL values are stored as:
  #define short_miss -32767
  #define long_miss  -2147483647
  #define flt_miss   -3.402823466e+38F
  #define dbl_miss   -1.7976931348623158e+308

  // Main program
  int main()
  {
      // Get system time
      time_t rawtime;
      time(&rawtime);

      // User to enter/browse file name
        file = GetFileName("Name of the file:");
//      cout << "Please enter the file name: ";
//      cin >> file;
//      cout << "Processing file " << file << endl;

      // Get field info
      DCBFile info;
      info.readDCBFile(file);
      string dcbFile = info.getDCBFileName(file);
      vector<string>tfieldNames = info.getFieldNames();
      vector<string>tfieldTypes = info.getFieldTypes();
      vector<int>tfieldBytes = info.getFieldBytes();
      vector<int>tfieldPos = info.getFieldPos();
      int total_fields = info.getNumFields();
      int field_size   = info.getFieldSize();

      // Write output to csv file and summary to log file
      vector<string> fileNameParts = info.split(file, '.');
      string dcbfName = fileNameParts[0]+".csv";
      string logfName = fileNameParts[0]+".log";
      const char* ofileName = dcbfName.c_str();
      const char* lfileName = logfName.c_str();

      // Print some info on the fields
      output.open(lfileName);
      output <<"==================================================================" << endl;
      output <<" Convert (BIN to CSV)                         " <<  ctime(&rawtime);
      output <<" Amar Sarvepalli, Sarvepalli@pbworld.com (Nov,2013)" << endl;
      output <<"==================================================================" << endl;
      output << "The binary file       : " << file         << endl;
      output << "The dictionary file   : " << dcbFile      << endl;
      output << "Total number of fields: " << total_fields << endl;
      output << "Total field bytes     : " << field_size   << endl;
      output <<"==================================================================" << endl;
      // Print field name, field type and size
      output << setw(3) << "Num" << setw(32) << "Field Name"  << setw(4) << "    Type";
      output << setw(4) << "  Size    " <<  setw(10)  <<"  Position" << endl;
      output <<"==================================================================" << endl;
      for (int i = 0; i <= total_fields-1; i++){
        output << setw(3) << i+1 << " " << setw(32) << tfieldNames[i] << "  ";
        output << setw(4) << tfieldTypes[i] << "  " << setw(4) << tfieldBytes[i];
        output << "    "  << setw(10) << tfieldPos[i] << endl;
      }
      output <<"==================================================================" << endl;

      // Open Binary file
      outfile.open(ofileName);
      ifstream binFile;
      ifstream::pos_type size;
      int max_stringlen = 100;
      char* memblock ;
      int nlines;
      short short_value;
      int interger_value;
      float float_value;
      double double_value;

       // Open file, get last position, read into block
       cout << file;
       const char* ifileName = file.c_str();
       binFile.open(ifileName, ios::in | ios::binary | ios::ate);
       // binFile.open(file, ios::in | ios::binary | ios::ate);

      // Read contents of binary file
      if (binFile.is_open()) {
          size = binFile.tellg();
          binFile.seekg (0, ios::beg);
          nlines = size/field_size ;

          // Write some stats on the file
          output <<"==================================================================" << endl;
          output << "Number of fields    :" << total_fields << endl;
          output << "Total size          :" << size         << endl;
          output << "Combined field size :" << field_size   << endl;
          output << "Number of Lines     :" << nlines       << endl;
          output <<"==================================================================" << endl;

          //Write the header
          for (int i = 0; i < total_fields; i++ ) {
            if (i == (total_fields-1)) {
                outfile << tfieldNames[i]<< "\n";
            }else {
                outfile << tfieldNames[i] << "," ;
            }
          }

          int line_pos = 0;

// Comment out
/*
          // Debug on a selected field:
            // int l = 8;           // Line number
            int i = 0;            // field number
            //string ftype = "C";    // field type
            //int fsize = 7;         // field size (width for characters)
            for (int l = 44; l < 45; l++){ // lines
              output<< "Current line position :" << line_pos << endl;
              output<< "Current field name    :" << tfieldNames[i] << endl;
              output<< "Current field type    :" << tfieldTypes[i] << endl;
              output<< "Current field byte    :" << tfieldBytes[i] << endl;
              output<< "Current field position:" << tfieldPos[i]   << endl;
              output<< "Current line          :" << l   << endl;
              line_pos = l * field_size + tfieldPos[i]-1;
              binFile.seekg(line_pos, ios::beg);
              output<< "Position set to       :" << line_pos << endl;
              if(tfieldTypes[i] == "C") {
                 memblock = new char [tfieldBytes[i]];
                 binFile.read(memblock, tfieldBytes[i]);
                 output <<"Field Value           :" << memblock << endl;
              }
              else if(tfieldTypes[i] == "R") {
                 binFile.read((char*)&double_value, 8);
                 output <<"Field Value           :" << float_value << endl;
              }
              else if(tfieldTypes[i] == "F") {
                 binFile.read((char*)&float_value, 4);
                 output <<"Field Value           :" << float_value << endl;
              }
              else if(tfieldTypes[i] == "I") {
                 // binFile.read((char*)&interger_value, 4);
                 // output <<"Field Value           :" << interger_value << endl;

                 // attempt to identify tcad deleted record
                 // deleted record int number results in val 1548389265
                 // deleted record real 2.63203E+272

                 memblock = new char [tfieldBytes[i]];
                 binFile.read(memblock, 1);
                 output <<"Field Value           :" << memblock << endl;
                 binFile.read((char*)&interger_value, 4);
                 output <<"Field Value           :" << interger_value << endl;
                 binFile.read(memblock, 36);
                 output <<"Field Value           :" << memblock << endl;
              }
              else if(tfieldTypes[i] == "S") {
                 binFile.read((char*)&short_value, 2);
                 output <<"Field Value          :" << short_value << endl;
              }
              // binFile.read(memblock, tfieldBytes[i]);
              // output <<"Read mem block data   :" << memblock << endl;
            } // Lines
*/
// comment ends

          // Loop by each line (since we know nlines, use for loop)
           for (int l = 0; l < nlines; l++){
          // DEBUG: for (int l = 697; l < 706; l++){
            // Loop by fields
            for (int i = 0; i < total_fields; i++ ) {

               // Set current field position (line no * max field size + cur_field start -1)
               line_pos = l * field_size + tfieldPos[i]-1;
               binFile.seekg(line_pos, ios::beg);

               if(tfieldTypes[i] == "C") {
                    memblock = new char [max_stringlen];
                    // clear previous values
                    for(int s= 0; s < max_stringlen ; s++){
                      memblock[s] = '\0';
                    }
                    binFile.read(memblock, tfieldBytes[i]);
                    if (i == total_fields-1) {
                        outfile << memblock << "\n";
                        // output << "Line : " << l+1 << " " <<"Field: " << setw(16) << tfieldNames[i] << "  "  << memblock << endl;
                        // cout << tfieldNames[i] <<" " <<  memblock << "\n";
                    }else {
                        outfile << memblock << ",";
                        // output << "Line : " << l+1 << " " <<"Field: " << setw(16) << tfieldNames[i] << "  " << memblock << endl;
                        // cout << tfieldNames[i] <<" "<< memblock << "\n";
                    }
                }

                // Field type is float
                else if(tfieldTypes[i] == "F") {
                    binFile.read((char*)&float_value, 4);
                    if(float_value == flt_miss){
                       float_value = 0;
                    }
                    if (i == (total_fields-1)) {
                        outfile << float_value << "\n";
                        // cout << tfieldNames[i] <<" "<< float_value << "\n";
                    }else {
                        outfile << float_value << ",";
                        // cout << tfieldNames[i] <<" "<< float_value << "\n";
                    }
                }

                // Field type is double
                else if(tfieldTypes[i] == "R") {
                    binFile.read((char*)&double_value, 8);
                    // Treat missing as zero
                    if(double_value == dbl_miss){
                       double_value = 0;
                    }
                    if (i == (total_fields-1)) {
                        outfile << double_value << "\n";
                        // cout << tfieldNames[i] <<" "<< double_value << "\n";
                    }else {
                        outfile << double_value << ",";
                        // cout<< tfieldNames[i] <<" " << double_value << "\n";
                    }
                  }

                // Field type is integer
                else if(tfieldTypes[i] == "I") {
                    binFile.read((char*)&interger_value, 4);
                    // Treat missing as zero
                    if(interger_value == long_miss){
                       interger_value = 0;
                    }
                    if (i == (total_fields-1)) {
                        outfile << interger_value << "\n";
                        // cout << tfieldNames[i] <<" "<< interger_value << "\n";
                    }else {
                        outfile << interger_value << ",";
                        // cout << tfieldNames[i] <<" "<< interger_value << "\n";
                    }
                  }
                // Field type is short
                else if(tfieldTypes[i] == "S") {
                    binFile.read((char*)&short_value, 2);
                    // Treat missing as zero
                    if(short_value == short_miss){
                       short_value = 0;
                    }
                    if (i == (total_fields-1)) {
                        outfile << short_value << "\n";
                        // cout << tfieldNames[i] <<" " << interger_value << "\n";
                    }else {
                        outfile << short_value << ",";
                        // cout << tfieldNames[i] <<" "<< interger_value << "\n";
                    }
                 } // types

              } // fields
          } // end line loop

          // Write message to screen
          cout << endl;
          cout <<"==============================================================" << endl;
          cout <<"        Export Complete !!!!       " << endl;
          cout << "File " << file << " of size " << size << " is exported to " << ofileName << endl;
          cout <<"==============================================================" << endl;
          //=========================================================================================
      }
      else {
          cout << "Unable to read file " << file <<endl;
      } // end File

      delete[] memblock;
      binFile.close();
      outfile.close();
      output.close();
    }
//=============================================================================
