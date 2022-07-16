#ifndef LISCH_BASE_FORMAT_H
#define LISCH_BASE_FORMAT_H

#include <iostream>
#include <string>
#include <vector>
#include "../../vconio.h"
#include "../types.h"
#include "../Functions.h"

using namespace std;
using namespace vconio;


ILisch_Styles Lisch_CreateStyle() {
  ILisch_Styles styles = {
    "center",
    3,
    {
      0x08,
      0x0f
    },
    {
      0x04,
      0x02,
      0x0c,
      0x0a
    }
  };

  return styles;
}

class Lisch_base_Format {
  public:
    string type;
    ILisch_Selected selected;
    vector<ILisch_Item> items;
    Buffer* buffer;
    ILisch_Styles styles;

    Lisch_base_Format(string type) {
      this->type = type;
      this->styles = Lisch_CreateStyle();
      this->buffer = new Buffer({ 30, 10 }, { 2, 3 }, 0x8f);
    }

    virtual void setItems(vector<ILisch_Item> items) {
      this->items = items;
    }

    short returnColor(short index) {
      if (type == "select") {
        if (index == selected.current)
          return styles.select.focus_color;
        else
          return styles.select.color;
      }
      else if (type == "check") {
        if (index == selected.current) {
          if (items[index].check) return styles.check.focus_check_color;
          else return styles.check.focus_color;
        }
        else {
          if (items[index].check) return styles.check.check_color;
          else return styles.check.color;
        }
      }

      return 0x0f;
    }

    virtual void loadItemsSize() {}
    virtual void loadItemsPosition() {}

    vector<ILisch_Item> loadItemsStyles() {
      int item_padding = 3;
      int spaceAdd = 0;

      if (styles.text_align == "center") {
        bool spaceAdd_isImpar = false;

        for (int x = 0; x < items.size(); x++) {
          string item_trim = trim(items[x].text);

          spaceAdd = items[x].text.length() - item_trim.length() - item_padding * 2;
          spaceAdd_isImpar = false;

          if (spaceAdd > 0) {
            if (spaceAdd % 2 != 0) {
              spaceAdd--;
              spaceAdd_isImpar = true;
            }
          }

          spaceAdd /= 2;

          items[x].text =
            repeatCh(' ', spaceAdd + item_padding) +
            item_trim +
            repeatCh(' ', spaceAdd + item_padding) +
            (spaceAdd_isImpar ? " " : "");
        }
      }
      else if (styles.text_align == "right") {
        for (int x = 0; x < items.size(); x++) {
          string item_trim = trim(items[x].text);

          spaceAdd = items[x].text.length() - item_trim.length() - item_padding * 2;

          items[x].text =
            repeatCh(' ', spaceAdd + item_padding) +
            item_trim +
            repeatCh(' ', item_padding);
        }
      }

      return items;
    }

    virtual int onKeyUp() { return selected.current; }
    virtual int onKeyDown() { return selected.current; }
    virtual int onKeyLeft() { return selected.current; }
    virtual int onKeyRight() { return selected.current; }
};

#endif
