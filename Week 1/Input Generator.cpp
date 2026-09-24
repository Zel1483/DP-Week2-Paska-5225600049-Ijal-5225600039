#include "input_generator.h"
#include <cstdlib>
#include <iostream>

int generateInput() {
    int input = rand() % 10 + 1; // angka acak 1 - 10
    std::cout << "[PLAY] input generated: " << input << std::endl;
    return input;
}