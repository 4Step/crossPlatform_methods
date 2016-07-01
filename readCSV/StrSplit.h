#ifndef STRSPLIT_H
#define STRSPLIT_H
#include <string>
#include <vector>
#include <iostream>

class StrSplit
{
    public:
        StrSplit(string s);
        vector<string> getFields(char delim, int rep);
    private:
        string line;
        vector<string> fields;

};

/*
// class StrSplit:public string {
class StrSplit{
    vector<string> flds;
public:
    StrSplit(char *s) : string(s) { };
    vector<string>& StrSplit:split(char delim, int rep=0);
};
*/
#endif // STRSPLIT_H



