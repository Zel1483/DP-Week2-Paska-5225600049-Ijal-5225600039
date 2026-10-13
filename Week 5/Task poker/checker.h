#ifndef CHECKER_H
#define CHECKER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Struktur Kartu
struct Card {
    string rank;
    string suit;
};

// Helper untuk mengubah rank menjadi angka berurutan agar mudah mengecek Straight
inline int getRankValue(const string& rank) {
    if (rank == "T") return 10;
    if (rank == "J") return 11;
    if (rank == "Q") return 12;
    if (rank == "K") return 13;
    if (rank == "A") return 14;
    return stoi(rank);
}

// Struct untuk menyimpan data analisis kartu (dihitung sekali untuk semua checker)
struct HandAnalysis {
    map<string, int> rankCounts;
    map<string, int> suitCounts;
    bool isFlush = false;
    bool isStraight = false;
    bool isRoyal = false;
    
    HandAnalysis(const vector<Card>& cards) {
        vector<int> rankValues;
        for (const auto& c : cards) {
            rankCounts[c.rank]++;
            suitCounts[c.suit]++;
            rankValues.push_back(getRankValue(c.rank));
        }

        // Cek Flush (ada 5+ kartu dengan suit yang sama)
        for (const auto& suit : suitCounts) {
            if (suit.second >= 5) isFlush = true;
        }

        // Cek Straight (ada 5 angka berurutan)
        sort(rankValues.begin(), rankValues.end());
        // Hapus duplikat angka agar perhitungan urutan tidak terputus oleh pair
        rankValues.erase(unique(rankValues.begin(), rankValues.end()), rankValues.end()); 
        
        int consecutive = 1;
        for (size_t i = 1; i < rankValues.size(); ++i) {
            if (rankValues[i] == rankValues[i - 1] + 1) {
                consecutive++;
                if (consecutive >= 5) {
                    isStraight = true;
                    if (rankValues[i] == 14) isRoyal = true; 
                }
            } else {
                if (consecutive < 5) consecutive = 1;
            }
        }
        // Edge case: Straight A-2-3-4-5
        if (rankCounts["A"] > 0 && rankCounts["2"] > 0 && rankCounts["3"] > 0 && rankCounts["4"] > 0 && rankCounts["5"] > 0) {
            isStraight = true;
        }
    }

    // Fungsi bantu mencari jumlah set tertentu (misal butuh 1 pair, 2 pair, dll)
    int countRanksWithFrequency(int freq) const {
        int count = 0;
        for (const auto& pair : rankCounts) {
            if (pair.second >= freq) count++;
        }
        return count;
    }
};

// --- Base Class Chain of Responsibility ---
class PokerHandChecker {
protected:
    PokerHandChecker* nextChecker;
public:
    PokerHandChecker() : nextChecker(nullptr) {}
    
    void setNext(PokerHandChecker* next) {
        nextChecker = next;
    }
    
    virtual string checkHand(const HandAnalysis& analysis) {
        if (nextChecker != nullptr) {
            return nextChecker->checkHand(analysis);
        }
        return "Unknown Hand";
    }
    
    virtual ~PokerHandChecker() = default;
};

// --- Concrete Checkers ---
class FlushHouseChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        bool hasFullHouse = (analysis.countRanksWithFrequency(3) >= 1 && analysis.countRanksWithFrequency(2) >= 2);
        if (analysis.isFlush && hasFullHouse) return "Flush House";
        return PokerHandChecker::checkHand(analysis);
    }
};

class FiveOfAKindChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.countRanksWithFrequency(5) >= 1) return "Five of a Kind";
        return PokerHandChecker::checkHand(analysis);
    }
};

class RoyalFlushChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.isFlush && analysis.isStraight && analysis.isRoyal) return "Royal Flush";
        return PokerHandChecker::checkHand(analysis);
    }
};

class StraightFlushChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.isFlush && analysis.isStraight) return "Straight Flush";
        return PokerHandChecker::checkHand(analysis);
    }
};

class FourOfAKindChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.countRanksWithFrequency(4) >= 1) return "Four of a Kind";
        return PokerHandChecker::checkHand(analysis);
    }
};

class FullHouseChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.countRanksWithFrequency(3) >= 1 && analysis.countRanksWithFrequency(2) >= 2) return "Full House";
        return PokerHandChecker::checkHand(analysis);
    }
};

class FlushChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.isFlush) return "Flush";
        return PokerHandChecker::checkHand(analysis);
    }
};

class StraightChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.isStraight) return "Straight";
        return PokerHandChecker::checkHand(analysis);
    }
};

class ThreeOfAKindChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.countRanksWithFrequency(3) >= 1) return "Three of a Kind";
        return PokerHandChecker::checkHand(analysis);
    }
};

class TwoPairChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.countRanksWithFrequency(2) >= 2) return "Two Pair";
        return PokerHandChecker::checkHand(analysis);
    }
};

class PairChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        if (analysis.countRanksWithFrequency(2) >= 1) return "Pair";
        return PokerHandChecker::checkHand(analysis);
    }
};

class HighCardChecker : public PokerHandChecker {
public:
    string checkHand(const HandAnalysis& analysis) override {
        return "High Card"; 
    }
};

#endif