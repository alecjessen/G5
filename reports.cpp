/*
  CS1B – G2: Serendipity
  Partner A: Zee Richmond (1244256) — Partner: A
  Partner B: Alexander Jessen (A00186160) — Partner B
  Date: 2025‑09‑23
  Purpose: Creating interactive menus
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp -o
  serendipity
*/

#include "reports.h"
#include <iostream>
#include <limits>

using namespace std;

static void clearScreen() {
  cout << "\033[H\033[2J";
  cout.flush();
}

static void waitForEnter() {
  cout << "Press Enter...";
  cin.get();
  // cin.ignore(numeric_limits<streamsize>::max(), '\n');
  clearScreen();
}

void repListing() {
  cout << "\nrepListing stub\n";
  waitForEnter();
}
void repWholesale() {
  cout << "\nrepWholesale stub\n";
  waitForEnter();
}
void repRetail() {
  cout << "\nrepRetail stub\n";
  waitForEnter();
}
void repQty() {
  cout << "\nrepQty stub\n";
  waitForEnter();
}
void repCost() {
  cout << "\nrepCost stub\n";
  waitForEnter();
}
void repAge() {
  cout << "\nrepAge stub\n";
  waitForEnter();
}

void reports() {
  int choice = 0;

  do {
    clearScreen();
    cout << "Serendipity Booksellers\n"
         << "Reports \n\n"
         << "1. Inventory Listing\n"
         << "2. Inventory Wholesale Value\n"
         << "3. Inventory Retail Value\n"
         << "4. Listing by Quantity\n"
         << "5. Listing by Cost\n"
         << "6. Listing by Age\n"
         << "7. Return to Main Menu\n\n"
         << "Enter choice (1-7): ";

    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid input. Try again.\n";
      continue;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // eat trailing newline

    switch (choice) {
    case 1:
      repListing();
      break;
    case 2:
      repWholesale();
      break;
    case 3:
      repRetail();
      break;
    case 4:
      repQty();
      break;
    case 5:
      repCost();
      break;
    case 6:
      repAge();
      break;
    case 7:
      break;
    default:
      cout << "Invalid choice. Enter 1-7.\n";
      break;
    }
  } while (choice != 7);
}
