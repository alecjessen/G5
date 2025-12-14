#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace menu {

void clearScreen();
std::string padRight(const std::string &text, int width);
void drawBorderLine(int innerWidth);
void printEmptyLine(int innerWidth);
void printMenuLine(const std::string &text, int innerWidth, bool highlight = false);
void printCenteredLine(const std::string &text, int innerWidth);
void printDualColumnLine(const std::string &leftText, bool highlightLeft,
                         int leftWidth, const std::string &rightText,
                         int rightWidth, int innerWidth);

// Returns current date formatted as mm/dd/YYYY.
std::string currentDate();

} // namespace menu

#endif
