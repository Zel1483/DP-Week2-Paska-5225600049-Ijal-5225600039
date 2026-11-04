CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

LIB_SRCS = HandGenerator.cpp ChosenHand.cpp HandPlayer.cpp GameManager.cpp

game: main.cpp $(LIB_SRCS)
	$(CXX) $(CXXFLAGS) main.cpp $(LIB_SRCS) -o game

test: tests.cpp HandGenerator.cpp ChosenHand.cpp
	$(CXX) $(CXXFLAGS) tests.cpp HandGenerator.cpp ChosenHand.cpp -o tests && ./tests

clean:
	rm -f game tests

.PHONY: test clean
