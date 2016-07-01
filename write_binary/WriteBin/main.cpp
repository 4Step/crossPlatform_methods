#include <iostream>     // open() - log file
#include <fstream>      // open() - log file
#include <string.h>
#include <vector>       // std::vector
#include <time.h>       // system time
#include <sstream>      // string stream
#include <algorithm>    // std::remove
#include <windows.h>    // windows libraries to browse
#include <cstdlib>      // to address compiler issue
#include <stdio.h>

using namespace std;


 /*! \brief  Method to convert csv file to transcad binary file.
  *  Auto generates filenames based on input file.
  *  Also auto generates associated .DCB file.
  * \param csv filename
  * \param
  * \return binary file
  *
  */


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

// Windows browse for file
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

class FieldType {
  string field_s;
  int field_i;
  double field_d;
};

//=============================================================================
//                   MAIN PROGRAM
//=============================================================================
  // Input file (make it a user entry)
  string file;
  vector<string> rowinfo;
  vector<string> fieldtype;
  vector<string> fieldname;
  vector<int> fieldwidth;
  vector<int> fieldbyte;
  vector<int> fieldposition;
  vector<int> fielddecimal;
  vector<bool> checkheaderFields;
  // vector<FieldType> datatable;
  vector< vector <string> > datatable;

  string line;
  int maxLineCheck = 1000000; // maximum lines to determine data type & write method
  int pos = 0;
  double test_d;
  int test_i;
  const char* test_s;
  int current_id = 0;
  int line_bin = 1000;

   // I/O files
  ofstream headerfile;
  ofstream binfile;
  ifstream infile;


  // TCAD NULL values are stored as:
  #define short_miss -32767
  #define long_miss  -2147483647
  #define flt_miss   -3.402823466e+38F
  #define dbl_miss   -1.7976931348623158e+308

    // Prototypes
    vector<string> split(const string &line, char delim, char remElement);
    bool is_number(const std::string& s);
    bool is_integer(const std::string & s);
    bool contains_alpha(const std::string& s);
    int get_width(const std::string& s);
    int get_decimals(const std::string& s);
    bool has_quotes(const std::string& s);

int main()
{
        FILE* pFile;

        // User to enter/browse file name
        file = GetFileName("Name of the file:");
        cout << file<< endl;

        // Get system time
        time_t rawtime;
        time(&rawtime);

        // create output files
        vector<string> fileNameParts = split(file, '.', ' '); // remove any white space
        string dcbfName = fileNameParts[0]+".DCB";
        string binfName = fileNameParts[0]+".bin";

//      // To address compiler issues (convert string to c-string)
//        const char* ifileName = file.c_str();          // input file
//        const char* headfName = dcbfName.c_str();
//        const char* ofileName = binfName.c_str();      // output file
//        infile.open(ifileName);
//        headerfile.open(headfName);

        infile.open(file);
        headerfile.open(dcbfName);
        cout << "Preparing on header (DCB) file: " << ctime(&rawtime) << endl;

        if (infile.is_open()) {

            int count_id = 0;  // count lines
            // Line 1: read first line
            string first_line;
            string hasheader = "true";
            getline(infile,first_line);
            rowinfo = split(first_line, ',', ' ');  // splits by "," and removes elements such as quotes or white spaces

            // check if every element of this line has a character to determine if it's header
            for (auto r=0; r < rowinfo.size(); r++){
                fieldname.push_back(rowinfo[r]) ;
                fieldwidth.push_back(0);
                fielddecimal.push_back(0);
                // checkheaderFields.push_back(contains_alpha(rowinfo[r])) ;
                checkheaderFields.push_back(has_quotes(rowinfo[r])) ;
                if (checkheaderFields[r] != 1) {hasheader = "false";}

                // add field data type as characters
                fieldtype.push_back("C");
            }

            // debug: print whether header exists or not
            cout << "header exists: " << hasheader << endl;

            // If no header then write one
            if (hasheader == "false"){
                for (auto r=0; r < rowinfo.size(); r++){
                   // fieldname[r] =  "Field_" + std::to_string(r);
                   string s;
                   stringstream ss;
                   ss << r;
                   fieldname[r] =  "Field_" + ss.str();
                }
            }

            // debug: print out header line
            for (int r=0; r < rowinfo.size(); r++){cout << fieldname[r] <<" " << checkheaderFields[r] <<" "<< fieldtype[r] << endl; }

            // check entire file to get the data types (char < int < double)
            // Debug: check data from a particular line (count_id)
            // while(!infile.eof() && count_id < maxLineCheck && count_id < 10){
            while(!infile.eof() && count_id < maxLineCheck){
            // while(!infile.eof()){
                if (count_id == 0 && hasheader == "false"){line = first_line;}
                else {getline(infile,line);}
                rowinfo = split(line, ',', '\0');  // use null character (meaning none of the chars such as white space or quotes are removed)

                for (auto r=0; r < rowinfo.size(); r++){
                   if(fieldtype[r] == "C") {
                      if(has_quotes(rowinfo[r]) == 1) {
                        rowinfo[r].erase(remove(rowinfo[r].begin(), rowinfo[r].end(), '"'),rowinfo[r].end());
                        // cout << "has quotes: " << fieldname[r] <<" " << rowinfo[r] << " "<< has_quotes(rowinfo[r]) <<endl;
                        // check and update field width
                        if (fieldwidth[r] < get_width(rowinfo[r])) {fieldwidth[r] = get_width(rowinfo[r]);}
                        } else {
                          // cout << "has quotes: " << has_quotes(rowinfo[r]) <<endl;
                         // check if it has numbers
                         if(is_number(rowinfo[r])) {
                             fieldtype[r]="I";
                             fieldwidth[r] = 4;
                             cout << "is_number: " << is_number(rowinfo[r]) << "field type for " << fieldname[r] << " changed to " << fieldtype[r] << " at line " << count_id << endl;
                         } else {
                             fieldtype[r] ="R";
                             fieldwidth[r] = 8;
                             cout << "field type for " << fieldname[r] << " changed to " << fieldtype[r] << " at line " << count_id << endl;
                         }

                       }
                   }

                   // Integer: can it be double else where
                   if(fieldtype[r] == "I") {
                      if(!is_number(rowinfo[r]) && rowinfo[r] != "NA") {
                        fieldtype[r]="R";
                        fieldwidth[r] = 8;
                      }
                    }

                   // Double: get decimal places
                   if(fieldtype[r] == "R") {
                      if (fielddecimal[r] < get_decimals(rowinfo[r])) {fielddecimal[r] = get_decimals(rowinfo[r]);}
                   }
                }

                // store data to data table
                datatable.push_back(rowinfo);
                // debug: Show the first row values from the datatable
                /* if(count_id == 1){
                   for (int x=0; x < datatable[count_id].size(); x++){ cout << datatable[count_id][x] << endl; }
                } */
                count_id += 1;
            }
            // Debug: write out data types for fields
            // for (int r=0; r < rowinfo.size(); r++){cout << fieldname[r] <<" " << fieldtype[r] << endl; }

            // assign bytes by data types and get field position
            int count_bytes = 0;
            for (auto r=0; r < fieldname.size(); r++){
               //
               if (fieldtype[r] == "C"){fieldbyte.push_back(fieldwidth[r]);}
               if (fieldtype[r] == "I"){fieldbyte.push_back(4);}
               if (fieldtype[r] == "R"){fieldbyte.push_back(8);}

               // compute field positions
               if (r==0) {
                    fieldposition.push_back(1);
               }
               else{
                    fieldposition.push_back(fieldposition[r-1]+fieldbyte[r-1]);
               }
               // total bytes across all fields
               count_bytes += fieldbyte[r];
            }

            // Write to header file
            for (auto r = 0; r < fieldname.size(); r++ ){
               if (r == 0){headerfile <<"\n" << count_bytes << endl;}
               headerfile << fieldname[r] << ","  << fieldtype[r]  << "," ;                  // fields 1-2
               headerfile << fieldposition[r] << "," << fieldwidth[r]   << ","  << fielddecimal[r] << ",";  // fields 3-5
               headerfile << 4+fieldwidth[r]    << "," << fielddecimal[r] << "," <<" , , , , ," << endl;    // fields 6-7,8-13
            }
            time(&rawtime);
            cout << "Total lines read in file "<< file << ": " << count_id << endl;
            cout << "Writing header (DCB) file completed :" << ctime(&rawtime) << endl;
            cout << "Size in datatable :" << datatable.size() << endl;

            // open the output file in binary mode
            // binfile.open(ofileName, ios::out | ios:: binary);
            // binfile.open(binfName, ios::out | ios:: binary);
            pFile = fopen(binfName.c_str(),"wb");

            if (count_id > 250000) {line_bin = 100000;}

            //========================================================================================
            // Method 1: Read from data table (can't handle heavy files since data table needs realloc)
            //========================================================================================
            if (count_id < maxLineCheck) {
                cout << "Memory size of current datatable: " << datatable.capacity() << endl;
                for (int d = 0; d < datatable.size(); d++) {
                    for (int r=0; r < datatable[current_id].size(); r++){

                       // binfile.seekp(pos,ios::beg);
                       char *pEnd;
                       if (fieldtype[r] == "R") {
                            test_d = strtod(datatable[current_id][r].c_str(),&pEnd);
                            // binfile.write((char*)&test_d,fieldbyte[r]);
                            // fwrite(datatable[current_id][r].c_str(),8,1,pFile);
                            fwrite((char*)&test_d,8,1,pFile);
                             // debug:
                            if (current_id == 0){cout << "double " << test_d << endl;}
                       }
                       if (fieldtype[r] == "I") {
                            test_i = strtol(datatable[current_id][r].c_str(),&pEnd,10);
                            // binfile.write((char*)&test_i,fieldbyte[r]);
                            // fwrite(datatable[current_id][r].c_str(),10,1,pFile);
                            fwrite((char*)&test_i,4,1,pFile);
                            // debug:
                            if (current_id == 0){cout << "int " << test_i << endl;}
                       }
                       if (fieldtype[r] == "C") {
                            test_s = new char [fieldbyte[r]];
                            test_s = datatable[current_id][r].c_str();
                            // binfile.write(test_s,fieldbyte[r]);
                            // fwrite(test_s,sizeof(test_s),1,pFile);
                            fwrite(test_s,fieldbyte[r],1,pFile);
                            // debug:                             // debug:
                            if (current_id == 0){cout << "string " << test_s  << " size " << sizeof(test_s) <<" bytes " << fieldbyte[r]<< endl;}
                       }
                       pos += fieldbyte[r];
                    }

                    // print lines processed in 1000s
                    current_id += 1;
                    if(current_id % line_bin == 0){
                       cout << "writing line: " << current_id <<" of " << count_id << endl;
                    }
                }

                fclose(pFile);
            } else {
            //========================================================================================
            // Method 2: Read from input file (reads one line at a time)
            //========================================================================================
                // infile.close();
                // Open input file
                // infile.open(ifileName);
                // read first line only if header is true
                infile.seekg(pos, ios::beg);
                if (hasheader == "true") {getline(infile,line);}

                // for(int l=0; l< 1; l++){
                while(!infile.eof()){
                    // read data
                    getline(infile,line);
                    // ignore empty lines                    if(line.size() > 0){
                        rowinfo = split(line, ',', '"');  // splits by "," and removes elements such as quote or whitespace"
                        for (int r=0; r < rowinfo.size(); r++){

                        // for (int r=0; r < 23; r++){
                           // binfile.seekp(pos,ios::beg);
                           char *pEnd;
                           if (fieldtype[r] == "R") {
                                test_d = strtod(rowinfo[r].c_str(),&pEnd);
                                // binfile.write((char*)&test_d,fieldbyte[r]);
                                fwrite((char*)&test_d,8,1,pFile);
    //                            cout << fieldtype[r] << " " << rowinfo[r] << " " << test_d;
    //                            cout << " pos: " <<  pos << " byte:" << fieldbyte[r] << endl;

                           }
                           if (fieldtype[r] == "I") {
                                test_i = strtol(rowinfo[r].c_str(),&pEnd,10);
                                // binfile.write((char*)&test_i,fieldbyte[r]);
                                fwrite((char*)&test_i,4,1,pFile);
    //                            cout << fieldtype[r] << " " << rowinfo[r] << " " << test_i;
    //                            cout << " pos: " <<  pos << " byte:" << fieldbyte[r] << endl;
                           }
                           if (fieldtype[r] == "C") {
                                test_s = new char [fieldbyte[r]];
                                test_s = rowinfo[r].c_str();
                                fwrite(test_s,fieldbyte[r],1,pFile);
                                // binfile.write(test_s,fieldbyte[r]);
    //                            cout << fieldtype[r] << " " << rowinfo[r] << " " << test_s;
    //                            cout << " pos: " <<  pos << " byte:" << fieldbyte[r] << endl;
                           }
                           pos += fieldbyte[r];
                        }
                     }
                     // print lines processed in 1000s
                     current_id += 1;
                     if(current_id % line_bin == 0){
                        cout << "writing line: " << current_id << endl;
                     }
                }
            } // file size checker (1 million records)
        } // input file checker

        else {
              cout << "Unable to read file " << file << endl;
        } // end File

        // track time to complete
        time(&rawtime);
        cout << "Writing binary (BIN) file completed at :" << ctime(&rawtime) << endl;
        binfile.close();
        infile.close();
        headerfile.close();
}


// Split string (splits by "," and removes elements such as quotes or whitespaces"
vector<string> split(const string &line, char delim, char remElement){
  string word;              // These two are local to this function need to move up
  vector<string> fields;    // These two are local to this function need to move up
  stringstream ss(line);
  while (getline(ss, word, delim)) {
    if (remElement == '"' || remElement == ' '){
      word.erase(remove(word.begin(), word.end(), remElement),word.end());
    }
    fields.push_back(word);
  }
  return(fields);
}


// Find a non-digit in the stream consider it as not a numeric
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {return ! std::isdigit(c);}) == s.end();
}

// Find an alphabet in the stream consider it as string
bool contains_alpha(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {return  std::isalpha(c);}) == s.end();
}

// check if first and last characters are quotes
bool has_quotes(const std::string& s)
{
    return (s[0] == '"' && s[s.size()-1] == '"');
}

// Get width of each the element in the vector
int get_width(const std::string& s)
{
    int element_width = 0;
    return element_width = s.end() - s.begin();
}

// Get width of each the element in the vector
int get_decimals(const std::string& s)
{
    int it= s.end() - std::find(s.begin(), s.end(), '\.');
    return it;
}

