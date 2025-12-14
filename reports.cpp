/*
  CS1B – Serendipity (Final)
  Partner A: Zee Richmond (1244256)
  Partner B: Alexander Jessen (A00186160)
  Date: 2025-12-12
  Purpose: Reports module with paginated listings and totals.
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp -o
  serendipity
*/

#include "reports.h"
#include "menuutils.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <termios.h>
#include <unistd.h>

using namespace std;

static string formatMoney(double value) {
  ostringstream oss;
  oss << fixed << setprecision(2) << value;
  return "$ " + oss.str();
}

int getch() {
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

int handlePaginationInput() {
  int c = getch();
  if (c == 27) {         // Escape sequence
    if (getch() == 91) { // '['
      switch (getch()) {
      case 65:
        return 1; // Up Arrow (Previous)
      case 66:
        return 0; // Down Arrow (Next)
      }
    }
  } else if (c == 'q' || c == 'Q') {
    return 2;
  }
  return 0;
}

static void clearScreen() {
  cout << "\033[H\033[2J";
  cout.flush();
}

static void waitForEnter() {
  cout << "Press Enter...";
  cin.get();
  clearScreen();
}

static int getChoiceInRange(int lo, int hi,
                            const string &prompt = "Enter your choice: ") {
  int choice;
  while (true) {
    cout << prompt;
    if (cin >> choice && choice >= lo && choice <= hi) {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return choice;
    }
    if (cin.eof())
      exit(0);
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid choice. Try again.\n";
  }
}

static void renderReportsMenuScreen(int highlight) {
  constexpr int innerWidth = 54;
  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Reports", innerWidth);
  menu::printEmptyLine(innerWidth);

  const vector<string> options = {
      "1. Inventory Listing",      "2. Inventory Wholesale Value",
      "3. Inventory Retail Value", "4. Listing by Quantity",
      "5. Listing by Cost",        "6. Listing by Age",
      "7. Return to Main Menu"};

  for (size_t i = 0; i < options.size(); ++i) {
    bool isHighlighted = (static_cast<int>(i) + 1 == highlight);
    menu::printMenuLine(options[i], innerWidth, isHighlighted);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Enter the menu number and press ENTER.", innerWidth);
  menu::drawBorderLine(innerWidth);
}

static string truncate(const string &str, size_t width) {
  if (str.length() > width) {
    return str.substr(0, width - 3) + "...";
  }
  return str;
}

// --- Report Implementations ---

void repListing(OrderedLinkedList<bookType *> &inventory) {
  if (inventory.isEmpty()) {
    cout << "\nInventory is empty.\n";
    waitForEnter();
    return;
  }

  int itemsPerPage = 20;
  int totalPages = std::ceil((double)inventory.size() / itemsPerPage);
  int currentPage = 1;

  while (true) {
    clearScreen();
    // Header
    cout << "Serendipity Booksellers" << endl;
    cout << "REPORT LISTING" << endl;
    cout << "Date: " << menu::currentDate() << "  Page " << currentPage
         << " of " << totalPages << endl;
    cout << "CURRENT BOOK COUNT: " << bookType::recordCount() << endl << endl;

    // Columns
    cout << left << setw(26) << "TITLE" << setw(14) << "ISBN" << setw(16)
         << "AUTHOR" << setw(16) << "PUBLISHER" << setw(11) << "DATE ADDED"
         << right << setw(9) << "QTY O/H" << "   " << left << setw(12)
         << "WHOLESALE" << setw(10) << "RETAIL" << endl;
    cout << string(117, '-') << endl;

    int startIndex = (currentPage - 1) * itemsPerPage;
    for (int i = 0; i < itemsPerPage; ++i) {
      bookType *book = inventory.getAt(startIndex + i);
      if (!book)
        break;

      cout << left << setw(26) << truncate(book->getTitle(), 25) << setw(14)
           << truncate(book->getISBN(), 13) << setw(16)
           << truncate(book->getAuthor(), 15) << setw(16)
           << truncate(book->getPub(), 15) << setw(11) << book->getDateAdded()
           << right << setw(9) << book->getQtyOnHand() << "   " << left
           << setw(12) << fixed << setprecision(2) << book->getWholesale()
           << setw(10) << book->getRetail() << endl;
    }

    cout << "\n[Down/Any: Next] [Up: Prev] [Q: Quit]\n";
    int action = handlePaginationInput();
    if (action == 2)
      break;           // Quit
    if (action == 0) { // Next
      if (currentPage == totalPages)
        currentPage = 1; // Rubber-band wrap
      else
        currentPage++;
    }
    if (action == 1) { // Prev
      if (currentPage == 1)
        currentPage = totalPages; // Rubber-band wrap
      else
        currentPage--;
    }
  }
}

void repWholesale(OrderedLinkedList<bookType *> &inventory) {
  if (inventory.isEmpty()) {
    cout << "\nInventory is empty.\n";
    waitForEnter();
    return;
  }

  int itemsPerPage = 10;
  int totalPages = std::ceil((double)inventory.size() / itemsPerPage);
  int currentPage = 1;
  double totalWholesale = 0.0;

  // Calculate total first
  for (auto it = inventory.begin(); it != inventory.end(); ++it) {
    totalWholesale += ((*it)->getWholesale() * (*it)->getQtyOnHand());
  }

  while (true) {
    clearScreen();
    cout << "Serendipity Booksellers" << endl;
    cout << "WHOLESALE VALUE LISTING" << endl;
    cout << "Date: " << menu::currentDate() << "  Page " << currentPage
         << " of " << totalPages << endl;
    cout << endl;

    cout << left << setw(40) << "TITLE" << setw(20) << "ISBN" << right
         << setw(10) << "QTY O/H" << "   " << setw(15) << "WHOLESALE COST"
         << endl;
    cout << string(88, '-') << endl;

    int startIndex = (currentPage - 1) * itemsPerPage;
    for (int i = 0; i < itemsPerPage; ++i) {
      bookType *book = inventory.getAt(startIndex + i);
      if (!book)
        break;

      cout << left << setw(40) << truncate(book->getTitle(), 38) << setw(20)
           << book->getISBN() << right << setw(10) << book->getQtyOnHand()
           << "   " << setw(15) << fixed << setprecision(2)
           << book->getWholesale() << endl;
    }

    if (currentPage == totalPages) {
      cout << endl;
      cout << left << setw(70) << "TOTAL WHOLESALE VALUE:" << right << setw(15)
           << formatMoney(totalWholesale) << endl;
    }

    cout << "\n[Down/Any: Next] [Up: Prev] [Q: Quit]\n";
    int action = handlePaginationInput();
    if (action == 2)
      break;
    if (action == 0) {
      if (currentPage == totalPages)
        currentPage = 1;
      else
        currentPage++;
    }
    if (action == 1) {
      if (currentPage == 1)
        currentPage = totalPages;
      else
        currentPage--;
    }
  }
}

void repRetail(OrderedLinkedList<bookType *> &inventory) {
  if (inventory.isEmpty()) {
    cout << "\nInventory is empty.\n";
    waitForEnter();
    return;
  }

  int itemsPerPage = 15;
  int totalPages = std::ceil((double)inventory.size() / itemsPerPage);
  int currentPage = 1;
  double totalRetail = 0.0;

  for (auto it = inventory.begin(); it != inventory.end(); ++it) {
    totalRetail += ((*it)->getRetail() * (*it)->getQtyOnHand());
  }

  while (true) {
    clearScreen();
    cout << "Serendipity Booksellers" << endl;
    cout << "RETAIL VALUE LISTING" << endl;
    cout << "Date: " << menu::currentDate() << "  Page " << currentPage
         << " of " << totalPages << endl;
    cout << endl;

    cout << left << setw(40) << "TITLE" << setw(20) << "ISBN" << right
         << setw(10) << "QTY O/H" << setw(15) << "RETAIL PRICE" << endl;
    cout << string(85, '-') << endl;

    int startIndex = (currentPage - 1) * itemsPerPage;
    for (int i = 0; i < itemsPerPage; ++i) {
      bookType *book = inventory.getAt(startIndex + i);
      if (!book)
        break;

      cout << left << setw(40) << truncate(book->getTitle(), 38) << setw(20)
           << book->getISBN() << right << setw(10) << book->getQtyOnHand()
           << setw(15) << fixed << setprecision(2) << book->getRetail() << endl;
    }

    if (currentPage == totalPages) {
      cout << endl;
      cout << left << setw(70) << "TOTAL RETAIL VALUE:" << right << setw(15)
           << formatMoney(totalRetail) << endl;
    }

    cout << "\n[Down/Any: Next] [Up: Prev] [Q: Quit]\n";
    int action = handlePaginationInput();
    if (action == 2)
      break;
    if (action == 0) {
      if (currentPage == totalPages)
        currentPage = 1;
      else
        currentPage++;
    }
    if (action == 1) {
      if (currentPage == 1)
        currentPage = totalPages;
      else
        currentPage--;
    }
  }
}

void repQty(OrderedLinkedList<bookType *> &inventory) {
  cout << "\n(Listing by Quantity is not available.\n";
  waitForEnter();
}

void repCost(OrderedLinkedList<bookType *> &inventory) {
  cout << "\n(Listing by Cost not fully implemented)\n";
  waitForEnter();
}

void repAge(OrderedLinkedList<bookType *> &inventory) {
  cout << "\n(Listing by Age not fully implemented)\n";
  waitForEnter();
}

void reports(OrderedLinkedList<bookType *> &inventory) {
  int choice = 0;
  int highlight = 1;

  do {
    renderReportsMenuScreen(highlight);
    choice = getChoiceInRange(1, 7, "\nEnter selection (1-7): ");
    highlight = choice;

    switch (choice) {
    case 1:
      repListing(inventory);
      break;
    case 2:
      repWholesale(inventory);
      break;
    case 3:
      repRetail(inventory);
      break;
    case 4:
      repQty(inventory);
      break;
    case 5:
      repCost(inventory);
      break;
    case 6:
      repAge(inventory);
      break;
    case 7:
      break;
    default:
      cout << "Invalid choice. Enter 1-7.\n";
      break;
    }
  } while (choice != 7);
}
