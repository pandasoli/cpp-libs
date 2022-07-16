#ifndef LISCH_LIST_FORMAT_H
#define LISCH_LIST_FORMAT_H

#include "base.h"
#include "../../vconio.h"

using namespace std;
using namespace vconio;


class Lisch_list_Format : public Lisch_base_Format {
  public:
    Lisch_list_Format(string type) : Lisch_base_Format(type) {}

    virtual void loadItemsSize() {
      int item_padding = 3;
      int spaceAdd = 0;
      int moreLargerLength = moreLargerItemLength(items);

      for (int x = 0; x < items.size(); x++) {
        string item_text = trim(items[x].text);
        spaceAdd = moreLargerLength - item_text.length();

        items[x].text =
          repeatCh(' ', item_padding) +
          item_text +
          repeatCh(' ', item_padding + spaceAdd);
      }
    }

    virtual void loadItemsPosition() {
      int screenPadding = (this->buffer->size.X / 2) - (items[0].text.length() / 2);

      for (int x = 0; x < items.size(); x++) {
        items[x].pos.X = this->buffer->pos.X + screenPadding;
        items[x].pos.Y = this->buffer->pos.Y + x;
      }
    }

    virtual int onKeyUp() {
      short nSelected = selected.current - 1;

      if (selected.current > 0) {
        while (trim(items[nSelected].text) == "" && nSelected > 0)
          nSelected--;

        return nSelected;
      }
      return selected.current;
    }

    virtual int onKeyDown() {
      short nSelected = selected.current + 1;

      if (selected.current < items.size() - 1) {
        while (trim(items[nSelected].text) == "" && nSelected < items.size() - 1)
          nSelected++;

        return nSelected;
      }
      return selected.current;
    }

};

#endif
