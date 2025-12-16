/*
  CS1B – Serendipity (Final)
  Partner A: Zee Richmond (1244256)
  Partner B: Alexander Jessen (A00186160)
  Date: 2025-12-12
  Purpose: Inventory database menus (lookup/add/edit/delete) using ordered list.
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp
         booktype.cpp bookinfo.cpp menuutils.cpp -o serendipity
*/

#include "invmenu.h"
#include "bookinfo.h"
#include "booktype.h"
#include "menuutils.h"
#include "orderedLinkedList.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static void flushLine() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int getChoiceInRange(int lo, int hi,
                            const string &prompt = "Enter your choice: ") {
  int choice;
  while (true) {
    cout << prompt;
    if (cin >> choice && choice >= lo && choice <= hi) {
      flushLine();
      return choice;
    }
    if (cin.eof())
      exit(0);
    cin.clear();
    cout << "Invalid choice. Try again.\n";
    flushLine();
  }
}

namespace {
constexpr size_t kMaxBooks = 20;

string trim(const string &text) {
  size_t start = text.find_first_not_of(" \t\n\r");
  size_t end = text.find_last_not_of(" \t\n\r");
  if (start == string::npos)
    return "";
  return text.substr(start, end - start + 1);
}

string formatAmount(bool isSet, float value) {
  ostringstream out;
  out << fixed << setprecision(2) << (isSet ? value : 0.0f);
  return out.str();
}

bool isValidDate(const string &value) {
  if (value.size() != 10)
    return false;
  if (!isdigit(value[0]) || !isdigit(value[1]) || value[2] != '/' ||
      !isdigit(value[3]) || !isdigit(value[4]) || value[5] != '/' ||
      !isdigit(value[6]) || !isdigit(value[7]) || !isdigit(value[8]) ||
      !isdigit(value[9]))
    return false;

  int month = stoi(value.substr(0, 2));
  int day = stoi(value.substr(3, 2));
  int year = stoi(value.substr(6, 4));

  if (month < 1 || month > 12)
    return false;
  if (day < 1 || day > 31)
    return false;
  if (year < 0)
    return false;

  return true;
}

string promptNonEmptyString(const string &prompt) {
  while (true) {
    cout << prompt;
    string input;
    getline(cin, input);
    if (cin.eof())
      exit(0);
    if (!input.empty())
      return input;
    cout << "Input cannot be blank. Please try again.\n";
  }
}

int promptNonNegativeInt(const string &prompt) {
  while (true) {
    cout << prompt;
    string line;
    getline(cin, line);
    if (cin.eof())
      exit(0);
    stringstream ss(line);
    int value;
    char leftover;
    if (ss >> value && !(ss >> leftover) && value >= 0)
      return value;
    cout << "Please enter a non-negative whole number.\n";
  }
}

float promptNonNegativeFloat(const string &prompt) {
  while (true) {
    cout << prompt;
    string line;
    getline(cin, line);
    if (cin.eof())
      exit(0);
    stringstream ss(line);
    float value;
    char leftover;
    if (ss >> value && !(ss >> leftover) && value >= 0.0f)
      return value;
    cout << "Please enter a non-negative value.\n";
  }
}

string promptDate(const string &prompt) {
  while (true) {
    cout << prompt;
    string line;
    getline(cin, line);
    if (cin.eof())
      exit(0);
    if (isValidDate(line))
      return line;
    cout << "Please enter a date in mm/dd/yyyy format.\n";
  }
}

bool promptYesNo(const string &prompt) {
  while (true) {
    cout << prompt;
    string response;
    getline(cin, response);
    if (cin.eof())
      exit(0);
    response = trim(response);
    if (response.empty())
      continue;
    char c = static_cast<char>(tolower(response[0]));
    if (c == 'y')
      return true;
    if (c == 'n')
      return false;
    cout << "Please enter y or n.\n";
  }
}

struct DraftBook {
  string title;
  bool titleSet = false;
  string isbn;
  bool isbnSet = false;
  string author;
  bool authorSet = false;
  string publisher;
  bool publisherSet = false;
  string dateAdded;
  bool dateSet = false;
  int quantity = 0;
  bool quantitySet = false;
  float wholesale = 0.0f;
  bool wholesaleSet = false;
  float retail = 0.0f;
  bool retailSet = false;

  bool anyFieldSet() const {
    return titleSet || isbnSet || authorSet || publisherSet || dateSet ||
           quantitySet || wholesaleSet || retailSet;
  }

  bool readyToSave() const {
    return titleSet && isbnSet && authorSet && publisherSet && dateSet &&
           quantitySet && wholesaleSet && retailSet;
  }
};

DraftBook makeDraftFromBook(const bookType *book) {
  DraftBook draft;
  draft.title = book->getTitle();
  draft.titleSet = true;
  draft.isbn = book->getISBN();
  draft.isbnSet = true;
  draft.author = book->getAuthor();
  draft.authorSet = true;
  draft.publisher = book->getPub();
  draft.publisherSet = true;
  draft.dateAdded = book->getDateAdded();
  draft.dateSet = true;
  draft.quantity = book->getQtyOnHand();
  draft.quantitySet = true;
  draft.wholesale = book->getWholesale();
  draft.wholesaleSet = true;
  draft.retail = book->getRetail();
  draft.retailSet = true;
  return draft;
}

void applyDraftToBook(bookType *book, const DraftBook &draft) {
  book->setTitle(draft.title);
  book->setISBN(draft.isbn);
  book->setAuthor(draft.author);
  book->setPub(draft.publisher);
  book->setDateAdded(draft.dateAdded);
  book->setQtyOnHand(draft.quantity);
  book->setWholesale(draft.wholesale);
  book->setRetail(draft.retail);
}

void printBookSummary(const bookType *book) {
  ios::fmtflags oldFlags = cout.flags();
  streamsize oldPrecision = cout.precision();

  cout << "\nTitle: " << book->getTitle() << '\n';
  cout << "ISBN: " << book->getISBN() << '\n';
  cout << "Author: " << book->getAuthor() << '\n';
  cout << "Publisher: " << book->getPub() << '\n';
  cout << "Date Added: " << book->getDateAdded() << '\n';
  cout << "Quantity on Hand: " << book->getQtyOnHand() << '\n';
  cout << fixed << setprecision(2);
  cout << "Wholesale: $" << book->getWholesale() << '\n';
  cout << "Retail: $" << book->getRetail() << '\n';

  cout.flags(oldFlags);
  cout.precision(oldPrecision);
}

void waitForEnter() {
  cout << "\nPress ENTER to continue...";
  cin.get();
}

string toLower(string text) {
  transform(text.begin(), text.end(), text.begin(),
            [](unsigned char c) { return static_cast<char>(tolower(c)); });
  return text;
}

bool hasDuplicateISBN(OrderedLinkedList<bookType *> &inventory,
                      const string &isbn, bookType *skipBook = nullptr) {
  string target = toLower(isbn);
  for (auto it = inventory.begin(); it != inventory.end(); ++it) {
    if (*it == skipBook)
      continue;
    if (toLower((*it)->getISBN()) == target)
      return true;
  }
  return false;
}

string ellipsize(const string &text, int maxLen) {
  if (maxLen <= 1 || static_cast<int>(text.size()) <= maxLen)
    return text;
  return text.substr(0, maxLen - 1) + "…";
}

string buildFieldSummaryText(const string &label, const string &value,
                             int rightWidth) {
  int available = max(1, rightWidth - static_cast<int>(label.size()) - 3);
  string truncated = ellipsize(value, available);
  return label + " : " + truncated;
}

void renderInventoryMenuScreen(int highlight) {
  constexpr int innerWidth = 54;
  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Inventory Database", innerWidth);
  menu::printEmptyLine(innerWidth);

  const vector<string> options = {"1. Look Up a Book", "2. Add a Book",
                                  "3. Edit a Book's Record", "4. Delete a Book",
                                  "5. Return to the Main Menu"};

  for (size_t i = 0; i < options.size(); ++i) {
    bool isHighlighted = (static_cast<int>(i) + 1 == highlight);
    menu::printMenuLine(options[i], innerWidth, isHighlighted);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Enter the menu number and press ENTER.", innerWidth);
  menu::drawBorderLine(innerWidth);
}

void renderAddBookScreen(const DraftBook &draft, int highlight,
                         const string &footer) {
  constexpr int innerWidth = 66;
  constexpr int leftWidth = 32;
  constexpr int rightWidth = innerWidth - leftWidth - 3;

  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Add a Book", innerWidth);
  menu::printEmptyLine(innerWidth);
  ostringstream meta;
  meta << "DATABASE SIZE: " << kMaxBooks
       << "   CURRENT COUNT: " << bookType::recordCount();
  menu::printMenuLine(ellipsize(meta.str(), innerWidth), innerWidth);
  menu::printEmptyLine(innerWidth);

  const vector<string> menuItems = {"1. Enter Book Title",
                                    "2. Enter ISBN",
                                    "3. Enter Author",
                                    "4. Enter Publisher",
                                    "5. Enter Date Added (mm/dd/yyyy)",
                                    "6. Enter Quantity on Hand",
                                    "7. Enter Wholesale Cost",
                                    "8. Enter Retail Price",
                                    "9. Save Book to Database",
                                    "0. Return to Inventory Menu"};

  const vector<pair<string, string>> fieldSummaries = {
      {"Title", draft.titleSet ? draft.title : "UNSET"},
      {"ISBN", draft.isbnSet ? draft.isbn : "UNSET"},
      {"Author", draft.authorSet ? draft.author : "UNSET"},
      {"Publisher", draft.publisherSet ? draft.publisher : "UNSET"},
      {"Date Added", draft.dateSet ? draft.dateAdded : "UNSET"},
      {"QOH", draft.quantitySet ? to_string(draft.quantity) : "0"},
      {"Wholesale Value", formatAmount(draft.wholesaleSet, draft.wholesale)},
      {"Retail Value", formatAmount(draft.retailSet, draft.retail)}};

  for (size_t i = 0; i < menuItems.size(); ++i) {
    bool isHighlighted = (static_cast<int>(i) + 1 == highlight);
    string rightText;
    if (i < fieldSummaries.size()) {
      const string &label = fieldSummaries[i].first;
      const string &value = fieldSummaries[i].second;
      rightText = buildFieldSummaryText(label, value, rightWidth);
    }
    menu::printDualColumnLine(menuItems[i], isHighlighted, leftWidth, rightText,
                              rightWidth, innerWidth);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine(footer, innerWidth);
  menu::drawBorderLine(innerWidth);
}

void renderEditBookScreen(const DraftBook &draft, int highlight,
                          const string &footer) {
  const vector<string> menuItems = {
      "1. Edit Book Title",     "2. Edit ISBN",
      "3. Edit Author",         "4. Edit Publisher",
      "5. Edit Date Added",     "6. Edit Quantity on Hand",
      "7. Edit Wholesale Cost", "8. Edit Retail Price",
      "9. Save Changes",        "0. Cancel Editing"};

  const vector<pair<string, string>> fieldSummaries = {
      {"Title", draft.titleSet ? draft.title : "UNSET"},
      {"ISBN", draft.isbnSet ? draft.isbn : "UNSET"},
      {"Author", draft.authorSet ? draft.author : "UNSET"},
      {"Publisher", draft.publisherSet ? draft.publisher : "UNSET"},
      {"Date Added", draft.dateSet ? draft.dateAdded : "UNSET"},
      {"QOH", draft.quantitySet ? to_string(draft.quantity) : "0"},
      {"Wholesale Value", formatAmount(draft.wholesaleSet, draft.wholesale)},
      {"Retail Value", formatAmount(draft.retailSet, draft.retail)}};

  int leftWidth = 0;
  for (const string &item : menuItems) {
    leftWidth = max(leftWidth, static_cast<int>(item.size()));
  }
  leftWidth = max(leftWidth, 32);

  int rightWidth = 0;
  for (const auto &summary : fieldSummaries) {
    string composed = summary.first + " : " + summary.second;
    rightWidth = max(rightWidth, static_cast<int>(composed.size()));
  }
  rightWidth = max(rightWidth, 32);

  string editingLine =
      "Editing: " + (draft.titleSet ? draft.title : "Title not set");

  int innerWidth = leftWidth + 3 + rightWidth;
  innerWidth = max(innerWidth, static_cast<int>(editingLine.size()));
  innerWidth = max(innerWidth, static_cast<int>(footer.size()));
  innerWidth = max(innerWidth, 66);

  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Edit Book Record", innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::printMenuLine(editingLine, innerWidth);
  menu::printEmptyLine(innerWidth);

  for (size_t i = 0; i < menuItems.size(); ++i) {
    bool isHighlighted = (static_cast<int>(i) + 1 == highlight);
    string rightText;
    if (i < fieldSummaries.size()) {
      const string &label = fieldSummaries[i].first;
      const string &value = fieldSummaries[i].second;
      rightText = buildFieldSummaryText(label, value, rightWidth);
    }
    menu::printDualColumnLine(menuItems[i], isHighlighted, leftWidth, rightText,
                              rightWidth, innerWidth);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine(footer, innerWidth);
  menu::drawBorderLine(innerWidth);
}

void renderLookUpMenuScreen(int highlight) {
  constexpr int innerWidth = 66;
  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Look Up Book", innerWidth);
  menu::printEmptyLine(innerWidth);

  const vector<string> options = {"1. Search Inventory (Title or ISBN)",
                                  "2. Return to Inventory Menu"};

  for (size_t i = 0; i < options.size(); ++i) {
    bool isHighlighted = (static_cast<int>(i) + 1 == highlight);
    menu::printMenuLine(options[i], innerWidth, isHighlighted);
  }

  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Enter 1 to search or 2 to return.", innerWidth);
  menu::drawBorderLine(innerWidth);
}

vector<bookType *> searchInventory(OrderedLinkedList<bookType *> &inventory,
                                   const string &query) {
  vector<bookType *> matches;
  string target = toLower(query);

  for (auto it = inventory.begin(); it != inventory.end(); ++it) {
    bookType *book = *it;
    string lowerTitle = toLower(book->getTitle());
    string lowerISBN = toLower(book->getISBN());
    if (lowerTitle.find(target) != string::npos ||
        lowerISBN.find(target) != string::npos) {
      matches.push_back(book);
    }
  }
  return matches;
}

void showNoMatches(const string &query) {
  constexpr int innerWidth = 64;
  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Look Up Book", innerWidth);
  menu::printEmptyLine(innerWidth);
  string message = "No matches for \"" + query + "\".";
  menu::printMenuLine(ellipsize(message, innerWidth), innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Book not in inventory.", innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::printMenuLine("Press ENTER to return...", innerWidth);
  menu::drawBorderLine(innerWidth);
  cin.get();
}

// Locate the node that holds the given book pointer; optionally return hop
// count.
Node<bookType *> *findNodeForBook(OrderedLinkedList<bookType *> &inventory,
                                  bookType *target, int *hopCount = nullptr) {
  Node<bookType *> *node = inventory.headPtr();
  int hops = 0;
  while (node) {
    if (node->data == target) {
      if (hopCount)
        *hopCount = hops;
      return node;
    }
    node = node->next;
    ++hops;
  }
  if (hopCount)
    *hopCount = -1;
  return nullptr;
}

// Returns pointer to selected book or nullptr
bookType *displaySearchResults(const vector<bookType *> &matches,
                               const string &query, bool selectionMode) {
  vector<string> resultLines;
  resultLines.reserve(matches.size());

  int innerWidth = 70;
  for (size_t i = 0; i < matches.size(); ++i) {
    const bookType *book = matches[i];
    string line = to_string(i + 1) + ". " + book->getTitle() +
                  "  [ISBN: " + book->getISBN() + "]";
    innerWidth = max(innerWidth, static_cast<int>(line.size()));
    resultLines.push_back(line);
  }

  ostringstream summaryStream;
  summaryStream << matches.size() << " match(es) for \"" << query
                << "\" (Title/ISBN).";
  string summaryText = summaryStream.str();
  innerWidth = max(innerWidth, static_cast<int>(summaryText.size()));

  const string instructionText =
      selectionMode ? "Enter result number (0 to cancel selection)."
                    : "Enter result number (0 to return).";
  innerWidth = max(innerWidth, static_cast<int>(instructionText.size()));

  while (true) {
    menu::clearScreen();
    menu::drawBorderLine(innerWidth);
    menu::printCenteredLine("Serendipity Booksellers", innerWidth);
    menu::printCenteredLine("Search Results", innerWidth);
    menu::printEmptyLine(innerWidth);

    menu::printMenuLine(summaryText, innerWidth);
    menu::printEmptyLine(innerWidth);

    for (const string &line : resultLines) {
      menu::printMenuLine(line, innerWidth);
    }

    menu::printEmptyLine(innerWidth);
    menu::printMenuLine(instructionText, innerWidth);
    menu::drawBorderLine(innerWidth);
    cout << "\n";

    int selection = getChoiceInRange(
        0, static_cast<int>(matches.size()),
        matches.empty()
            ? "Select (0 to cancel): "
            : "Select (0 to cancel, 1-" + to_string(matches.size()) + "): ");
    if (selection == 0) {
      return nullptr;
    }

    bookType *selected = matches[static_cast<size_t>(selection - 1)];
    if (selectionMode) {
      return selected;
    }
    bookInfo(*selected);
  }
}

bookType *performSearch(OrderedLinkedList<bookType *> &inventory,
                        bool selectionMode) {
  constexpr int innerWidth = 66;
  menu::clearScreen();
  menu::drawBorderLine(innerWidth);
  menu::printCenteredLine("Serendipity Booksellers", innerWidth);
  menu::printCenteredLine("Search Inventory", innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::printMenuLine(
      "Enter title keywords or ISBN fragment, then press ENTER.", innerWidth);
  menu::printEmptyLine(innerWidth);
  menu::drawBorderLine(innerWidth);
  cout << "\n";

  string query =
      trim(promptNonEmptyString("Enter title keywords or ISBN fragment: "));
  vector<bookType *> matches = searchInventory(inventory, query);

  if (matches.empty()) {
    showNoMatches(query);
    return nullptr;
  }

  return displaySearchResults(matches, query, selectionMode);
}

} // namespace

Node<bookType *> *lookUpBook(OrderedLinkedList<bookType *> &inventory,
                             bool selectionMode) {
  if (inventory.isEmpty()) {
    constexpr int innerWidth = 58;
    menu::clearScreen();
    menu::drawBorderLine(innerWidth);
    menu::printCenteredLine("Serendipity Booksellers", innerWidth);
    menu::printCenteredLine("Look Up Book", innerWidth);
    menu::printEmptyLine(innerWidth);
    menu::printMenuLine("The inventory is currently empty.", innerWidth);
    menu::printEmptyLine(innerWidth);
    menu::printMenuLine("Press ENTER to return...", innerWidth);
    menu::drawBorderLine(innerWidth);
    cin.get();
    return nullptr;
  }

  int highlight = 1;
  while (true) {
    renderLookUpMenuScreen(highlight);
    int choice = getChoiceInRange(1, 2, "\nEnter selection (1-2): ");
    highlight = choice;
    switch (choice) {
    case 1: {
      bookType *result = performSearch(inventory, selectionMode);
      if (selectionMode && result != nullptr) {
        return findNodeForBook(inventory, result);
      }
      break;
    }
    case 2:
      return nullptr;
    }
  }
}

// Return hop count from head to selected node; -1 if no selection or not found.
int lookUpBookHopCount(OrderedLinkedList<bookType *> &inventory) {
  Node<bookType *> *selectedNode = lookUpBook(inventory, true);
  if (!selectedNode)
    return -1;

  Node<bookType *> *node = inventory.headPtr();
  int hops = 0;
  while (node && node != selectedNode) {
    node = node->next;
    ++hops;
  }
  return node ? hops : -1;
}

// Return the node pointer for the selected book; nullptr if none chosen.
Node<bookType *> *lookUpBookNodePtr(OrderedLinkedList<bookType *> &inventory) {
  return lookUpBook(inventory, true);
}

void addBook(OrderedLinkedList<bookType *> &inventory) {
  DraftBook draft;
  int highlight = 1;
  const string defaultFooter = "Enter a number (0-9), then press ENTER.";
  string footer = defaultFooter;

  while (true) {
    if (inventory.size() >= static_cast<int>(kMaxBooks)) {
      DraftBook empty;
      renderAddBookScreen(empty, 0,
                          "Database is full. Press ENTER to return...");
      cout << "\nThe inventory database is full. Remove a book before adding a "
              "new record.\n";
      waitForEnter();
      return;
    }

    renderAddBookScreen(draft, highlight, footer);
    footer = defaultFooter;
    cout << "\nEnter selection (0-9): ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    int choice;
    if (!(ss >> choice) || choice < 0 || choice > 9) {
      footer = "Enter a number between 0 and 9.";
      continue;
    }
    highlight = (choice == 0 ? 10 : choice);

    switch (choice) {
    case 1:
      renderAddBookScreen(draft, highlight,
                          "Editing Title. Enter Book Title below.");
      draft.title = promptNonEmptyString("Enter Book Title: ");
      draft.titleSet = true;
      break;
    case 2: {
      renderAddBookScreen(draft, highlight, "Editing ISBN. Enter ISBN below.");
      if (draft.isbnSet) {
        cout << "\nCurrent ISBN: " << draft.isbn << '\n';
      }
      while (true) {
        string newISBN = promptNonEmptyString("Enter ISBN: ");
        if (hasDuplicateISBN(inventory, newISBN)) {
          cout
              << "A book with that ISBN already exists. Enter a unique ISBN.\n";
          continue;
        }
        draft.isbn = newISBN;
        draft.isbnSet = true;
        break;
      }
      break;
    }
    case 3:
      renderAddBookScreen(draft, highlight,
                          "Editing Author. Enter Author below.");
      draft.author = promptNonEmptyString("Enter Author: ");
      draft.authorSet = true;
      break;
    case 4:
      renderAddBookScreen(draft, highlight,
                          "Editing Publisher. Enter Publisher below.");
      draft.publisher = promptNonEmptyString("Enter Publisher: ");
      draft.publisherSet = true;
      break;
    case 5:
      renderAddBookScreen(
          draft, highlight,
          "Editing Date Added. Enter Date Added (mm/dd/yyyy) below.");
      draft.dateAdded = promptDate("Enter Date Added (mm/dd/yyyy): ");
      draft.dateSet = true;
      break;
    case 6:
      renderAddBookScreen(draft, highlight,
                          "Editing Quantity. Enter Quantity on Hand below.");
      draft.quantity = promptNonNegativeInt("Enter Quantity on Hand: ");
      draft.quantitySet = true;
      break;
    case 7:
      renderAddBookScreen(draft, highlight,
                          "Editing Wholesale Cost. Enter value below.");
      draft.wholesale = promptNonNegativeFloat("Enter Wholesale Cost: ");
      draft.wholesaleSet = true;
      break;
    case 8:
      renderAddBookScreen(draft, highlight,
                          "Editing Retail Price. Enter value below.");
      draft.retail = promptNonNegativeFloat("Enter Retail Price: ");
      draft.retailSet = true;
      break;
    case 9:
      renderAddBookScreen(draft, highlight,
                          "Attempting to save book to database...");
      if (inventory.size() >= static_cast<int>(kMaxBooks)) {
        renderAddBookScreen(draft, highlight,
                            "Unable to save. Database already full.");
        waitForEnter();
        return;
      }
      if (!draft.readyToSave()) {
        footer = "Complete all fields before saving.";
        break;
      }
      if (hasDuplicateISBN(inventory, draft.isbn)) {
        footer = "Duplicate ISBN exists. Update ISBN before saving.";
        break;
      }
      inventory.insert(new bookType(
          draft.isbn, draft.title, draft.author, draft.publisher,
          draft.dateAdded, draft.quantity, draft.wholesale, draft.retail));
      footer = "Book saved to database. Enter another or select 0 to return.";
      draft = DraftBook{};
      highlight = 1;
      break;
    case 0:
      if (!draft.anyFieldSet()) {
        return;
      } else {
        renderAddBookScreen(draft, highlight,
                            "Discard current entry? (y/n) Enter choice below.");
        cout << "\nDiscard current entry? (y/n): ";
        string response;
        getline(cin, response);
        if (!response.empty() && (response[0] == 'y' || response[0] == 'Y')) {
          return;
        }
      }
      break;
    }
  }
}

void editBook(OrderedLinkedList<bookType *> &inventory) {
  if (inventory.isEmpty()) {
    cout << "\nThere are no books to edit.\n";
    waitForEnter();
    return;
  }

  while (true) {
    Node<bookType *> *bookNode = lookUpBook(inventory, true);
    bookType *book = bookNode ? bookNode->data : nullptr;
    if (book == nullptr) {
      cout << "\nNo book selected.\n";
      if (!promptYesNo("Edit another? (y/n): "))
        return;
      continue;
    }

    DraftBook draft = makeDraftFromBook(book);
    const string defaultFooter =
        "Enter a number (0-9) to edit a field, save, or cancel.";
    string footer = defaultFooter;
    int highlight = 1;
    bool editing = true;

    while (editing) {
      renderEditBookScreen(draft, highlight, footer);
      footer = defaultFooter;
      cout << "\nEnter selection (0-9): ";
      string line;
      getline(cin, line);
      stringstream ss(line);
      int choice;
      if (!(ss >> choice) || choice < 0 || choice > 9) {
        footer = "Enter a number between 0 and 9.";
        continue;
      }
      highlight = choice;

      switch (choice) {
      case 1:
        renderEditBookScreen(draft, highlight,
                             "Editing Title. Enter new Title below.");
        cout << "\nCurrent Title: " << draft.title << '\n';
        draft.title = promptNonEmptyString("Enter Book Title: ");
        draft.titleSet = true;
        break;
      case 2: {
        renderEditBookScreen(draft, highlight,
                             "Editing ISBN. Enter new ISBN below.");
        cout << "\nCurrent ISBN: " << draft.isbn << '\n';
        string newISBN = promptNonEmptyString("Enter ISBN: ");
        if (hasDuplicateISBN(inventory, newISBN, book)) {
          footer = "Duplicate ISBN detected. Value unchanged.";
        } else {
          draft.isbn = newISBN;
          draft.isbnSet = true;
        }
        break;
      }
      case 3:
        renderEditBookScreen(draft, highlight,
                             "Editing Author. Enter new Author below.");
        cout << "\nCurrent Author: " << draft.author << '\n';
        draft.author = promptNonEmptyString("Enter Author: ");
        draft.authorSet = true;
        break;
      case 4:
        renderEditBookScreen(draft, highlight,
                             "Editing Publisher. Enter new Publisher below.");
        cout << "\nCurrent Publisher: " << draft.publisher << '\n';
        draft.publisher = promptNonEmptyString("Enter Publisher: ");
        draft.publisherSet = true;
        break;
      case 5:
        renderEditBookScreen(
            draft, highlight,
            "Editing Date Added. Enter Date (mm/dd/yyyy) below.");
        cout << "\nCurrent Date Added: " << draft.dateAdded << '\n';
        draft.dateAdded = promptDate("Enter Date Added (mm/dd/yyyy): ");
        draft.dateSet = true;
        break;
      case 6:
        renderEditBookScreen(
            draft, highlight,
            "Editing Quantity on Hand. Enter non-negative value below.");
        cout << "\nCurrent Quantity on Hand: " << draft.quantity << '\n';
        draft.quantity = promptNonNegativeInt("Enter Quantity on Hand: ");
        draft.quantitySet = true;
        break;
      case 7:
        renderEditBookScreen(
            draft, highlight,
            "Editing Wholesale Cost. Enter non-negative value below.");
        cout << "\nCurrent Wholesale Cost: " << draft.wholesale << '\n';
        draft.wholesale = promptNonNegativeFloat("Enter Wholesale Cost: ");
        draft.wholesaleSet = true;
        break;
      case 8:
        renderEditBookScreen(
            draft, highlight,
            "Editing Retail Price. Enter non-negative value below.");
        cout << "\nCurrent Retail Price: " << draft.retail << '\n';
        draft.retail = promptNonNegativeFloat("Enter Retail Price: ");
        draft.retailSet = true;
        break;
      case 9:
        if (hasDuplicateISBN(inventory, draft.isbn, book)) {
          footer = "Duplicate ISBN detected. Resolve before saving.";
          break;
        }
        applyDraftToBook(book, draft);
        renderEditBookScreen(draft, highlight,
                             "Changes saved. Press ENTER to continue.");
        waitForEnter();
        editing = false;
        break;
      case 0:
        if (promptYesNo("Discard changes without saving? (y/n): ")) {
          editing = false;
        }
        break;
      }
    }

    if (!promptYesNo("Edit another? (y/n): "))
      return;
  }
}

void deleteBook(OrderedLinkedList<bookType *> &inventory) {
  if (inventory.isEmpty()) {
    cout << "\nThere are no books to delete.\n";
    waitForEnter();
    return;
  }

  while (true) {
    if (inventory.isEmpty()) {
      cout << "\nInventory is empty. Returning to menu.\n";
      waitForEnter();
      return;
    }

    Node<bookType *> *bookNode = lookUpBook(inventory, true);
    bookType *book = bookNode ? bookNode->data : nullptr;
    if (book == nullptr) {
      cout << "\nNo book selected.\n";
      if (!promptYesNo("Delete another? (y/n): "))
        return;
      continue;
    }

    cout << "\nSelected Book:";
    printBookSummary(book);
    bool confirm = promptYesNo("Delete this book? (y/n): ");
    if (confirm) {
      inventory.remove(book);
      delete book;
      cout << "\nBook deleted from inventory.\n";
    } else {
      cout << "\nDeletion canceled.\n";
    }

    if (!promptYesNo("Delete another? (y/n): "))
      return;
  }
}

void invMenu(OrderedLinkedList<bookType *> &inventory) {
  int highlight = 1;
  int choice = 0;
  do {
    renderInventoryMenuScreen(highlight);
    choice = getChoiceInRange(1, 5, "\nEnter selection (1-5): ");
    highlight = choice;

    switch (choice) {
    case 1:
      lookUpBook(inventory);
      break;
    case 2:
      addBook(inventory);
      break;
    case 3:
      editBook(inventory);
      break;
    case 4:
      deleteBook(inventory);
      break;
    case 5:
      break;
    }
  } while (choice != 5);
}
