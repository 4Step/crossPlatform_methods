/* Class splitstring which adds method split()
  define MAIN if this is a standalone program
*/
#define MAIN 1

#include <string>
#include <vector>
#include <iostream>
using namespace std;


class splitstring : public string {
    vector<string> flds;
public:
    splitstring(char *s) : string(s) { };
    vector<string>& split(char delim, int rep=0);
};

// split: receives a char delimiter; returns a vector of strings
// By default ignores repeated delimiters, unless argument rep == 1.
vector<string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    string work = data();
    string buf = "";
    int i = 0;
    while (i < work.length()) {
        if (work[i] != delim)
            buf += work[i];
        else if (rep == 1) {
            flds.push_back(buf);
            buf = "";
        } else if (buf.length() > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
        flds.push_back(buf);
    return flds;
}


// Convert string line to character array
char* convertToChar(string line){
  int lineLen = line.size();
  char* newLine = new char[lineLen];
  for(int k = 0; k<= lineLen; k++){
    newLine[k] = line[k];
  }
  return(newLine);
}


#ifdef MAIN
main()
{
    // we define a string
    string line = "Humpty, Dumpty, sat, on, a, wall,Humpty, Dumpty, had, a, great ,fall";
    char *charLine = convertToChar(line);

    splitstring s(charLine);
    // splitstring s("Humpty, Dumpty, sat, on, a, wall,Humpty, Dumpty, had, a, great ,fall");
    cout << s << endl;

    // splits and displays the vector of strings
    vector<string> flds = s.split(',');
    for (int k = 0; k < flds.size(); k++)
        cout << flds[k] << endl;

    // now taking account of repeated delimiters
    cout << endl << "with repeated delimiters:" << endl;
    vector<string> flds2 = s.split(',', 1);
    for (int k = 0; k < flds2.size(); k++)
        cout << flds2[k] << endl;

}
#endif
