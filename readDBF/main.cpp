#include <iostream>
#include <fstream>
#include <vector>       // std::vector
#include <string.h>
#include <sstream>
#include <algorithm>

using namespace std;

class dbfheader {
public:
  dbfheader();
  vector<string> getFieldNames();
  vector<string> getFieldTypes();
  vector<int> getFieldWidth();
  vector<int> getFieldDecimals();

private:
  void readHeader();
  vector<string>fieldNames;
  vector<string>fieldTypes;
  vector<int>fieldWidth;
  vector<int>fieldDecimals;
};

// Constructor (reads header info into predefined arrays)
dbfheader::dbfheader(){
   void readHeader();
}

// FUNCTION: Read dbf field info
void dbfheader::readHeader(){
 // Currently in Main()
}


// FUNCTION: get field names
vector<string> dbfheader::getFieldNames(){
  return(fieldNames);
 }

// FUNCTION: get field types
vector<string> dbfheader::getFieldTypes(){
  return(fieldTypes);
 }

// FUNCTION: get field width
vector<int> dbfheader::getFieldWidth(){
  return(fieldWidth);
 }

// FUNCTION: get field decimals
vector<int> dbfheader::getFieldDecimals(){
  return(fieldDecimals);
 }

  // Function Prototype
  vector<string> split(const string &line, char delim);

  // To hold header information
  vector<string>fieldNames;
  vector<string>fieldTypes;
  vector<int>fieldWidth;
  vector<int>fieldDecimals;


int main()
{
    string ifileName;
    ifstream dbfFile;
    ofstream outfile;
    ofstream logfile;

    // User to enter file name
    cout << "Please enter the file name: ";
    cin >> ifileName;
    cout << "Processing file " << ifileName << endl;

    // Write output to csv file and summary to log file
    vector<string> fileNameParts = split(ifileName, '.');
    string dcbfName = fileNameParts[0]+".csv";
    string logfName = fileNameParts[0]+".log";
    const char* ofileName = dcbfName.c_str();
    const char* lfileName = logfName.c_str();

    cout << "Outfiles are: " << ofileName << " " << lfileName << endl;

    // Open input file
    dbfFile.open(ifileName,ios::in | ios::binary);
    outfile.open(ofileName);
    logfile.open(lfileName);

    // Define data types
    short sht2 = 0;
    int16_t bit16 = 0;
    int32_t bit32 = 0;
    char temp_string[11]; // Field Name is fixed in DBF at 10 characters
    char *string_value = nullptr;      // Record length could vary and so hold off

    // Field info
    int nfields = 0;
    int headerBytes = 0;
    int nrecords = 0;
    int recSize = 0;

    logfile <<   "----------  DBF File Info  ---------" << endl;
    // Read one byte at a time
    dbfFile.read((char*)&sht2, 1);  // 0-1: dBASE III PLUS {0,1}
    logfile << "0  : Version         : " << sht2 << endl;

    dbfFile.read((char*)&sht2, 3);  // 3: Get Date (YYMMDD)
    logfile << "1-3: Date            : " << sht2 << endl;

    dbfFile.read((char*)&bit32, 4); // 4-7: Number of records
    logfile << "4-7: Num of Recs     : " << bit32 << endl;
    nrecords = bit32;

    dbfFile.read((char*)&bit16, 2); // 8-9: Number of bytes in header (position of first record?)
    logfile << "8-9: Bytes in header : " << bit16 << endl;
    headerBytes = bit16;

    dbfFile.read((char*)&bit16, 2); // 10-11: Number of bytes in record (length of one data record)
    logfile << "10-11: Bytes in Record : " << bit16 << endl;
    recSize = bit16;

    dbfFile.read((char*)&bit32, 3); // 12-14: Reserved bytes
    logfile << "12-14: Reserved Bytes  : " << bit32 << endl;

    dbfFile.read((char*)&bit32, 13); // 15-27: dbase III LAN
    logfile << "15-27: Reserved LAN    : " << bit32 << endl;

    dbfFile.read((char*)&bit32, 4); // 28-31: Reserved
    logfile << "28-31: Reserved 3 bytes: " << bit32 << endl;
    logfile << "---------------------------------------" << endl;

    // In the header, first the 32 are the above bytes and last one is field terminatior
    // Each field into array is about 32 and nfields = (header bytes - 32 -1) / size of field info
    int temp = headerBytes - 33;
    nfields = temp/ 32;
    logfile << "Number of fields :" << nfields << endl;

    // Field Description starts here ---
    logfile <<   "----------  Field Header Info ---------" << endl;
    for (int i = 0; i < nfields; i++) {
      logfile << "---------------------------------------" << endl;

      dbfFile.read(temp_string,  11); // 32-64 : field info (zero filled)
      logfile << "0-10: Zero Filled     : " << temp_string << endl;
      fieldNames.push_back(temp_string);

      // Clear the array
      for(int s= 0; s < 11 ; s++){
          temp_string[s] = '\0';
      }

      dbfFile.read(temp_string,  1); // 32-64 : field info
      logfile << "11: Field Type        : " << temp_string << endl;
      fieldTypes.push_back(temp_string);

      dbfFile.read((char*)&bit32,  4); // 32-64 : field info
      logfile << "12-15: Reserved       : " << bit32 << endl;

      dbfFile.read((char*)&bit32,  1); // 32-64 : field info
      logfile << "16: Field Length      : " << bit32 << endl;
      fieldWidth.push_back(bit32);

      dbfFile.read((char*)&bit32,  1); // 32-64 : field info
      logfile << "17: Field decimal     : " << bit32 << endl;
      fieldDecimals.push_back(bit32);

      dbfFile.read((char*)&bit32,  2); // 32-64 : field info
      logfile << "18-19: Reserved       : " << bit32 << endl;

      dbfFile.read((char*)&bit32,  1); // 32-64 : field info
      logfile << "20: Work Area ID      : " << bit32 << endl;

      dbfFile.read((char*)&bit32,  10); // 32-64 : field info
      logfile << "20-30: Reserved       : " << bit32 << endl;

      dbfFile.read((char*)&bit32,  1); // 32-64 : field info
      logfile << "31: MDX Flag          : " << bit32 << endl;
      logfile << "---------------------------------------" << endl;
    }

      // Skip a byte (field array terminator)
      dbfFile.read(temp_string,  1);
      logfile<<" prefix data : " << temp_string[0] << endl;

      // Record information starts here
      logfile <<"Current Position before record data: " << dbfFile.tellg() << endl;
      dbfFile.read(temp_string,  1);
      logfile <<"Record Status: " << temp_string << endl;

      // Record length could vary by DBF file (as the number of fields increase)
      string_value = new char [recSize];

      int position[nfields];
      position[0] = 0;
      string sub_values = " ";

      // Print field header names
      for (int f=0; f< nfields; f++){
        if(f == nfields-1){
          outfile << fieldNames[f] << "\n";
          cout << fieldNames[f] << "\n";
        } else {
          outfile << fieldNames[f] << ",";
          cout << fieldNames[f] << "\n";
        }
      }

      // Loop by records and get data
      for (int r = 0; r < nrecords; r++){
        dbfFile.read(string_value,  recSize);

        sub_values=string_value;

        for (int f=0; f< nfields; f++){
            if (f>0) {
              position[f] = position[f-1]+fieldWidth[f-1];
            }
            // DEBUG:
            // cout << fieldNames[f] << " " << sub_values.substr(position[f], fieldWidth[f]) << endl;
            if(f == nfields-1){
              outfile << sub_values.substr(position[f], fieldWidth[f]) << "\n";
            } else {
              outfile << sub_values.substr(position[f], fieldWidth[f]) << ",";
            }
         }
      }

    dbfFile.close();
    outfile.close();
    logfile.close();
    delete[] string_value;
   // delete temp_string;

    cout << " ------------------------- " << endl;
    cout << " --- DBF file exported --- " << endl;
    cout << " ------------------------- " << endl;

}


// FUNCTION: Split string
vector<string> split(const string &line, char delim){
  string word;
  vector<string> fields;
  stringstream ss(line);
  while (getline(ss, word, delim)) {
    word.erase(remove(word.begin(), word.end(), '\"' ),word.end());
    fields.push_back(word);
  }
  return(fields);
}

