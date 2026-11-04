#pragma once

enum class HandRank {
    HighCard, Pair, TwoPair, ThreeOfAKind, Straight, Flush, FullHouse,
    FourOfAKind, StraightFlush, RoyalFlush, FiveOfAKind, FlushHouse
};

inline const char* toString(HandRank rank) {
    switch (rank) {
        case HandRank::HighCard:      return "High Card";
        case HandRank::Pair:          return "Pair";
        case HandRank::TwoPair:       return "Two Pair";
        case HandRank::ThreeOfAKind:  return "Three of a Kind";
        case HandRank::Straight:      return "Straight";
        case HandRank::Flush:         return "Flush";
        case HandRank::FullHouse:     return "Full House";
        case HandRank::FourOfAKind:   return "Four of a Kind";
        case HandRank::StraightFlush: return "Straight Flush";
        case HandRank::RoyalFlush:    return "Royal Flush";
        case HandRank::FiveOfAKind:   return "Five of a Kind";
        case HandRank::FlushHouse:    return "Flush House";
    }
    return "Unknown";
}
