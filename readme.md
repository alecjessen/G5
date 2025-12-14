# CS1B – Serendipity (Final)

## Partners

- Partner A: Zee Richmond (1244256) — Partner A
- Partner B: Alexander Jessen (A00186160) — Partner B

## Date

2025-12-12

## Project Description

Serendipity now uses an **ordered linked list of `bookType*`** for inventory storage, driven by `bookType::sortCode` and overloaded comparison operators. It loads data from `prog.out`, supports full CRUD in the inventory module, multi-book receipts in the cashier module, and paginated reports with arrow-key navigation.

## Key Features
- **Ordered linked list inventory** (`orderedLinkedList.h`): stores `bookType*`, maintains sort order via `bookType::sortCode` (title/ISBN/etc.), and cleans up memory on exit.
- **Static record counter** (`booktype.cpp/h`): `bookType::num_recs` increments in all constructors and decrements in the destructor; exposed via `recordCount()`.
- **Data loading** (`mainmenu.cpp`): parses `prog.out` marker/value pairs (1–9) and inserts books while preserving ordering.
- **Inventory module** (`invmenu.cpp`): add/edit/delete/search with duplicate-ISBN checks and in-place edits; lookup helpers can return hop count or node pointer for demos.
- **Cashier module** (`cashier.cpp`): multi-book cart using dynamic int arrays, prevents overselling by tracking quantities already in cart, auto-uses current date, and confirms purchases before updating stock.
- **Reports module** (`reports.cpp`): bordered menu plus three paginated reports (listing, wholesale, retail) with arrow/rubber-band navigation, current-date headers, and totals on wholesale/retail.
- **UI helpers** (`menuutils.cpp/h`): shared screen rendering and `currentDate()` helper for consistent dating across modules.

## Demo Pointers
- Show linked list structure/sorting: `orderedLinkedList.h`, `booktype.cpp` operators and `sortCode`.
- Show `prog.out` parsing: `mainmenu.cpp::loadData`.
- Show cashier safeguards: `cashier.cpp` (cart arrays, `qtyAlreadyInCart`, confirmation, thank-you).
- Show report pagination: `reports.cpp` (per-report loops and headers).

## Build & Run Notes

Compile on Linux with:

```bash
make
```

Or manually:

```bash
g++ -std=c++20 mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp booktype.cpp bookinfo.cpp menuutils.cpp -o serendipity
```
