#pragma once
#include <algorithm>
#include <array>
#include <cstddef>

#include "Hand.h"
#include "PokerHandChecker.h"

// ---------------------------------------------------------------------------
// Helper internal untuk semua checker
// ---------------------------------------------------------------------------
namespace detail {

constexpr std::size_t kHandSize = 5;
constexpr int kMinRank = 2;
constexpr int kMaxRank = 14;   // Ace

using RankCounts = std::array<int, kMaxRank + 1>;   // index = rank

inline RankCounts countRanks(const Hand& hand) {
    RankCounts counts{};
    for (const Card& c : hand.cards)
        if (c.rank >= kMinRank && c.rank <= kMaxRank) ++counts[c.rank];
    return counts;
}

// Jumlah kartu terbanyak dengan rank yang sama
inline int maxOfAKind(const RankCounts& counts) {
    return *std::max_element(counts.begin(), counts.end());
}

// Berapa banyak rank yang muncul >= n kali
inline int ranksWithAtLeast(const RankCounts& counts, int n) {
    return static_cast<int>(
        std::count_if(counts.begin(), counts.end(), [n](int c) { return c >= n; }));
}

inline bool isFlush(const Hand& hand) {
    if (hand.cards.size() != kHandSize) return false;
    return std::all_of(hand.cards.begin(), hand.cards.end(),
                       [&](const Card& c) { return c.suit == hand.cards[0].suit; });
}

// 5 rank berbeda & berurutan. Ace boleh rendah (A-2-3-4-5) atau tinggi (10-J-Q-K-A).
inline bool isStraight(const Hand& hand) {
    if (hand.cards.size() != kHandSize) return false;
    const RankCounts counts = countRanks(hand);

    for (int r = kMinRank; r <= kMaxRank; ++r)
        if (counts[r] > 1) return false;                       // ada rank kembar

    if (counts[14] && counts[2] && counts[3] && counts[4] && counts[5])
        return true;                                           // A-2-3-4-5

    for (int low = kMinRank; low + 4 <= kMaxRank; ++low) {
        bool run = true;
        for (int r = low; r < low + 5; ++r)
            if (!counts[r]) { run = false; break; }
        if (run) return true;
    }
    return false;
}

// 3 kartu satu rank + 2 kartu rank lain
inline bool isFullHouse(const Hand& hand) {
    if (hand.cards.size() != kHandSize) return false;
    const RankCounts counts = countRanks(hand);
    bool three = false, two = false;
    for (int c : counts) {
        if (c == 3) three = true;
        else if (c == 2) two = true;
    }
    return three && two;
}

// 10-J-Q-K-A dengan suit sama
inline bool isRoyalFlush(const Hand& hand) {
    if (!isStraight(hand) || !isFlush(hand)) return false;
    const RankCounts counts = countRanks(hand);
    return counts[10] && counts[11] && counts[12] && counts[13] && counts[14];
}

}  // namespace detail

// ---------------------------------------------------------------------------
// Concrete checkers
// Setiap checker: cocok -> return rank-nya, tidak cocok -> passToNext()
// ---------------------------------------------------------------------------

// Hand ini ujung rantai: selalu cocok
class HighCardChecker : public PokerHandChecker {
public:
    HandRank check(const Hand&) override { return HandRank::HighCard; }
};

class PairChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::maxOfAKind(detail::countRanks(hand)) >= 2) return HandRank::Pair;
        return passToNext(hand);
    }
};

class TwoPairChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::ranksWithAtLeast(detail::countRanks(hand), 2) >= 2)
            return HandRank::TwoPair;
        return passToNext(hand);
    }
};

class ThreeOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::maxOfAKind(detail::countRanks(hand)) >= 3)
            return HandRank::ThreeOfAKind;
        return passToNext(hand);
    }
};

class StraightChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::isStraight(hand)) return HandRank::Straight;
        return passToNext(hand);
    }
};

class FlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::isFlush(hand)) return HandRank::Flush;
        return passToNext(hand);
    }
};

class FullHouseChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::isFullHouse(hand)) return HandRank::FullHouse;
        return passToNext(hand);
    }
};

class FourOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::maxOfAKind(detail::countRanks(hand)) >= 4)
            return HandRank::FourOfAKind;
        return passToNext(hand);
    }
};

class StraightFlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::isStraight(hand) && detail::isFlush(hand))
            return HandRank::StraightFlush;
        return passToNext(hand);
    }
};

class RoyalFlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::isRoyalFlush(hand)) return HandRank::RoyalFlush;
        return passToNext(hand);
    }
};

// 5 kartu dengan rank sama (butuh kartu duplikat, seperti di Balatro)
class FiveOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (hand.cards.size() >= detail::kHandSize &&
            detail::maxOfAKind(detail::countRanks(hand)) >= 5)
            return HandRank::FiveOfAKind;
        return passToNext(hand);
    }
};

// Full House dengan semua kartu satu suit
class FlushHouseChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        if (detail::isFullHouse(hand) && detail::isFlush(hand))
            return HandRank::FlushHouse;
        return passToNext(hand);
    }
};