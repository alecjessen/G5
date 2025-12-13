#ifndef BOOKTYPE_H
#define BOOKTYPE_H

#include <string>

class bookType {
public:
  bookType();
  bookType(const std::string &isbn, const std::string &title,
           const std::string &author, const std::string &publisher,
           const std::string &dateAdded, int qty, float wholesale,
           float retail);
  bookType(const bookType &other);
  bookType(bookType &&other) noexcept;
  ~bookType();

  bookType &operator=(const bookType &other) = default;
  bookType &operator=(bookType &&other) noexcept = default;

  void setISBN(const std::string &value);
  void setTitle(const std::string &value);
  void setAuthor(const std::string &value);
  void setPub(const std::string &value);
  void setDateAdded(const std::string &value);
  void setQtyOnHand(int value);
  void setWholesale(float value);
  void setRetail(float value);

  std::string getISBN() const;
  std::string getTitle() const;
  std::string getAuthor() const;
  std::string getPub() const;
  std::string getDateAdded() const;
  int getQtyOnHand() const;
  float getWholesale() const;
  float getRetail() const;

  static int getBookCount();

private:
  std::string isbn;
  std::string bookTitle;
  std::string author;
  std::string publisher;
  std::string dateAdded;
  int qtyOnHand;
  float wholesale;
  float retail;

  static int bookCount;
};

#endif
