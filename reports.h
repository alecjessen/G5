#ifndef REPORTS_H
#define REPORTS_H

#include "orderedLinkedList.h"
#include "booktype.h"

void reports(OrderedLinkedList<bookType*>& inventory);

/**
 * @brief Generates a paginated report of all books in the inventory.
 * @param inventory A list of pointers to bookType objects.
 */
void repListing(OrderedLinkedList<bookType*>& inventory);
void repWholesale(OrderedLinkedList<bookType*>& inventory);
void repRetail(OrderedLinkedList<bookType*>& inventory);
void repQty(OrderedLinkedList<bookType*>& inventory);
void repCost(OrderedLinkedList<bookType*>& inventory);
void repAge(OrderedLinkedList<bookType*>& inventory);

#endif
