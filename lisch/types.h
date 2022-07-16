#ifndef TYPES_H
#define TYPES_H

#include <windows.h>
#include <string>
using namespace std;


struct ILisch_Selected {
  short int current;
  short int last;
};

struct ILisch_Item {
  string text;
  COORD pos;
  bool check;
};

struct ILisch_Styles {
  string text_align;
  short int padding;
  struct {
    short color;
    short focus_color;
  } select;
  struct {
    short color;
    short check_color;
    short focus_color;
    short focus_check_color;
  } check;
};

#endif
