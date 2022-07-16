#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "types.h"

using namespace std;


string rtrim(string str) {
  return str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1);
}

string ltrim(string str) {
  return str.erase(0, str.find_first_not_of("\t\n\v\f\r "));
}

string trim(string str) {
  return ltrim(rtrim(str));
}

int moreLargerItemLength(vector<ILisch_Item> list, bool _trim = true) {
  int res = 0;

  for (ILisch_Item x : list) {
    string text = _trim ? trim(x.text) : x.text;
    int text_len = text.length();
    if (text_len > res) res = text_len;
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

string removeSymbols(string str, string more = "") {
  string accept = "abcdefghijklmnopqrstuvwxyz";
  string res;

  for (char ch : str)
    if (
      accept.find(ch) != string::npos ||
      more.find(ch) != string::npos
    ) res += ch;

  return res;
}

string repeatCh(char ch, int times) {
  string res = "";

  for (int x = 0; x < times; x++)
    res += ch;

  return res;
}

#endif