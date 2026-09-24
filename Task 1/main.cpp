#include "advance_round.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int money = 0;
    const int TOTAL_ROUNDS = 3;

    std::cout << "=== RUN START ===" << std::endl;

    for (int round = 1; round <= TOTAL_ROUNDS; round++) {
        advanceRound(round, money);
    }

    std::cout << "\n=== RUN END ===" << std::endl;
    std::cout << "Final money: " << money << std::endl;

    return 0;
}