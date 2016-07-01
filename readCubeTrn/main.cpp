#include <time.h>       // system time
#include <iostream>     // open() - log file
#include <fstream>      // open() - log file
#include <sstream>      // split()
#include <string.h>
#include <vector>       // std::vector
#include <iomanip>      // std::setw - log file
#include <algorithm>    // std::remove

using namespace std;
string file;       // Input file (user entry)
ifstream datafile; // input stream
ofstream outcsv;   // csv file stream


  // FUNCTION: Split string
  vector<string> split(const string &line, char delim){
    string word;
    string lineEnd;
    vector<string> fields;
    stringstream ss(line);

    // first split by line ending
    while (getline(ss, lineEnd, '\n')){
      stringstream se(lineEnd);
      while (getline(se, word, delim)) {
        word.erase(remove(word.begin(), word.end(), '\"' ),word.end());
        fields.push_back(word);
      }
    }
    return(fields);
  }

//=============================================================================
//                   MAIN PROGRAM
//=============================================================================
  // Main program
  int main()
  {
    // Get system time
    time_t rawtime;
    time(&rawtime);
    string row;
    string element;
    string value;
    string longName;
    string circ;
    string oper;
    string mode;
    string hdwy1;
    string hdwy2;
    string hdwy3;
    string waitcrv;
    string timefac;
    string use1;
    string use2;
    string use3;
    string use4;
    string nodes;

    // User to enter file name
    cout << "Please enter the file name: ";
    cin >> file;

    // file = "TROUTE_10A.LIN" ;

	  // Check file
	  datafile.open(file);
    if (datafile.is_open())
	  {
      cout <<"=============================================================================" << endl;
      cout <<" Convert (CUBE to CSV)                             "<<endl;
      cout <<" Version Beta (Nov,2013)" << endl;
      cout <<" Parsons Brinckerhoff" << endl;
      cout <<  ctime(&rawtime);
      cout <<"=============================================================================" << endl;
      cout << "\n Processing file " << file << "\n" << endl;

      // Write output to csv file and summary to log file
      vector<string> fileNameParts = split(file, '.');
      string csvName = fileNameParts[0]+".csv";
      string logName = fileNameParts[0]+".log";
      const char* ofileName = csvName.c_str();

      outcsv.open(ofileName);
      // Write Header
      outcsv << "ID, NAME, LONGNAME, MODE, OPERATOR, HEADWAY[1], HEADWAY[2], HEADWAY[3], WAITCURVE, TIMEFAC[1],USERA1, USERA2, USERA3, USERA4" << endl;

      int lineNum = 0;
      streampos pos;

       while (datafile.good()) {
       // for(int i=1; i<1000; i++) {
         // Read line, split by coma, further split by =, trim white space and get values
         getline(datafile,element,',');

         pos = datafile.tellg();

         // Trim space and line endings
         element.erase(remove(element.begin(), element.end(), ' ' ),element.end());
         vector<string> elementData = split(element, '=');
         for(int e=0; e<elementData.size(); e++) {
            if (elementData[e] == "LINENAME") {
               lineNum = lineNum + 1;
               outcsv << lineNum << " , " << elementData[e+1]<< " , ";

               longName = "";
               circ = "";
               oper = "";
               mode = "";
               hdwy1= "";
               hdwy2= "";
               hdwy3= "";
               waitcrv="";
               timefac="";
               use1 ="";
               use2 ="";
               use3 ="";
               use4 ="";
               nodes = "";
               // read next 100 lines
               for(int r=0; r<100; r++) {
                 getline(datafile,row,',');
                 vector<string> elementData = split(row, '=');
                 string nextLineName = elementData[0];
                 nextLineName.erase(remove(nextLineName.begin(), nextLineName.end(), ' ' ),nextLineName.end());
                 nextLineName.erase(remove(nextLineName.begin(), nextLineName.end(), '\n' ),nextLineName.end());

                // This assumes the all variables are coded before Node listings
                 if (nextLineName == "LONGNAME")    longName = elementData[1];
                 if (nextLineName == "CIRCULAR")    circ =  elementData[1];
                 if (nextLineName == "OPERATOR")    oper =  elementData[1];
                 if (nextLineName == "MODE")        mode =  elementData[1];
                 if (nextLineName == "HEADWAY[1]")  hdwy1 =  elementData[1];
                 if (nextLineName == "HEADWAY[2]")  hdwy2 =  elementData[1];
                 if (nextLineName == "HEADWAY[3]")  hdwy3 =  elementData[1];
                 if (nextLineName == "WAITCURVE")  waitcrv =  elementData[1];
                 if (nextLineName == "TIMEFAC[1]")  timefac =  elementData[1];
                 if (nextLineName == "USERA1")  use1 =  elementData[1];
                 if (nextLineName == "USERA2")  use2 =  elementData[1];
                 if (nextLineName == "USERA3")  use3 =  elementData[1];
                 if (nextLineName == "USERA4")  use4 =  elementData[1];
                 if (nextLineName == "N") {
                    // nodes = elementData[1];
                    r = 100;
                    outcsv << longName <<"," << oper << "," << mode <<"," << hdwy1 <<"," << hdwy2 << "," << hdwy3 <<"," << waitcrv <<","<< timefac << ","<< use1 <<"," << use2 << "," << use3 << "," << use4 << endl;
                 }
              }
              break;
            }
         }
    }

	  // Close file connections
	  datafile.close();
	  outcsv.close();
	  cout << "   !!!!! ---  Successfully Exported  ---  !!!!  " << endl;
	 }

	 else cout << "Unable to open/read the file";
	 return 0;
}
