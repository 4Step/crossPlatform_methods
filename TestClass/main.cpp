#include <iostream>     // open() - log file
#include <fstream>      // open() - log file
#include <string.h>
#include <vector>       // std::vector
#include <iomanip>      // std::setw - log file
#include <time.h>       // system time
#include "DCBFile.h"

using namespace std;

// CLASS: Binary file contents
class BINFile
{
  public:
    BINFile(int nfields);
    void createFields(vector<string> type, vector<string> fldName);
    void addField(string type);
    void getField(string name);
    // void printFieldNumber();

  private:
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

//=============================================================================
//                   MAIN PROGRAM
//=============================================================================
  // Input file (make it a user entry)
  string file;
  // string file = "tsz2030.bin";

  // Outputs not required but keep it in case of debug
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

      // User to enter file name
      cout << "Please enter the file name: " << endl;
      cin >> file;
      cout << "Processing file " << file << endl;

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
      int max_stringlen = 30;
      char* memblock ;
      int nlines ;
      int interger_value;
      float float_value;
      double double_value;

       // Open file, get last position, read into block
       binFile.open(file,ios::in | ios::binary | ios::ate);

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
       /*
          // Debug on a selected field:
            // int l = 8;           // Line number
            int i = 11;            // field number
            string ftype = "C";    // field type
            int fsize = 7;         // field size (width for characters)
            for (int l = 0; l < 10; l++){
              output<< "Current line position :" << line_pos << endl;
              output<< "Current field name    :" << tfieldNames[i] << endl;
              output<< "Current field type    :" << tfieldTypes[i] << endl;
              output<< "Current field byte    :" << tfieldBytes[i] << endl;
              output<< "Current field position:" << tfieldPos[i]   << endl;
              line_pos = l * field_size + tfieldPos[i]-1;
              binFile.seekg(line_pos, ios::beg);
              output<< "Position set to       :" << line_pos << endl;
              memblock = new char [tfieldBytes[i]];
              binFile.read(memblock, tfieldBytes[i]);
              output <<"Read mem block data   :" << memblock << endl;
              }
      */

          // Loop by each line (since we know nlines, use for loop)
          for (int l = 0; l < nlines; l++){
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
                        // cout << memblock << "\n";
                    }else {
                        outfile << memblock << ",";
                        // output << "Line : " << l+1 << " " <<"Field: " << setw(16) << tfieldNames[i] << "  " << memblock << endl;
                        // cout << memblock << "\n";
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
                        // cout << float_value << "\n";
                    }else {
                        outfile << float_value << ",";
                        // cout << float_value << "\n";
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
                        // cout << double_value << "\n";
                    }else {
                        outfile << double_value << ",";
                        // cout << double_value << "\n";
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
                        // cout << interger_value << "\n";
                    }else {
                        outfile << interger_value << ",";
                        // cout << interger_value << "\n";
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
