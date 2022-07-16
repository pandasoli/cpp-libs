#include <iostream>
#include <windows.h>
#include <vector>
#include "funs.cpp"
#include "../vconio.h"

using namespace std;
using namespace vconio;


int main(int argc, char** argv) {
  const string input = string(GetCommandLineA()).substr(string(argv[0]).length() + 2);

  struct Param {
    string key;
    string value;
  };

  vector<Param> params;
  string text = "";

  bool inParam = false;
  bool inParamValue = false;
  bool safeArea = false;
  bool noParamsArea = false;

  for (int x = 0; x < input.length(); x++) {
    char ch = input[x];

    if (safeArea || noParamsArea) {
      text += ch;
      if (ch == '~' && safeArea) text += ch;

      safeArea = false;
    }
    else {
      if (ch == '~') safeArea = true;
      else {
        if (ch == ' ') {
          inParam = false;
          inParamValue = false;
        }

        if (inParam) {
          if (ch == ':') inParamValue = true;
          else {
            if (inParamValue) params[params.size() - 1].value += ch;
            else params[params.size() - 1].key += ch;
          }
        }

        if (ch == '/') {
          inParam = true;
          params.push_back({ "", "" });
        }

        if (!inParam && ch != ' ') {
          text += ch;
          noParamsArea = true;
        }
      }
    }

  }

  // ---

  CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfos;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ScreenBufferInfos);

  COORD pos = ScreenBufferInfos.dwCursorPosition;
  string idiom = "English";
  string end = "back";
  string bg = "colored";

  for (Param param : params) {
    if (param.key == "idiom") idiom = param.value;
    else if (param.key == "x") {
      string withNoColor = removeColorParams(text);

      if (param.value == "left") pos.X = 0;
      else if (param.value == "center") pos.X = ScreenBufferInfos.dwSize.X / 2 - withNoColor.length() / 2 - 1;
      else if (param.value == "right") pos.X = ScreenBufferInfos.dwSize.X - withNoColor.length();
      else {
        if (param.value[0] == '+' || param.value[0] == '-')
          pos.X += strtodec(param.value);
        else
          pos.X = strtodec(param.value);
      }
    }
    else if (param.key == "y") {
      if (param.value == "top") pos.Y = 0;
      else if (param.value == "center") pos.Y = short(ScreenBufferInfos.dwSize.Y / 2 - 1);
      else if (param.value == "bottom") pos.X = ScreenBufferInfos.dwSize.Y - 1;
      else {
        if (param.value[0] == '+' || param.value[0] == '-')
          pos.Y += strtodec(param.value);
        else
          pos.Y = strtodec(param.value);
      }
    }
    else if (param.key == "end") {
      if (param.value == "pointer") end = param.value;
    }
    else if (param.key == "bg") {
      if (param.value == "transparent") bg = param.value;
    }
  }

  setlocale(LC_ALL, idiom.c_str());
  cprintxy(text, pos, end != "pointer" ? true : false, bg);

  return 0;
}
