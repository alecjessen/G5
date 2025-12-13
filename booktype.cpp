#include "booktype.h"
#include <utility>

int bookType::bookCount = 0;

bookType::bookType()
    : isbn(""), bookTitle(""), author(""), publisher(""), dateAdded(""),
      qtyOnHand(0), wholesale(0.0f), retail(0.0f) {
  ++bookCount;
}

bookType::bookType(const std::string &isbnValue, const std::string &titleValue,
                   const std::string &authorValue,
                   const std::string &publisherValue,
                   const std::string &dateAddedValue, int qtyValue,
                   float wholesaleValue, float retailValue)
    : isbn(isbnValue), bookTitle(titleValue), author(authorValue),
      publisher(publisherValue), dateAdded(dateAddedValue), qtyOnHand(qtyValue),
      wholesale(wholesaleValue), retail(retailValue) {
  ++bookCount;
}

bookType::bookType(const bookType &other)
    : isbn(other.isbn), bookTitle(other.bookTitle), author(other.author),
      publisher(other.publisher), dateAdded(other.dateAdded),
      qtyOnHand(other.qtyOnHand), wholesale(other.wholesale),
      retail(other.retail) {
  ++bookCount;
}

bookType::bookType(bookType &&other) noexcept
    : isbn(std::move(other.isbn)), bookTitle(std::move(other.bookTitle)),
      author(std::move(other.author)), publisher(std::move(other.publisher)),
      dateAdded(std::move(other.dateAdded)), qtyOnHand(other.qtyOnHand),
      wholesale(other.wholesale), retail(other.retail) {
  ++bookCount;
}

bookType::~bookType() { --bookCount; }

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

int bookType::getBookCount() { return bookCount; }
