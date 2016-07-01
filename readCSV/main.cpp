#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "StrSplit.h"

using namespace std;
int nrows = 20;
int nfields = 6;
char fName[] = "skyassignment.csv";
ifstream datafile;
ofstream outfile;

// Method to convert string to array characters
char* convertToChar(string line){
  int lineLen = line.size();
  char* newLine = new char[lineLen];
  for(int k = 0; k<= lineLen; k++){
    newLine[k] = line[k];
  }
  return(newLine);
}


// Method to join strings
string joinStrings(string A, string B) {
   string C = A + "-" + B;
   return C;
}

string readHeader(char fileName[]){

     // Open the file
    datafile.open(fileName);

	// Check file
	if (datafile.is_open())
	{
      // Read the first line
      string firstRow;
      getline(datafile,firstRow);
      cout << firstRow;

      // Get the length of string
      nfields = firstRow.length();
      cout << 'Total fields = ' + nfields << endl;

      datafile.close();
      return(firstRow);
	} else {
	  cout << "Unable to open/read the file";
	}
}


/*
// Method to set and get values
string getValues(string data[nrows][nfields], string selIJ, int col) {
     string IJ[nrows] = data[nrows][7];
     string value[nrows] = data[nrows][col];
     for (int r = 0; r <= IJ.length(); r++ ) {
        if(IJ[r] == selIJ[r]) {
          string foundVal = data[r];
          return(foundVal);
        }
     }
}
*/

int main() {
    string csvData[nrows][nfields];
    string IJZone;
    string row;
	// Get header fields
    // string header = readHeader(fName);

	// Open file
	datafile.open(fName);
	outfile.open("skywaytrips.txt");

    getline(datafile,row);
    char *charLine = convertToChar(row);

    StrSplit splitObj;

    /*
    splitObj(charLine);

    vector<string> flds2 = StrSplit.split(',', 1);
    for (int k = 0; k < flds2.size(); k++){
        cout << flds2[k] << endl;
    }
    */

	// Check file
	if (datafile.is_open())
	{
	 // test code
     for(int r=0; r<10; r++) {
	 // while (datafile.good()) {
		// read data
        for (int f=1; f<= nfields; f++) {
           if (f < nfields){
             getline(datafile,csvData[r][f],',');
           }
           else{
             getline(datafile,csvData[r][f]);
           }
        }
        // Function to join strings
        csvData[r][7] = joinStrings(csvData[r][2], csvData[r][3]) ;

		// write line
		outfile << csvData[r][7] << endl;
	 }
	  // Close file connections
	  datafile.close();
	  outfile.close();
	}
	else cout << "Unable to open/read the file";
	return 0;
}
