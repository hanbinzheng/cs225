EXECUTABLE = mp1

# compile flags
CXXFLAGS = -std=c++11 -Wall -Werror -pedantic -O0 -c -g
LXXFLAGS = -std=c++11
CXX = g++
LXX = g++

# makefile rules
all: $(EXECUTABLE)

unit_tests.o : tests/unit_tests.cpp tests/catch.hpp cs225/PNG.h cs225/HSLAPixel.h
	$(CXX) $(CXXFLAGS) tests/unit_tests.cpp

test : unit_tests.o mp1.o PNG.o HSLAPixel.o lodepng.o
	$(LXX) unit_tests.o mp1.o PNG.o HSLAPixel.o lodepng.o $(LDFLAGS) -o test

HSLAPixel.o: cs225/HSLAPixel.cpp cs225/HSLAPixel.h
	$(CXX) $(CXXFLAGS) cs225/HSLAPixel.cpp

PNG.o: HSLAPixel.o lodepng.o cs225/PNG.h cs225/PNG.cpp cs225/RGB_HSL.h
	$(CXX) $(CXXFLAGS) cs225/PNG.cpp

lodepng.o: cs225/lodepng/lodepng.h cs225/lodepng/lodepng.cpp
	$(CXX) $(CXXFLAGS) cs225/lodepng/lodepng.cpp

mp1.o: PNG.o mp1.h mp1.cpp
	$(CXX) $(CXXFLAGS) mp1.cpp

main.o: main.cpp mp1.o
	$(CXX) $(CXXFLAGS) main.cpp

$(EXECUTABLE): main.o mp1.o lodepng.o PNG.o HSLAPixel.o
	$(LXX) $(LXXFLAGS) main.o mp1.o lodepng.o PNG.o HSLAPixel.o -o $(EXECUTABLE)

clean:
	rm -f main.o mp1.o lodepng.o PNG.o HSLAPixel.o mp1 test unit_tests.o tests/out.png in.png out.png
