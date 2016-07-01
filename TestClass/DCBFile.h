#ifndef DCBFILE_H
#define DCBFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>       // std::vector
#include <iomanip>      // std::setw
#include <algorithm>    // std::remove

using namespace std;

// CLASS: Get field info of a binary file
class DCBFile
{
  public:
    DCBFile(string binfName);
    DCBFile();
    string getDCBFileName(string binfName);
    void readDCBFile(string fileName);
    vector<string> getFieldNames();
    vector<string> getFieldTypes();
    vector<int> getFieldBytes();
    vector<int> getFieldPos();
    int getNumFields();
    int getFieldSize();
    vector<string> split(const string&, char);
  private:
    string binfName;
    string fileName;
    vector<string> fieldNames;
    vector<string> fieldTypes;
    vector<int> fieldBytes;
    vector<int> fieldPos;
    int n_fields;
    int s_fields;
};


#endif // DCBFILE_H
