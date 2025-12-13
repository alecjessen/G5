/*
  CS1B – G2: Serendipity
  Partner A: Zee Richmond (1244256) — Partner: A
  Partner B: Alexander Jessen (A00186160) — Partner B
  Date: 2025‑09‑23
  Purpose: Creating interactive menus
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp
         booktype.cpp menuutils.cpp -o serendipity
*/

#include "cashier.h"
#include "invmenu.h"
#include "menuutils.h"
#include "reports.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

namespace {
void flushLine() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getChoiceInRange(int lo, int hi,
                     const string &prompt = "Enter your choice: ") {
  int choice;
  while (true) {
    cout << prompt;
    if (cin >> choice && choice >= lo && choice <= hi) {
      flushLine();
      return choice;
    }
    cout << "Invalid choice. Try again.\n";
    flushLine();
  }
}

void renderMainMenuScreen(int highlight) {
  constexpr int innerWidth = 48;
  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Main Menu", innerWidth);
  menu::printEmptyLine(innerWidth);

  const vector<string> options = {"1. Cashier Module",   "2. Inventory Database",
                                  "3. Reports",          "4. Exit"};

  for (size_t i = 0; i < options.size(); ++i) {
    bool highlightOption = (static_cast<int>(i) + 1 == highlight);
    menu::printMenuLine(options[i], innerWidth, highlightOption);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Enter the menu number and press ENTER.", innerWidth);
  menu::drawBorderLine(innerWidth);
}
} // namespace

int main() {
  int highlight = 1;
  int choice = 0;
  do {
    renderMainMenuScreen(highlight);
    choice = getChoiceInRange(1, 4, "\nEnter selection (1-4): ");
    highlight = choice;

    switch (choice) {
    case 1:
      cashier();
      break;
    case 2:
      invMenu();
      break;
    case 3:
      reports();
      break;
    case 4:
      cout << "Exiting..." << endl;
      break;
    default:
      cout << "Invalid Choice." << endl;
    }
  } while (choice != 4);

  return 0;
}
