#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

string removeColorParams(string text) {
  string res = "";
  bool safe = false;
  bool inParam = false;

  for (char ch : text) {
    if (safe) res += ch;
    else {
      if (ch == '~') safe = true;
      else if (ch == '[') inParam = true;

      if (!inParam)
        res += ch;

      if (ch == ']') inParam = false;
    }
  }

  return res;
}

vector<string> splitString(string str, char ch) {
  vector<string> res;
  res.push_back("");

  for (char x : str)
    if (x == ch) res.push_back("");
    else res[res.size() - 1] += x;

  return res;
}
