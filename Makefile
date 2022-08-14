# Define the symbols we might want to change:
CXX	:= c++
CXXFLAGS := -O0 -ggdb
sources := $(wildcard *.cpp)
OBJECTS := $(sources:cpp=o)
HEADERS := $(wildcard *.h)
prog_name := spell-test-cpp

.PHONY: all clean

all: $(prog_name)

$(OBJECTS) : %.o: %.cpp $(HEADERS) Makefile
	$(CXX) -c $< $(CXXFLAGS)

$(prog_name): $(OBJECTS) Makefile
	$(CXX) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(prog_name)

