#ifndef SPLITSTRING_H
#define SPLITSTRING_H


class splitstring : public string {
    vector<string> flds;
public:
    splitstring(char *s) : string(s) { };
    vector<string>& split(char delim, int rep=0);
};

#endif // SPLITSTRING_H
