CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic
SOURCES := mainmenu.cpp cashier.cpp invmenu.cpp reports.cpp booktype.cpp bookinfo.cpp menuutils.cpp
TARGET := serendipity

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

.PHONY: clean

clean:
	rm -f $(TARGET)
