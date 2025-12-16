/*
  CS1B – Serendipity (Final)
  Partner A: Zee Richmond (1244256)
  Partner B: Alexander Jessen (A00186160)
  Date: 2025-12-12
  Purpose: Cashier module with multi-book receipts and oversell protection.
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp
         booktype.cpp menuutils.cpp -o serendipity
*/

#include "cashier.h"
#include "menuutils.h"
#include "invmenu.h"
#include "booktype.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

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

static int qtyAlreadyInCart(const int* indices, const int* qtys, int count, int targetIndex) {
  int total = 0;
  for (int i = 0; i < count; ++i) {
    if (indices[i] == targetIndex) total += qtys[i];
  }
  return total;
}

static bool promptYesNo(const string &prompt) {
  while (true) {
    cout << prompt;
    string response;
    getline(cin, response);
    if (cin.eof()) exit(0);
    // Simple trim logic
    size_t first = response.find_first_not_of(" \t");
    if (first == string::npos) continue;
    char c = static_cast<char>(tolower(response[first]));
    if (c == 'y') return true;
    if (c == 'n') return false;
    cout << "Please enter y or n.\n";
  }
}

static void renderCashierScreen(const string& date, int cartCount) {
  constexpr int innerWidth = 70;
  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Cashier Module • Active Checkout", innerWidth);
  menu::printMenuLine("Date: " + date, innerWidth);
  menu::printMenuLine("Items in cart: " + to_string(cartCount), innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Add books to the receipt, then confirm purchase.", innerWidth);
  menu::drawBorderLine(innerWidth);
  cout << "\n";
}

void cashier(OrderedLinkedList<bookType*>& inventory) {
  if (inventory.isEmpty()) {
      cout << "\nInventory is empty. Cannot process sales.\n";
      cout << "Press ENTER to return...";
      cin.get();
      return;
  }

  int capacity = 10;
  int count = 0;
  int* bookIndices = new int[capacity];
  int* purchaseQtys = new int[capacity];

  string date = menu::currentDate();

  while (true) {
      renderCashierScreen(date, count);
      cout << "--- Add Book to Receipt ---\n";
      // Use lookUpBook to find book
      // Note: lookUpBook returns a bookType pointer.
      // We need to find its "index" in the list to store in our "integer array"
      // or we can store the pointer casted to long long if identifiers are loose.
      // But let's stick to indices for strict compliance if possible, 
      // or better: let's ask the user to select the book via lookUpBook,
      // then we find its index.

      bookType* selectedBook = lookUpBook(inventory, true); // Selection mode = true

      if (selectedBook == nullptr) {
          if (count > 0) {
              cout << "\nFinished adding books.\n";
              break;
          } else {
              if (promptYesNo("No books selected. Exit cashier? (y/n): ")) {
                  delete[] bookIndices;
                  delete[] purchaseQtys;
                  return;
              }
              continue;
          }
      }

      // We have the book pointer. We need its index for the array.
      int index = 0;
      bool foundIndex = false;
      for (auto it = inventory.begin(); it != inventory.end(); ++it) {
          if (*it == selectedBook) {
              foundIndex = true;
              break;
          }
          index++;
      }

      if (!foundIndex) {
          // Should not happen if lookUpBook works correctly
          cout << "Error locating book in inventory.\n";
          continue;
      }

      int inCart = qtyAlreadyInCart(bookIndices, purchaseQtys, count, index);
      int remaining = selectedBook->getQtyOnHand() - inCart;
      if (remaining <= 0) {
          cout << "\nYou already have the maximum available quantity of this title in the cart.\n";
          if (!promptYesNo("Select a different book? (y/n): ")) {
              break;
          }
          continue;
      }

      cout << "\nSelected: " << selectedBook->getTitle() << "\n";
      cout << "Qty on Hand: " << selectedBook->getQtyOnHand() << "\n";
      cout << "In Cart (this title): " << inCart << "\n";
      cout << "Retail Price: $" << selectedBook->getRetail() << "\n";

      int qtyNeeded = 0;
      while(true) {
          cout << "Enter quantity to purchase (max " << remaining << "): ";
          if (cin >> qtyNeeded && qtyNeeded > 0 && qtyNeeded <= remaining) {
              flushLine();
              break;
          }
          cout << "Invalid quantity. Please enter between 1 and " << remaining << ".\n";
          flushLine();
      }

      // Add to cart
      if (count == capacity) {
          // Resize
          int newCap = capacity * 2;
          int* newIndices = new int[newCap];
          int* newQtys = new int[newCap];
          for(int i=0; i<capacity; ++i) {
              newIndices[i] = bookIndices[i];
              newQtys[i] = purchaseQtys[i];
          }
          delete[] bookIndices;
          delete[] purchaseQtys;
          bookIndices = newIndices;
          purchaseQtys = newQtys;
          capacity = newCap;
      }

      bookIndices[count] = index;
      purchaseQtys[count] = qtyNeeded;
      count++;

      cout << "\nBook added to cart.\n";
      if (!promptYesNo("Add another book? (y/n): ")) {
          break;
      }
  }

  // Process Receipt
  menu::clearScreen();
  cout << "Serendipity Booksellers\n";
  cout << "Date: " << date << "\n\n";

  cout << left << setw(4) << "Qty" << setw(14) << "ISBN" << setw(26) << "Title" 
       << right << setw(10) << "Price" << setw(10) << "Total" << endl;
  cout << string(64, '-') << endl;

  double subtotal = 0.0;

  for (int i = 0; i < count; ++i) {
      int idx = bookIndices[i];
      int qty = purchaseQtys[i];

      bookType* book = inventory.getAt(idx);
      if (!book) continue; // Should not happen

      double price = book->getRetail();
      double lineTotal = price * qty;
      subtotal += lineTotal;

      string title = book->getTitle();
      if (title.length() > 25) title = title.substr(0, 22) + "...";

      cout << left << setw(4) << qty
           << setw(14) << book->getISBN()
           << setw(26) << title
           << right << setw(10) << formatMoney(price)
           << setw(10) << formatMoney(lineTotal) << endl;
  }

  cout << endl;
  double tax = subtotal * 0.06;
  double total = subtotal + tax;

  cout << setw(54) << "Subtotal: " << setw(10) << formatMoney(subtotal) << endl;
  cout << setw(54) << "Tax: " << setw(10) << formatMoney(tax) << endl;
  cout << setw(54) << "Total: " << setw(10) << formatMoney(total) << endl;

  cout << "\n";
  if (promptYesNo("Confirm purchase? (y/n): ")) {
      // Update inventory
      for (int i = 0; i < count; ++i) {
          bookType* book = inventory.getAt(bookIndices[i]);
          if (book) {
              book->setQtyOnHand(book->getQtyOnHand() - purchaseQtys[i]);
          }
      }
      cout << "\nTransaction processed. Inventory updated.\n";
      cout << "Thank you for shopping at Serendipity Booksellers!\n";
  } else {
      cout << "\nTransaction cancelled.\n";
  }

  delete[] bookIndices;
  delete[] purchaseQtys;

  cout << "Press ENTER to return...";
  cin.get();
}
