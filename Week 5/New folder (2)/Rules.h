#pragma once
#include <cstddef>
#include <memory>
#include <vector>

#include "Hand.h"
#include "PokerHandChecker.h"
#include "PokerHandCheckers.h"

// ---------------------------------------------------------------------------
// ScoringRule: membangun rantai checker & mengubah HandRank menjadi skor
// ---------------------------------------------------------------------------
class ScoringRule {
public:
    ScoringRule() {
        // URUTAN = PRIORITAS: dari hand terkuat ke terlemah
        addChecker<FlushHouseChecker>();
        addChecker<FiveOfAKindChecker>();
        addChecker<RoyalFlushChecker>();
        addChecker<StraightFlushChecker>();
        addChecker<FourOfAKindChecker>();
        addChecker<FullHouseChecker>();
        addChecker<FlushChecker>();
        addChecker<StraightChecker>();
        addChecker<ThreeOfAKindChecker>();
        addChecker<TwoPairChecker>();
        addChecker<PairChecker>();
        addChecker<HighCardChecker>();

        for (std::size_t i = 0; i + 1 < chain.size(); ++i)
            chain[i]->setNext(chain[i + 1].get());
    }

    // Jalankan rantai checker -> jenis hand
    HandRank detectRank(const Hand& hand) { return chain.front()->check(hand); }

    int scoreHand(const Hand& hand) { return convertRankToScore(detectRank(hand)); }

    // Skor dasar = chips x mult (nilai level 1 di Balatro)
    int convertRankToScore(HandRank rank) const {
        switch (rank) {
            case HandRank::HighCard:      return 5   * 1;
            case HandRank::Pair:          return 10  * 2;
            case HandRank::TwoPair:       return 20  * 2;
            case HandRank::ThreeOfAKind:  return 30  * 3;
            case HandRank::Straight:      return 30  * 4;
            case HandRank::Flush:         return 35  * 4;
            case HandRank::FullHouse:     return 40  * 4;
            case HandRank::FourOfAKind:   return 60  * 7;
            case HandRank::StraightFlush: return 100 * 8;
            case HandRank::RoyalFlush:    return 100 * 8;
            case HandRank::FiveOfAKind:   return 120 * 12;
            case HandRank::FlushHouse:    return 140 * 14;
        }
        return 0;
    }

private:
    std::vector<std::unique_ptr<PokerHandChecker>> chain;

    template <typename T>
    void addChecker() { chain.push_back(std::make_unique<T>()); }
};

// ---------------------------------------------------------------------------
// BlindRule: apakah total skor mencapai target blind?
// ---------------------------------------------------------------------------
class BlindRule {
public:
    explicit BlindRule(int targetScore = 300) : target(targetScore) {}

    bool checkBlind(int score) const { return score >= target; }

private:
    int target;
};

// ---------------------------------------------------------------------------
// RewardRule: uang yang didapat setelah blind selesai
// ---------------------------------------------------------------------------
class RewardRule {
public:
    // Kalah = $0. Menang = $3 dasar + $1 tiap 100 poin skor (placeholder).
    int earnMoney(bool win, int score) const {
        if (!win) return 0;
        return kBaseReward + score / kScorePerBonus;
    }

private:
    static constexpr int kBaseReward   = 3;
    static constexpr int kScorePerBonus = 100;
};