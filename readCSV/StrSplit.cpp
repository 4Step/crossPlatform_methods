#include "StrSplit.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Constructor
StrSplit::StrSplit():line(s) {

}
// Method
vector<string> StrSplit::getFields(char delim, int rep){
     cout << "fields :" << endl;

     if (!flds.empty()) flds.clear();
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
}

/*
StrSplit::StrSplit(): public string
{
    //ctor
}

// split: receives a char delimiter; returns a vector of strings
// By default ignores repeated delimiters, unless argument rep == 1.
vector<string>& StrSplit::split(char delim, int rep) {
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
*/
