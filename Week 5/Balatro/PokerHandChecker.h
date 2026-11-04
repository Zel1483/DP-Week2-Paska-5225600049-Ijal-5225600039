#pragma once
#include "Hand.h"
#include "HandRank.h"

// Base class Chain of Responsibility.
// Tiap checker mengecek satu jenis hand; bila tidak cocok, hand diteruskan
// ke checker berikutnya (nextChecker).
class PokerHandChecker {
public:
    virtual ~PokerHandChecker() = default;

    virtual HandRank check(const Hand& hand) const = 0;

    void setNext(PokerHandChecker* next) { nextChecker = next; }

protected:
    PokerHandChecker* nextChecker = nullptr;

    // Teruskan ke checker berikutnya. Rantai habis -> High Card.
    HandRank passToNext(const Hand& hand) const {
        return nextChecker ? nextChecker->check(hand) : HandRank::HighCard;
    }
};
