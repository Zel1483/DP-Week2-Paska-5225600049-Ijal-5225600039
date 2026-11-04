#include "HandPlayer.h"
#include <iostream>

void HandPlayer::playHand(const Hand& hand) const {
    std::cout << "Playing:";
    for (const Card& c : hand.cards) std::cout << " [" << toString(c) << "]";
    std::cout << '\n';
}
