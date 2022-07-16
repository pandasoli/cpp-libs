#ifndef CONIO_H
#define CONIO_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <future>

using namespace std;

namespace vconio {
  /*
    -- conversios
  */

  string inttostr(int number) {
    string res;
    stringstream ss;
    ss << number;
    ss >> res;
    return res;
  }

  int strtodec(string str) {
    int res;
    stringstream ss;
    ss << str;
    ss >> res;
    return res;
  }

  int strtohex(string code) {
    unsigned res;
    stringstream ss;
    ss << hex << code;
    ss >> res;
    return res;
  }

  int dectohex(int decimal) {
    unsigned res;
    stringstream ss;
    ss << hex << decimal;
    ss >> res;
    return res;
  }

  int hextodec(string code) {
    int res;
    stringstream ss;
    ss << hex << code;
    ss >> res;
    return res;
  }

  string hextostr(int code) {
    string res;
    stringstream ss;
    ss << hex << code;
    ss >> res;
    return res;
  }

  /*
    -- functions
  */

  void gotoxy(int x, int y) {
    SetConsoleCursorPosition(
      GetStdHandle(STD_OUTPUT_HANDLE),
      { short(x), short(y) }
    );
  }

  void cursorvsb(bool value) {
    CONSOLE_CURSOR_INFO CursorInfos;
    CursorInfos.dwSize = 100;
    CursorInfos.bVisible = value;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfos);
  }

  void textcolor(unsigned hex, short fc = 0, short bgc = 0) {
    unsigned color = hex;

    if (hex == 0) color = fc + bgc * 16;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
  }

  void printxy(string str, COORD pos) {
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfos;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ScreenBufferInfos);

    gotoxy(pos.X, pos.Y);
    cout << str;

    gotoxy(
      ScreenBufferInfos.dwCursorPosition.X,
      ScreenBufferInfos.dwCursorPosition.Y
    );
  }

  void cprintxy(string str, COORD pos, bool jmp = true, string bg = "colored") {
    string color = "";
    bool saveColor = false;
    bool safe = false;
    COORD nPos = pos;

    CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfos;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ScreenBufferInfos);

    string WindowColor = hextostr(dectohex(ScreenBufferInfos.wAttributes));

    if (WindowColor.length() == 1)
      WindowColor = "0" + WindowColor;

    for (int x = 0; x < str.length(); x++) {
      char ch = str[x];

      if (safe) {
        safe = false;
        nPos.X++;
        printxy({ ch }, nPos);
      }
      else {
        if (ch == '~') safe = true;
        else if (ch == '[') saveColor = true;
        else if (saveColor) {
          if (ch == ']') {
            saveColor = false;

            if (color == "") color = WindowColor;
            else if (color.length() == 1) color = WindowColor[0] + color;

            textcolor(strtohex(color));
            color = "";
          }
          else
            color += ch;
        }
        else {
          if (ch == ' ') {
            if (bg != "transparent")
              printxy({ ch }, nPos);
          }
          else
            printxy({ ch }, nPos);
          nPos.X++;
        }
      }
    }

    gotoxy(nPos.X, pos.Y);

    if (str[str.length() - 1] == ' ' && str[str.length() - 2] == ' ')
      cout << endl;

    textcolor(ScreenBufferInfos.wAttributes);

    if (jmp)
      gotoxy(ScreenBufferInfos.dwCursorPosition.X, ScreenBufferInfos.dwCursorPosition.Y);
  }


  /*
    -- classes
  */

  #define KB_BACKSPACE 8
  #define KB_ENTER 13

  class Buffer {
  private:
    struct {
      COORD cursor;
      unsigned color;
    } window;

    void ini() {
      CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfos;
      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ScreenBufferInfos);

      this->window = {
        ScreenBufferInfos.dwCursorPosition,
        ScreenBufferInfos.wAttributes
      };

      loadCursor();
      textcolor(this->color);
    }

    void end() {
      gotoxy(this->window.cursor.X, this->window.cursor.Y);
      textcolor(dectohex(this->window.color));
    }

    void loadCursor() {
      gotoxy(
        this->pos.X + this->cursor.X,
        this->pos.Y + this->cursor.Y
      );
    }

    void nextCursor() {
      this->cursor.X++;

      if (this->cursor.X % this->size.X == 0)
        this->cursor = { 0, short(this->cursor.Y + 1) };

      loadCursor();
    }

    void backCursor() {
      this->cursor.X--;

      if (this->cursor.X < 0)
        this->cursor = {
          short(this->size.X - 1),
          short(this->cursor.Y - 1)
        };

      loadCursor();
    }


    void clear_fun() {
      for (int y = 0; y < this->size.Y; y++) {
        for (int x = 0; x < this->size.X; x++) {
          cout_fun(" ");
        }
      }

      this->cursor = { 0, 0 };
    }

    void cout_fun(string str) {
      for (int x = 0; x < str.length(); x++) {
        char ch = str[x];

        if (ch == '\n') {
          this->cursor = { 0, short(this->cursor.Y + 1) };
          loadCursor();
        }
        else {
          cout << ch;
          nextCursor();
        }
      }
    }

    string cin_fun() {
      string str = "";
      short key;
      bool loop = true;

      while (loop) {
        key = getch();
        if (key == 0 || key == 0xe0) key = getch();

        if (loop) {
          if (key == KB_ENTER) loop = false;
          else if (key == KB_BACKSPACE) {
            if (str.length() > 0) {
              str = str.substr(0, str.length() - 1);
              backCursor();
              cout_fun(" ");
              backCursor();
            }
          }
          else {
            str += char(key);
            cout_fun({ char(key) });
          }
        }
      }

      return str;
    }

  public:
    COORD pos;
    COORD size;
    COORD cursor;
    unsigned color;

    Buffer(COORD size, COORD pos, unsigned cl = 0x0f) {
      this->pos = pos;
      this->size = size;
      this->color = cl;
      this->cursor = { 0, 0 };

      clear();
    }

    void clear() {
      ini();
      clear_fun();
      end();
    }

    void setSize(COORD n) {
      ini();
      const unsigned oldColor = this->color;

      this->color = this->window.color;

      clear_fun();

      this->color = oldColor;
      this->size = n;

      clear_fun();
      end();
    }

    void setPos(COORD n) {
      ini();
      const unsigned oldColor = this->color;

      this->color = this->window.color;

      clear_fun();

      this->color = oldColor;
      this->pos = n;

      clear_fun();
      end();
    }

    friend Buffer& operator >>(Buffer &bfr, string *var) {
      bfr.ini();
      *var = bfr.cin_fun();
      bfr.end();
    }

    friend Buffer& operator <<(Buffer &bfr, string str) {
      bfr.ini();
      bfr.cout_fun(str);
      bfr.end();
    }

  };

}

#endif