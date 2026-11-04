#pragma once
#include <cstddef>
#include <random>
#include "Hand.h"

class HandGenerator {
public:
    static constexpr std::size_t kDefaultHandSize = 8;   // seperti Balatro

    HandGenerator();                          // seed acak (tiap run berbeda)
    explicit HandGenerator(unsigned seed);    // seed tetap (untuk testing)

    // Kocok 1 deck 52 kartu, ambil `size` kartu teratas -> acak & tanpa kartu kembar.
    Hand generateHand(std::size_t size = kDefaultHandSize);

private:
    std::mt19937 rng;
};
