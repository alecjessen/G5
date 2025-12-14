/*
  CS1B – Serendipity (Final)
  Partner A: Zee Richmond (1244256)
  Partner B: Alexander Jessen (A00186160)
  Date: 2025-12-12
  Purpose: Inventory database menus (lookup/add/edit/delete) using ordered list.
  Build: g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp
         booktype.cpp bookinfo.cpp menuutils.cpp -o serendipity
*/

#include "booktype.h"
#include <algorithm>
#include <cctype>
#include <utility>

std::size_t bookType::num_recs = 0;
int bookType::sortCode = 0;

std::size_t bookType::recordCount() { return num_recs; }

bookType::bookType()
    : isbn(""), bookTitle(""), author(""), publisher(""), dateAdded(""),
      qtyOnHand(0), wholesale(0.0f), retail(0.0f) {
  ++num_recs;
}

bookType::bookType(const std::string &isbnValue, const std::string &titleValue,
                   const std::string &authorValue,
                   const std::string &publisherValue,
                   const std::string &dateAddedValue, int qtyValue,
                   float wholesaleValue, float retailValue)
    : isbn(isbnValue), bookTitle(titleValue), author(authorValue),
      publisher(publisherValue), dateAdded(dateAddedValue), qtyOnHand(qtyValue),
      wholesale(wholesaleValue), retail(retailValue) {
  ++num_recs;
}

bookType::bookType(const bookType &other)
    : isbn(other.isbn), bookTitle(other.bookTitle), author(other.author),
      publisher(other.publisher), dateAdded(other.dateAdded),
      qtyOnHand(other.qtyOnHand), wholesale(other.wholesale),
      retail(other.retail) {
  ++num_recs;
}

bookType::bookType(bookType &&other) noexcept
    : isbn(std::move(other.isbn)), bookTitle(std::move(other.bookTitle)),
      author(std::move(other.author)), publisher(std::move(other.publisher)),
      dateAdded(std::move(other.dateAdded)), qtyOnHand(other.qtyOnHand),
      wholesale(other.wholesale), retail(other.retail) {
  ++num_recs;
}

bookType::~bookType() { --num_recs; }

static std::string toLower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return s;
}

bool bookType::operator<(const bookType &other) const {
  switch (sortCode) {
  case 0:
    return toLower(bookTitle) < toLower(other.bookTitle);
  case 1:
    return toLower(isbn) < toLower(other.isbn);
  case 2:
    return toLower(author) < toLower(other.author);
  case 3:
    return toLower(publisher) < toLower(other.publisher);
  case 4:
    return dateAdded < other.dateAdded; // Simple string comparison for date
  case 5:
    return qtyOnHand < other.qtyOnHand;
  case 6:
    return wholesale < other.wholesale;
  case 7:
    return retail < other.retail;
  default:
    return toLower(bookTitle) < toLower(other.bookTitle);
  }
}

bool bookType::operator>(const bookType &other) const { return other < *this; }

bool bookType::operator<=(const bookType &other) const {
  return !(*this > other);
}

bool bookType::operator>=(const bookType &other) const {
  return !(*this < other);
}

bool bookType::operator==(const bookType &other) const {
  switch (sortCode) {
  case 0:
    return toLower(bookTitle) == toLower(other.bookTitle);
  case 1:
    return toLower(isbn) == toLower(other.isbn);
  case 2:
    return toLower(author) == toLower(other.author);
  case 3:
    return toLower(publisher) == toLower(other.publisher);
  case 4:
    return dateAdded == other.dateAdded;
  case 5:
    return qtyOnHand == other.qtyOnHand;
  case 6:
    return wholesale == other.wholesale;
  case 7:
    return retail == other.retail;
  default:
    return toLower(bookTitle) == toLower(other.bookTitle);
  }
}

bool bookType::operator!=(const bookType &other) const {
  return !(*this == other);
}

void bookType::setISBN(const std::string &value) { isbn = value; }
void bookType::setTitle(const std::string &value) { bookTitle = value; }
void bookType::setAuthor(const std::string &value) { author = value; }
void bookType::setPub(const std::string &value) { publisher = value; }
void bookType::setDateAdded(const std::string &value) { dateAdded = value; }
void bookType::setQtyOnHand(int value) { qtyOnHand = value; }
void bookType::setWholesale(float value) { wholesale = value; }
void bookType::setRetail(float value) { retail = value; }

std::string bookType::getISBN() const { return isbn; }
std::string bookType::getTitle() const { return bookTitle; }
std::string bookType::getAuthor() const { return author; }
std::string bookType::getPub() const { return publisher; }
std::string bookType::getDateAdded() const { return dateAdded; }
int bookType::getQtyOnHand() const { return qtyOnHand; }
float bookType::getWholesale() const { return wholesale; }
float bookType::getRetail() const { return retail; }
