#ifndef BOOKTYPE_H
#define BOOKTYPE_H

#include <string>
#include <cstddef>

/**
 * @class bookType
 * @brief Represents a book in the inventory.
 *
 * This class stores details about a book and maintains a static count
 * of the total number of bookType objects currently instantiated.
 * It also supports sorting based on a static sortCode.
 */
class bookType {
public:
  /**
   * @brief Static counter for the number of active bookType objects.
   *
   * Incremented in every constructor and decremented in the destructor.
   */
  static std::size_t num_recs;

  /**
   * @brief Static code to determine sorting criteria.
   * 0: Sort by Title
   * 1: Sort by ISBN
   * 2: Sort by Author
   * 3: Sort by Publisher
   * 4: Sort by Date Added
   * 5: Sort by Qty On Hand
   * 6: Sort by Wholesale Cost
   * 7: Sort by Retail Price
   */
  static int sortCode;

  /**
   * @brief Returns the current number of bookType records.
   * @return The number of live bookType objects.
   */
  static std::size_t recordCount();

  /**
   * @brief Default Constructor.
   * @post num_recs is incremented.
   */
  bookType();

  /**
   * @brief Parameterized Constructor.
   * @param isbn The ISBN of the book.
   * @param title The title of the book.
   * @param author The author of the book.
   * @param publisher The publisher of the book.
   * @param dateAdded The date the book was added.
   * @param qty The quantity on hand.
   * @param wholesale The wholesale cost.
   * @param retail The retail price.
   * @post num_recs is incremented.
   */
  bookType(const std::string &isbn, const std::string &title,
           const std::string &author, const std::string &publisher,
           const std::string &dateAdded, int qty, float wholesale,
           float retail);

  /**
   * @brief Copy Constructor.
   * @param other The bookType object to copy.
   * @post num_recs is incremented.
   */
  bookType(const bookType &other);

  /**
   * @brief Move Constructor.
   * @param other The bookType object to move from.
   * @post num_recs is incremented.
   */
  bookType(bookType &&other) noexcept;

  /**
   * @brief Destructor.
   * @post num_recs is decremented.
   */
  ~bookType();

  bookType &operator=(const bookType &other) = default;
  bookType &operator=(bookType &&other) noexcept = default;

  // Relational Operators
  bool operator<(const bookType &other) const;
  bool operator>(const bookType &other) const;
  bool operator<=(const bookType &other) const;
  bool operator>=(const bookType &other) const;
  bool operator==(const bookType &other) const;
  bool operator!=(const bookType &other) const;

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

private:
  std::string isbn;
  std::string bookTitle;
  std::string author;
  std::string publisher;
  std::string dateAdded;
  int qtyOnHand;
  float wholesale;
  float retail;
};

#endif
