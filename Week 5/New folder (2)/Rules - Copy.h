#pragma once
#include "Hand.h"

// Base class Chain of Responsibility.
// Tiap checker mengecek satu jenis hand; kalau tidak cocok, hand
// diteruskan ke checker berikutnya (nextChecker).
class PokerHandChecker {
public:
    virtual ~PokerHandChecker() = default;

    virtual HandRank check(const Hand& hand) = 0;

    void setNext(PokerHandChecker* next) { nextChecker = next; }

protected:
    PokerHandChecker* nextChecker = nullptr;

    // Helper: teruskan ke checker berikutnya. Kalau rantai habis -> High Card.
    HandRank passToNext(const Hand& hand) {
        return nextChecker ? nextChecker->check(hand) : HandRank::HighCard;
    }
};