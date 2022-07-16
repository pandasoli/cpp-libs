#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <conio.h> // _getch
#include "Formats/base.h"
#include "./types.h"

using namespace std;

#define KB_UP 72
#define KB_DOWN 80
#define KB_RIGHT 77
#define KB_LEFT 75
#define KB_SPACE 32
#define KB_ENTER 13


class Lisch {
  private:
    bool changedItems;

    void end() {
      cursorvsb(true);
      // gotoxy(0, items[items.size() - 1].pos.Y + 1);
      // textcolor(0x0f);
    }

    string colorCodeToString(short code) {
      return '[' + hextostr(code) + ']';
    }

    void loadList() {
      for (int x = 0; x < items.size(); x++) {
        ILisch_Item item = items[x];
        printxy(colorCodeToString(format->returnColor(x)) + item.text, item.pos);
      }
    }

    void reloadList() {
      ILisch_Item current = items[selected.current];
      ILisch_Item last = items[selected.last];

      printxy(colorCodeToString(format->returnColor(selected.last)) + last.text, last.pos);
      printxy(colorCodeToString(format->returnColor(selected.current)) + current.text, current.pos);
    }

  public:
    vector<ILisch_Item> items;
    Lisch_base_Format* format;
    ILisch_Selected selected;

    Lisch() {
      cursorvsb(false);
      setlocale(LC_ALL, "Spanish");

      selected = { 0, 0 };
      changedItems = true;
      format = new Lisch_base_Format("select");
    }

    void setBuffer(COORD size, COORD pos) {
      format->buffer->setSize(size);
      format->buffer->setPos(pos);
    }

    void setStyles(ILisch_Styles styles) {
      format->styles = styles;
    }

    void start() {
      short int pressedKey;
      bool loop = true;

      format->selected = selected;
      if (changedItems) {
        format->setItems(items);
        changedItems = false;
      }

      format->loadItemsSize();
      format->loadItemsPosition();
      items = format->loadItemsStyles();

      loadList();

      while (loop) {
        pressedKey = getch();
        if (pressedKey == 0 || pressedKey == 0xe0) pressedKey = getch();

        if (loop) {
          selected.last = selected.current;

          if (pressedKey == KB_UP) selected.current = format->onKeyUp();
          else if (pressedKey == KB_LEFT) selected.current = format->onKeyLeft();
          else if (pressedKey == KB_DOWN) selected.current = format->onKeyDown();
          else if (pressedKey == KB_RIGHT) selected.current = format->onKeyRight();
          else if (pressedKey == KB_SPACE) {
            if (format->type == "select") loop = false;
            else if (format->type == "check") {
              items[selected.current].check = !items[selected.current].check;
              changedItems = true;
            }
          }
          else if (pressedKey == KB_ENTER) loop = false;

          if (selected.current != selected.last || format->type == "check") {
            format->items = items;
            format->selected = selected;
            reloadList();
          }
        }
      }

      end();
    }

    void pushItem(string text) {
      items.push_back({ text, { -1, -1 }, false });
      changedItems = true;
    }

};
