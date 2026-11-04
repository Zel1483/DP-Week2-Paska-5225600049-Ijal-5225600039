#include "HandGenerator.h"

#include <algorithm>
#include <chrono>
#include <vector>

namespace {

unsigned makeSeed() {
    // random_device di beberapa MinGW lama bersifat deterministik,
    // jadi dicampur dengan waktu agar tiap run tetap berbeda.
    std::random_device rd;
    auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    return rd() ^ static_cast<unsigned>(now);
}

std::vector<Card> buildDeck() {
    static const char suits[] = {'H', 'D', 'C', 'S'};
    std::vector<Card> deck;
    deck.reserve(52);
    for (char s : suits)
        for (int r = 2; r <= 14; ++r)
            deck.push_back({r, s});
    return deck;
}

}  // namespace

HandGenerator::HandGenerator() : rng(makeSeed()) {}
HandGenerator::HandGenerator(unsigned seed) : rng(seed) {}

Hand HandGenerator::generateHand(std::size_t size) {
    std::vector<Card> deck = buildDeck();
    std::shuffle(deck.begin(), deck.end(), rng);

    size = std::min(size, deck.size());
    Hand hand;
    hand.cards.assign(deck.begin(), deck.begin() + size);
    return hand;
}
