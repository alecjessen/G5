/*
  CS1B – Serendipity (Final)
  Partner A: Zee Richmond (1244256)
  Partner B: Alexander Jessen (A00186160)
  Date: 2025-12-12
  Purpose: Inventory database menus (lookup/add/edit/delete) using ordered list.
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp
         booktype.cpp bookinfo.cpp menuutils.cpp -o serendipity
*/

#include "bookinfo.h"
#include "menuutils.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {
string moneyString(float value) {
  ostringstream oss;
  oss << fixed << setprecision(2) << value;
  return "$ " + oss.str();
}

string buildFieldLine(const string &label, const string &value,
                      int labelWidth) {
  ostringstream oss;
  oss << left << setw(labelWidth) << (label + ":") << value;
  return oss.str();
}
} // namespace

void bookInfo(const bookType &book) {
  constexpr int labelWidth = 16;
  vector<pair<string, string>> fields = {
      {"Title", book.getTitle()},
      {"ISBN", book.getISBN()},
      {"Author", book.getAuthor()},
      {"Publisher", book.getPub()},
      {"Date Added", book.getDateAdded()},
      {"Quantity", to_string(book.getQtyOnHand())},
      {"Wholesale", moneyString(book.getWholesale())},
      {"Retail", moneyString(book.getRetail())}};

  int innerWidth = 70;
  vector<string> lines;
  lines.reserve(fields.size());
  for (const auto &field : fields) {
    string line = buildFieldLine(field.first, field.second, labelWidth);
    lines.push_back(line);
    innerWidth = max(innerWidth, static_cast<int>(line.size()));
  }

  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Book Information", innerWidth);
  menu::printEmptyLine(innerWidth);

  for (const string &line : lines) {
    menu::printMenuLine(line, innerWidth);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Press ENTER to return...", innerWidth);
  menu::drawBorderLine(innerWidth);

  cin.get();
}
