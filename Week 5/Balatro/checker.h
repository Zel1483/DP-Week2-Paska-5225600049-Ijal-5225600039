#include <iostream>
#include <string>

using namespace std;

// Base Class
class PokerHandChecker {
protected:
    PokerHandChecker* nextChecker;
public:
    PokerHandChecker() : nextChecker(nullptr) {}
    
    void setNext(PokerHandChecker* next) {
        nextChecker = next;
    }
    
    virtual string checkHand() {
        if (nextChecker != nullptr) {
            return nextChecker->checkHand();
        }
        return "Unknown Hand";
    }
    
    virtual ~PokerHandChecker() = default;
};

// Concrete Checkers
class FlushHouseChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isFlushHouse = false; // Implementasi logika asli di sini
        if (isFlushHouse) return "Flush House";
        return PokerHandChecker::checkHand();
    }
};

class FiveOfAKindChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isFiveOfAKind = false; 
        if (isFiveOfAKind) return "Five of a Kind";
        return PokerHandChecker::checkHand();
    }
};

class RoyalFlushChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isRoyalFlush = false;
        if (isRoyalFlush) return "Royal Flush";
        return PokerHandChecker::checkHand();
    }
};

class StraightFlushChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isStraightFlush = false;
        if (isStraightFlush) return "Straight Flush";
        return PokerHandChecker::checkHand();
    }
};

class FourOfAKindChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isFourOfAKind = false;
        if (isFourOfAKind) return "Four of a Kind";
        return PokerHandChecker::checkHand();
    }
};

class FullHouseChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isFullHouse = false;
        if (isFullHouse) return "Full House";
        return PokerHandChecker::checkHand();
    }
};

class FlushChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isFlush = false; 
        if (isFlush) return "Flush";
        return PokerHandChecker::checkHand();
    }
};

class StraightChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isStraight = false;
        if (isStraight) return "Straight";
        return PokerHandChecker::checkHand();
    }
};

class ThreeOfAKindChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isThreeOfAKind = false;
        if (isThreeOfAKind) return "Three of a Kind";
        return PokerHandChecker::checkHand();
    }
};

class TwoPairChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isTwoPair = false;
        if (isTwoPair) return "Two Pair";
        return PokerHandChecker::checkHand();
    }
};

class PairChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isPair = false;
        if (isPair) return "Pair";
        return PokerHandChecker::checkHand();
    }
};

class HighCardChecker : public PokerHandChecker {
public:
    string checkHand() override {
        bool isHighCard = true; // Kondisi default terakhir
        if (isHighCard) return "High Card";
        return PokerHandChecker::checkHand();
    }
};