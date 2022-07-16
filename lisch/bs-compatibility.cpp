#include <bits/stdc++.h>
#include <windows.h>
#include <direct.h>
#include <fstream>
#include <vector>

#include "Lisch.cpp"
#include "Formats/base.h"
#include "Formats/list.h"
#include "Formats/table.h"
#include "types.h"
#include "../vconio.h"

using namespace std;
using namespace vconio;


struct ConfigFileLine {
  string key;
  string value;
};
ConfigFileLine separeKeyValue(string line, char separator = '=') {
  string key;
  string value;
  bool separatorFound = false;

  for (int x = 0; x < line.length(); x++) {
    if (line[x] == separator) separatorFound = true;
    else {
      if (!separatorFound) key += line[x];
      else value += line[x];
    }
  }

  return { key, value };
}

int main(int argc, char** argv) {
  char cCurrentPath[FILENAME_MAX];
  getcwd(cCurrentPath, sizeof(cCurrentPath));

  CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfos;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ScreenBufferInfos);

  ifstream configFile (string(cCurrentPath) + "\\.config");
  string path = "";
  string line;

  COORD Buffer_size;
  COORD Buffer_pos;
  ILisch_Styles styles = Lisch_CreateStyle();

  while (configFile.good() && configFile.is_open()) {
    configFile >> line;

    if (line[0] == ';') {
      configFile >> line;
    }
    else {
      if (line[0] == '[') {
        if (line[1] == '.') path += line.substr(1, line.length() - 2);
        else path = line.substr(1, line.length() - 2);
      }
      else {
        ConfigFileLine infos = separeKeyValue(line);

        if (infos.value != "") {
          if (path == "lisch.buffer") {
            short value = strtodec(infos.value);

            if (infos.key == "width") Buffer_size.X = value;
            else if (infos.key == "height") Buffer_size.Y = value;
            else if (infos.key == "x") Buffer_pos.X = value;
            else if (infos.key == "y") Buffer_pos.Y = value;
          }
          else if (path == "lisch.styles") {
            if (infos.key == "text-align") styles.text_align = infos.value;
            else if (infos.key == "padding") styles.padding = strtodec(infos.value);

            else if (infos.key == "select-color") styles.select.color = strtohex(infos.value);
            else if (infos.key == "select-focus_color") styles.select.focus_color = strtohex(infos.value);

            else if (infos.key == "check-color") styles.check.color = strtohex(infos.value);
            else if (infos.key == "check-check_color") styles.check.check_color = strtohex(infos.value);
            else if (infos.key == "check-focus_color") styles.check.focus_color = strtohex(infos.value);
            else if (infos.key == "check-focus_check_color") styles.check.focus_check_color = strtohex(infos.value);
          }
        }
      }
    }
  }

  // ---

  Lisch lst;
  string type;
  string format;

  for (int x = 1; x < argc; x++) {
    if (argv[x][0] == '/') {
      ConfigFileLine infos = separeKeyValue(string(argv[x]).substr(1), ':');

      if (infos.key == "type") type = infos.value;
      else if (infos.key == "format") format = infos.value;
    }
    else
      lst.pushItem(argv[x]);
  }

  if (format == "base") lst.format = new Lisch_base_Format(type);
  else if (format == "list") lst.format = new Lisch_list_Format(type);
  else if (format == "table") lst.format = new Lisch_table_Format(type);

  lst.setStyles(styles);
  lst.setBuffer(Buffer_size, Buffer_pos);

  lst.start();

  return lst.selected.current;
}
