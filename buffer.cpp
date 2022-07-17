#include <iostream>

#include "vconio.h"

using namespace std;
using namespace vconio;


int main(int argc, char** argv) {
  Buffer bfr ({ 30, 10 }, { 2, 3 }, 0x4f);
  string res = "";

  bfr << "What's y" << "our name? ";

  gotoxy(0, 20);
  return 0;
}
