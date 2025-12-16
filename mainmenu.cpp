/*
  CS1B – Serendipity (Final)
  Partner A: Zee Richmond (1244256)
  Partner B: Alexander Jessen (A00186160)
  Date: 2025-12-12
  Purpose: Main menu and data loading for the Serendipity bookstore.
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp
         booktype.cpp menuutils.cpp -o serendipity
*/

#include "booktype.h"
#include "cashier.h"
#include "invmenu.h"
#include "menuutils.h"
#include "orderedLinkedList.h"
#include "reports.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {
void flushLine() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string trim(const string &text) {
  size_t start = text.find_first_not_of(" \t\n\r");
  size_t end = text.find_last_not_of(" \t\n\r");
  if (start == string::npos)
    return "";
  return text.substr(start, end - start + 1);
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
    if (cin.eof())
      exit(0); // Safety exit on EOF
    cin.clear();
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

  const vector<string> options = {"1. Cashier Module", "2. Inventory Database",
                                  "3. Reports", "4. Exit"};

  for (size_t i = 0; i < options.size(); ++i) {
    bool highlightOption = (static_cast<int>(i) + 1 == highlight);
    menu::printMenuLine(options[i], innerWidth, highlightOption);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Enter the menu number and press ENTER.", innerWidth);
  menu::drawBorderLine(innerWidth);
}

void loadData(OrderedLinkedList<bookType *> &inventory) {
  ifstream inFile("prog.out");
  if (!inFile) {
    cerr << "Warning: prog.out not found. Starting with empty inventory.\n";
    if (cin.rdbuf()->in_avail() > 0)
      cin.ignore();
    return;
  }

  string line;
  while (true) {
    if (!getline(inFile, line))
      break;
    if (trim(line).empty())
      continue;

    string title, isbn, author, pub, date;
    string qtyStr, wholeStr, retailStr;

    // Read Title
    if (!getline(inFile, title))
      break;

    // Read marker for ISBN (expect "2")
    getline(inFile, line);
    // Read ISBN
    if (!getline(inFile, isbn))
      break;

    // Read marker for Author (expect "3")
    getline(inFile, line);
    // Read Author
    if (!getline(inFile, author))
      break;

    // Read marker for Publisher (expect "4")
    getline(inFile, line);
    // Read Publisher
    if (!getline(inFile, pub))
      break;

    // Read marker for Date (expect "5")
    getline(inFile, line);
    // Read Date
    if (!getline(inFile, date))
      break;

    // Read marker for Qty (expect "6")
    getline(inFile, line);
    // Read Qty
    if (!getline(inFile, qtyStr))
      break;

    // Read marker for Wholesale (expect "7")
    getline(inFile, line);
    // Read Wholesale
    if (!getline(inFile, wholeStr))
      break;

    // Read marker for Retail (expect "8")
    getline(inFile, line);
    // Read Retail
    if (!getline(inFile, retailStr))
      break;

    // Read marker for Save (expect "9")
    getline(inFile, line);

    try {
      int qty = stoi(trim(qtyStr));
      float wholesale = stof(trim(wholeStr));
      float retail = stof(trim(retailStr));

      // Insert into ordered list
      // Note: sortCode defaults to 0 (Title) initially
      inventory.insert(new bookType(trim(isbn), trim(title), trim(author),
                                    trim(pub), trim(date), qty, wholesale,
                                    retail));
    } catch (...) {
      // malformed line, skip
    }
  }
  inFile.close();
}

} // namespace

int main() {
  OrderedLinkedList<bookType *> inventory;

  // Load initial data
  loadData(inventory);

  int highlight = 1;
  int choice = 0;
  do {
    renderMainMenuScreen(highlight);
    choice = getChoiceInRange(1, 4, "\nEnter selection (1-4): ");
    highlight = choice;

    switch (choice) {
    case 1:
      cashier(inventory);
      break;
    case 2:
      invMenu(inventory);
      break;
    case 3:
      reports(inventory);
      break;
    case 4:
      cout << "Exiting..." << endl;
      break;
    default:
      cout << "Invalid Choice." << endl;
    }
  } while (choice != 4);

  // Cleanup memory
  for (auto it = inventory.begin(); it != inventory.end(); ++it) {
    delete *it;
  }

  return 0;
}
