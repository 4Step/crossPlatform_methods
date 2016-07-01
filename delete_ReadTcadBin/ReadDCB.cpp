#include "ReadDCB.h"

  // Constructors
  DCBFile::DCBFile(){
  }
  DCBFile::DCBFile(string binfName){
  }

  // FUNCTION: Get DCB filename
  string DCBFile::getDCBFileName(string binfName){

      // Get the dictionary filename
      vector<string> fileNameParts = split(binfName, '.');
      string fileName = fileNameParts[0]+".dcb";
      // cout << "The original file is: " << binfName ;
      // cout << " and the dictionary file is: " << fileName << endl;
      return(fileName);
  }

  // FUNCTION: Get field names
  vector<string> DCBFile::getFieldNames(){
       return(fieldNames);
  }

  // FUNCTION: Get field types
  vector<string> DCBFile::getFieldTypes(){
       return(fieldTypes);
  }

  // FUNCTION: Get field bytes
  vector<int> DCBFile::getFieldBytes(){
       return(fieldBytes);
  }

  // FUNCTION: Get field starting position
  vector<int> DCBFile::getFieldPos(){
       return(fieldPos);
  }

  // FUNCTION: Get number of fields
  int DCBFile::getNumFields(){
       return(n_fields);
  }

  // FUNCTION: Get size of all fields
  int DCBFile::getFieldSize(){
       return(s_fields);
  }
  // FUNCTION: Read the contents of DCB file
   void DCBFile::readDCBFile(string binfName){
      // Local variables
      fstream dictFile;
      string blankLine;
      string sizeLine;
      string dataLine;
      vector<string> fieldinfo;

      string dcbfName = getDCBFileName(binfName);
      const char* ifileName = dcbfName.c_str();
      dictFile.open(ifileName);

      if(dictFile.is_open()){

           // Read first two lines
           getline(dictFile,blankLine);
           getline(dictFile,sizeLine);

           // Get size of all fields
           remove(sizeLine.begin(), sizeLine.end(), ' ');
           s_fields = atoi(sizeLine.c_str());

           // Get total number of lines in the file
           int count_id = 0;
           while(!dictFile.eof()){

              // Get line and remove white spaces
              getline(dictFile,dataLine);
              // remove(dataLine.begin(), dataLine.end(), ' ');

              // check blank line and skip
              if(dataLine.size() > 0){

                // track number of lines
                count_id += 1;

                // Get field info
                fieldinfo.clear();
                fieldinfo = split(dataLine, ',');
                fieldNames.push_back(fieldinfo[0]);
                fieldTypes.push_back(fieldinfo[1]);
                fieldPos.push_back(atoi(fieldinfo[2].c_str()));
                fieldBytes.push_back(atoi(fieldinfo[3].c_str()));
                n_fields = fieldNames.size();
                // cout.setf(ios::left);
                // cout << setw(3) << n_fields << " " << setw(16) << fieldNames[n_fields-1] << " " ;
                // cout << setw(3) << fieldTypes[n_fields-1] << " " << setw(3) << temp_fieldBytes[n_fields-1] << endl;

              }
           }
      }
      else {
          cout << "Couldn't find file: " << ifileName << endl;
      }
    dictFile.close();
    }

  // FUNCTION: Split string
  vector<string> DCBFile::split(const string &line, char delim){
    string word;              // These two are local tho this function need to move uo
    vector<string> fields;    // These two are local tho this function need to move uo
    stringstream ss(line);
    while (getline(ss, word, delim)) {
      word.erase(remove(word.begin(), word.end(), '\"' ),word.end());
      fields.push_back(word);
    }
    return(fields);
  }

