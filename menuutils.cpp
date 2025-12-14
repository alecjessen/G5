#include "menuutils.h"
#include <cctype>
#include <iostream>
#include <string>

namespace menu {

void clearScreen() { std::cout << "\033[H\033[2J"; }

std::string padRight(const std::string &text, int width) {
  if (static_cast<int>(text.size()) >= width) {
    return text.substr(0, width);
  }
  return text + std::string(width - text.size(), ' ');
}

void drawBorderLine(int innerWidth) {
  std::cout << '+' << std::string(innerWidth + 2, '-') << "+\n";
}

void printEmptyLine(int innerWidth) {
  std::cout << "| " << std::string(innerWidth, ' ') << " |\n";
}

void printMenuLine(const std::string &text, int innerWidth, bool highlight) {
  std::string padded = padRight(text, innerWidth);
  std::cout << "| ";
  if (highlight) {
    std::cout << "\033[7m" << padded << "\033[0m";
  } else {
    std::cout << padded;
  }
  std::cout << " |\n";
}

void printCenteredLine(const std::string &text, int innerWidth) {
  std::string trimmed = text;
  if (static_cast<int>(trimmed.size()) > innerWidth) {
    trimmed = trimmed.substr(0, innerWidth);
  }
  int leftPad = (innerWidth - static_cast<int>(trimmed.size())) / 2;
  std::string line(leftPad, ' ');
  line += trimmed;
  line += std::string(innerWidth - leftPad - static_cast<int>(trimmed.size()),
                      ' ');
  printMenuLine(line, innerWidth);
}

void printDualColumnLine(const std::string &leftText, bool highlightLeft,
                         int leftWidth, const std::string &rightText,
                         int rightWidth, int innerWidth) {
  std::string leftTrimmed = leftText;
  if (static_cast<int>(leftTrimmed.size()) > leftWidth) {
    leftTrimmed = leftTrimmed.substr(0, leftWidth);
  }
  std::string leftPadded = padRight(leftTrimmed, leftWidth);

  std::string rightTrimmed = rightText;
  if (static_cast<int>(rightTrimmed.size()) > rightWidth) {
    rightTrimmed = rightTrimmed.substr(0, rightWidth);
  }
  std::string rightPadded = padRight(rightTrimmed, rightWidth);
  std::cout << "| ";
  if (highlightLeft) {
    std::cout << "\033[7m" << leftPadded << "\033[0m";
  } else {
    std::cout << leftPadded;
  }
  std::cout << " | " << rightPadded;
  int used = leftWidth + 3 + rightWidth;
  if (innerWidth > used) {
    std::cout << std::string(innerWidth - used, ' ');
  }
  std::cout << " |\n";
}

std::string currentDate() {
  using namespace std::chrono;
  auto now = system_clock::now();
  std::time_t t = system_clock::to_time_t(now);
  std::tm tm = *std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%m/%d/%Y");
  return oss.str();
}

} // namespace menu
