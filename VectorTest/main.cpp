#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

/*
vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}
*/

vector<string> split(const string &line, char delim){
  string word;
  vector<string> fields;
  stringstream ss(line);
  while (getline(ss, word, ',')) {
    fields.push_back(word);
  }
  return(fields);
}

int main ()
{
  string line = "This, is, a, single, string, to, split, by, comma";
 /* // function inside the main
  vector<string> elems;
  string x;
  stringstream ss (line);
    while (getline(ss, x, ',')) {
    elems.push_back(x);
  }
  */
  // Moved outside main
   vector<string> elems = split(line, ',');

  int xLen = elems.size();
  for (int i = 0; i < xLen; i++) {
    cout << elems[i] << endl;
  }
}
