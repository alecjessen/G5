/*
  CS1B – G2: Serendipity
  Partner A: Zee Richmond (1244256) — Partner: A
  Partner B: Alexander Jessen (A00186160) — Partner B
  Date: 2025‑09‑23
  Purpose: Creating interactive menus
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp
         booktype.cpp menuutils.cpp -o serendipity
*/

#include "menuutils.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

static string formatMoney(double value) {
  ostringstream oss;
  oss << fixed << setprecision(2) << value;
  return "$ " + oss.str();
}

static void flushLine() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
template <typename T> static T getNumber(const char *prompt) {
  T v{};
  while (true) {
    cout << prompt;
    if (cin >> v) {
      flushLine();
      return v;
    }
    cout << "Invalid input. Try again.\n";
    flushLine();
  }
}
static string getLineNonEmpty(const char *prompt) {
  string s;
  while (true) {
    cout << prompt;
    getline(cin, s);
    if (!s.empty())
      return s;
    cout << "Please enter a value.\n";
  }
}

void cashier() {
  menu::clearScreen();
  if (cin.rdbuf()->in_avail() > 0 && cin.peek() == '\n') {
    cin.ignore();
  }
  constexpr int innerWidth = 54;
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Cashier Module", innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Please enter the sales information below.", innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::drawBorderLine(innerWidth);
  cout << "\n";

  string date = getLineNonEmpty("Date (MM/DD/YY): ");
  int qty = getNumber<int>("Quantity of Book: ");
  string isbn = getLineNonEmpty("ISBN: ");
  string title = getLineNonEmpty("Title: ");
  double price = getNumber<double>("Unit Price: $");

  double merchandise = qty * price;
  double tax = merchandise * 0.06;
  double total = merchandise + tax;

  const int COL_QTY = 4;
  const int COL_ISBN = 20;
  const int COL_TITLE = 43;
  const int COL_PRICE = 12;
  const int COL_TOTAL = 12;

  cout << fixed << setprecision(2);

  cout << "\nSerendipity Book Sellers\n\n";
  cout << "Date: " << date << "\n\n";

  cout << left << setw(COL_QTY) << "Qty" << setw(COL_ISBN) << "ISBN"
       << setw(COL_TITLE) << "Title" << setw(COL_PRICE) << "Price"
       << setw(COL_TOTAL) << "Total" << "\n";

  int underlineLen = COL_QTY + COL_ISBN + COL_TITLE + COL_PRICE + COL_TOTAL;
  cout << string(underlineLen, '_') << "\n";

  auto truncatedTitle = title;
  const int maxTitleLen = COL_TITLE - 1;
  if (static_cast<int>(truncatedTitle.size()) > maxTitleLen) {
    truncatedTitle = truncatedTitle.substr(0, maxTitleLen - 1) + "…";
  }

  cout << left << setw(COL_QTY) << qty << setw(COL_ISBN) << isbn
       << setw(COL_TITLE) << truncatedTitle << right << setw(COL_PRICE)
       << formatMoney(price) << setw(COL_TOTAL) << formatMoney(merchandise)
       << "\n\n";

  int leftPad = COL_QTY + COL_ISBN + COL_TITLE;
  cout << string(leftPad, ' ') << left << setw(COL_PRICE) << "Subtotal" << right
       << setw(COL_TOTAL) << formatMoney(merchandise) << "\n";

  cout << string(leftPad, ' ') << left << setw(COL_PRICE) << "Tax" << right
       << setw(COL_TOTAL) << formatMoney(tax) << "\n";

  cout << string(leftPad, ' ') << left << setw(COL_PRICE) << "Total" << right
       << setw(COL_TOTAL) << formatMoney(total) << "\n\n";

  cout << "Thank You for Shopping at Serendipity!\n\n";
  cout << "Press ENTER to return to Main...";
  cin.get();
}
