#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>       // std::vector
#include <iomanip>      // std::setw
#include <algorithm>    // std::remove

using namespace std;

char* binfName = "TransitR.bin";

ifstream dictFile;
ofstream output;
string blankLine;
string sizeLine;
string dataLine;
vector<string> fieldNames;
vector<string> fieldTypes;
vector<string> temp_fieldBytes;
vector<string> fieldinfo;
int nth_fields;

// Function prototype
vector<string> split(const string &line, char delim);

int main()
{
    // Get the dictionary filename
    string file = binfName;
    vector<string> fileNameParts = split(binfName, '.');
    string dcbfName = fileNameParts[0]+".dcb";
    const char* ifileName = dcbfName.c_str();
    cout << ifileName << endl;

    // auto-create output file
    string txtfName = fileNameParts[0]+".csv";
    const char* ofileName = txtfName.c_str();
    cout << "input file is:" << ifileName <<" -> " << ofileName << endl;

    dictFile.open(ifileName);
    output.open(ofileName);

	if(dictFile.is_open()){

       // Read first two lines
       getline(dictFile,blankLine);
       getline(dictFile,sizeLine);
       cout<< "size is: "<< sizeLine << endl;

       // Get total number of lines in the file
       int count_id = 0;
       while(!dictFile.eof()){

          // Get line and remove white spaces
          getline(dictFile,dataLine);
          remove(dataLine.begin(), dataLine.end(), ' ');

          // check blank line and skip
          if(dataLine.size() > 0){

            // track number of lines
            count_id += 1;

            //Print line to the screen
            // cout << dataLine << endl;

            // Remove whitespace
            remove(dataLine.begin(), dataLine.end(), ' ');
            fieldinfo.clear();
            fieldinfo = split(dataLine, ',');
            fieldNames.push_back(fieldinfo[0]);
            fieldTypes.push_back(fieldinfo[1]);
            temp_fieldBytes.push_back(fieldinfo[3]);

            nth_fields = fieldNames.size();
            output.setf(ios::left);
            output << setw(3) << nth_fields << " " << setw(16) << fieldNames[nth_fields-1] << " " ;
            output << setw(3) << fieldTypes[nth_fields-1] << " " << setw(3) << temp_fieldBytes[nth_fields-1] << endl;

          }
       }
	}
	else {
      cout << "Couldn't find file: " << ifileName << endl;
	}
dictFile.close();
output.close();
}

// Split string
vector<string> split(const string &line, char delim){
  string word;              // These two are local tho this function need to move uo
  vector<string> fields;    // These two are local tho this function need to move uo
  stringstream ss(line);
  while (getline(ss, word, delim)) {
    word.erase(remove(word.begin(), word.end(), '\"' ),word.end());
    fields.push_back(word);
  }
  return(fields);
}


//==========================================================================
//  Next steps: Create a class object to hold DCB file contents
//==========================================================================
// CLASS StrProcess
/*  String process class
   Move split method
   Removes quotes
    - Add trim method
   Remove whitespace (all whitespace between the elements)
   Replace whitespace with underscore
*/

// CLASS VectorToArray
/*  converts vector list to arrays
   Convert vector<string> string[]
   Convert vector<string> char[]
   Convert vector<string> int[]
   Convert vector<int>    int[]
   Convert vector<long>   long[]
   Convert vector<float>  float[]
   Convert vector<double> double[]

*/

/*
// CLASS DCBFile
class DCBFile(string fname){
    public:
        ifstream dictFile;
        string blankLine;
        string sizeLine;
        string dataLine;

        vector<string> fieldinfo;
        vector<string> fieldNames;      // rename this to temp_fieldNames
        vector<string> fieldTypes;
        vector<string> temp_fieldBytes;
        string *array_fieldNames;       // rename the variable to fieldNames
        char *array_fieldNames;
        int *temp_fieldBytes;

     // FUNCTION: Read file contents
     void readDCBFile(string fname){
        dictFile.open(ifileName);

        if(dictFile.is_open()){
            // Read first two lines
            getline(dictFile,blankLine);
            getline(dictFile,sizeLine);
            cout<< "size is: "<< sizeLine << endl;

            while(!dictFile.eof()){
                getline(dictFile,dataLine);
                fieldinfo.clear();
                fieldinfo = split(dataLine, ',');
                fieldNames.push_back(fieldinfo[0]);
                fieldTypes.push_back(fieldinfo[1]);
                temp_fieldBytes.push_back(fieldinfo[3]);
             }
            cout << fieldNames[0] << " " << fieldTypes[0] << " " <<temp_fieldBytes[0] << endl;
        }
        else {
        cout << "Couldn't find file: " << ifileName << endl;
        }
        dictFile.close();
     }

    // FUNCTION: Get field names
    vector<string> getFieldNames(){
        array_fieldNames = new string[fieldNames.size()];
        for(int i=0; i<fieldNames.size(); i++){
            array_fieldNames[i] = fieldNames[i];//Copy the vector to the string
        }
        return array_fieldNames;
    }

    // FUNCTION: Get field types
    vector<string> getFieldTypes(){
        array_fieldTypes = new string[fieldTypes.size()];
        for(int i=0; i<fieldTypes.size(); i++){
            array_fieldTypes[i] = fieldTypes[i];//Copy the vector to the string
        }
        return array_fieldTypes;
    }

    // FUNCTION : Get field bytes
    vector<string> getFieldBytes(){
        array_fieldBytes = new string[fieldBytes.size()];
        for(int i=0; i<fieldBytes.size(); i++){
            array_fieldBytes[i] = fieldTypes[i];//Copy the vector to the string
        }
        return array_fieldBytes;
    }
};

*/
