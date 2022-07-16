#ifndef LISCH_TABLE_FORMAT_H
#define LISCH_TABLE_FORMAT_H

#include <iostream>
#include "base.h"
#include "../../vconio.h"

using namespace std;
using namespace vconio;


class Lisch_table_Format : public Lisch_base_Format {
  private:
    short int columns;
    short int rows;

    void end() {
      int item_column = 0;
      int item_row = 0;

      for (int x = 0; x < items.size(); x++) {
        item_column = x % columns;
        item_row = x / columns;

        items[x] = Table[item_column][item_row];
      }
    }

  public:
    vector<vector<ILisch_Item>> Table;

    Lisch_table_Format(string type) : Lisch_base_Format(type) {}

    virtual void setItems(vector<ILisch_Item> items) {
      Table.clear();

      int largerest = moreLargerItemLength(items, false);
      int width = largerest + styles.padding * 2;
      int item_column = 0;

      columns = short(this->buffer->size.X / width);
      if (columns > items.size()) columns = items.size();
      rows = short(items.size() / columns);

      for (int x = 0; x < columns; x++)
        Table.push_back({});

      for (int x = 0; x < items.size(); x++) {
        item_column = x % columns;

        Table[item_column].push_back(items[x]);
      }
    }

    virtual void loadItemsSize() {
      short item_column = 0;
      short item_row = 0;
      short spaceAdd = 0;
      short largerest = 0;

      for (vector<ILisch_Item> column : Table) {
        item_row = 0;
        largerest = moreLargerItemLength(column);

        for (ILisch_Item item : column) {
          string item_text = trim(item.text);
          spaceAdd = largerest - item_text.length();

          Table[item_column][item_row].text =
            repeatCh(' ', styles.padding) +
            item_text +
            repeatCh(' ', styles.padding + spaceAdd);

          item_row++;
        }

        item_column++;
      }

      end();
    }

    virtual void loadItemsPosition() {
      short item_column = 0;
      short item_row = 0;
      short left_margin = 0;
      short screenPadding = this->buffer->size.X;

      for (vector<ILisch_Item> column : Table)
        screenPadding -= column[0].text.length() - 1;

      screenPadding /= 2;
      screenPadding -= columns;

      for (vector<ILisch_Item> column : Table) {
        item_row = 0;

        for (ILisch_Item item : column) {
          Table[item_column][item_row].pos.Y = this->buffer->pos.Y + item_row;
          Table[item_column][item_row].pos.X = this->buffer->pos.X + screenPadding + left_margin + 1;

          item_row++;
        }

        left_margin += column[0].text.length() + 1;
        item_column++;
      }

      end();
    }

    virtual int onKeyUp() {
      if (selected.current / columns > 0)
        return selected.current - columns;
      return selected.current;
    }

    virtual int onKeyLeft() {
      if (selected.current % columns > 0)
        return selected.current - 1;
      return selected.current;
    }

    virtual int onKeyDown() {
      if (selected.current + columns <= items.size() - 1)
        return selected.current + columns;
      return selected.current;
    }

    virtual int onKeyRight() {
      if (selected.current % columns < columns - 1)
        if (selected.current + 1 <= items.size() - 1)
          return selected.current + 1;
      return selected.current;
    }

};

#endif
