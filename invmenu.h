#ifndef INVMENU_H
#define INVMENU_H

#include "booktype.h"
#include "orderedLinkedList.h"

void invMenu(OrderedLinkedList<bookType*>& inventory);

bookType* lookUpBook(OrderedLinkedList<bookType*>& inventory, bool selectionMode = false);
int lookUpBookHopCount(OrderedLinkedList<bookType*>& inventory);
Node<bookType*>* lookUpBookNodePtr(OrderedLinkedList<bookType*>& inventory);

void addBook(OrderedLinkedList<bookType*>& inventory);
void editBook(OrderedLinkedList<bookType*>& inventory);
void deleteBook(OrderedLinkedList<bookType*>& inventory);

#endif
