#pragma once
#include <string>

// rank: 2..10, 11 = J, 12 = Q, 13 = K, 14 = A
// suit: 'H' (Hearts), 'D' (Diamonds), 'C' (Clubs), 'S' (Spades)
struct Card {
    int  rank;
    char suit;
};

inline std::string toString(const Card& c) {
    std::string r;
    switch (c.rank) {
        case 11: r = "J"; break;
        case 12: r = "Q"; break;
        case 13: r = "K"; break;
        case 14: r = "A"; break;
        default: r = std::to_string(c.rank);
    }
    return r + c.suit;
}
