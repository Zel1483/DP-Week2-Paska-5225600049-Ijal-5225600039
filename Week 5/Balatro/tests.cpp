// Tes sederhana tanpa framework: build dengan `make test`
#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include "ChosenHand.h"
#include "HandGenerator.h"
#include "Rules.h"

static int failures = 0, total = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        ++total;                                                                 \
        if (!(cond)) { ++failures; std::cout << "GAGAL (baris " << __LINE__     \
                                             << "): " #cond "\n"; }              \
    } while (0)

// "10H JD QC KS AH" -> Hand
static Hand H(const std::string& text) {
    Hand hand;
    std::istringstream in(text);
    std::string tok;
    while (in >> tok) {
        std::string r = tok.substr(0, tok.size() - 1);
        int rank = r == "J" ? 11 : r == "Q" ? 12 : r == "K" ? 13 : r == "A" ? 14 : std::stoi(r);
        hand.cards.push_back({rank, tok.back()});
    }
    return hand;
}

int main() {
    ScoringRule rules;

    // ---------- Tugas 2: checker ----------
    CHECK(rules.detectRank(H("2H 5D 9C JS KH"))   == HandRank::HighCard);
    CHECK(rules.detectRank(H("2H 2D 9C JS KH"))   == HandRank::Pair);
    CHECK(rules.detectRank(H("2H 2D 9C 9S KH"))   == HandRank::TwoPair);
    CHECK(rules.detectRank(H("2H 2D 2C JS KH"))   == HandRank::ThreeOfAKind);
    CHECK(rules.detectRank(H("5H 6D 7C 8S 9H"))   == HandRank::Straight);
    CHECK(rules.detectRank(H("AH 2D 3C 4S 5H"))   == HandRank::Straight);      // A rendah
    CHECK(rules.detectRank(H("10H JD QC KS AH"))  == HandRank::Straight);      // A tinggi
    CHECK(rules.detectRank(H("QH KD AC 2S 3H"))   == HandRank::HighCard);      // tidak boleh memutar
    CHECK(rules.detectRank(H("2H 5H 9H JH KH"))   == HandRank::Flush);
    CHECK(rules.detectRank(H("2H 2D 2C 9S 9H"))   == HandRank::FullHouse);
    CHECK(rules.detectRank(H("2H 2D 2C 2S KH"))   == HandRank::FourOfAKind);
    CHECK(rules.detectRank(H("5H 6H 7H 8H 9H"))   == HandRank::StraightFlush);
    CHECK(rules.detectRank(H("AH 2H 3H 4H 5H"))   == HandRank::StraightFlush);
    CHECK(rules.detectRank(H("10H JH QH KH AH"))  == HandRank::RoyalFlush);
    CHECK(rules.detectRank(H("7H 7D 7C 7S 7H"))   == HandRank::FiveOfAKind);   // butuh duplikat
    CHECK(rules.detectRank(H("7H 7H 7H 9H 9H"))   == HandRank::FlushHouse);    // butuh duplikat
    CHECK(rules.detectRank(H("2H 3H 4H 5H"))      == HandRank::HighCard);      // 4 kartu bukan flush
    CHECK(rules.detectRank(H("5H 6D 7C 8S"))      == HandRank::HighCard);      // 4 kartu bukan straight
    CHECK(rules.detectRank(H("KH KD"))            == HandRank::Pair);          // main < 5 kartu
    CHECK(rules.scoreHand(H("10H JH QH KH AH")) == 800);
    CHECK(rules.scoreHand(H("2H 5D 9C JS KH"))  == 5);

    // ---------- Tugas 1: ChosenHand ----------
    Hand dealt = H("2H 3D 4C 5S 6H 7D 8C 9S");
    ChosenHand ch(dealt);
    CHECK(!ch.isValid() && ch.size() == 0);                 // belum memilih apa pun
    CHECK(ch.select(3) && ch.select(0) && ch.select(5));
    CHECK(!ch.select(3));                                   // sudah dipilih
    CHECK(!ch.select(99));                                  // di luar jangkauan
    CHECK(ch.select(1) && ch.select(2));
    CHECK(ch.size() == 5 && ch.isValid());
    CHECK(!ch.select(7));                                   // maksimal 5
    CHECK(ch.toHand().cards.size() == 5);
    CHECK(ch.toHand().cards[0].rank == 2 && ch.toHand().cards[4].rank == 7);  // urutan sesuai hand
    CHECK(ch.deselect(5) && !ch.deselect(5) && ch.size() == 4);
    CHECK(ch.toggle(7) && ch.isSelected(7) && ch.toggle(7) && !ch.isSelected(7));
    ch.clear();
    CHECK(ch.size() == 0 && !ch.isValid() && ch.toHand().cards.empty());

    ChosenHand bot(H("2H 10H 9C JH 4D QH KH AH"));          // royal flush tersembunyi
    bot.autoSelectBest([&](const Hand& h) { return rules.scoreHand(h); });
    CHECK(bot.size() == 5);
    CHECK(rules.detectRank(bot.toHand()) == HandRank::RoyalFlush);

    // ---------- Tugas 3: HandGenerator ----------
    HandGenerator gen;
    Hand h = gen.generateHand();
    CHECK(h.cards.size() == 8);
    CHECK(gen.generateHand(100).cards.size() == 52);        // dibatasi 1 deck

    bool inRange = true, noDuplicate = true;
    std::set<std::string> distinctHands;
    std::set<int> firstRanks; std::set<char> firstSuits;
    int royal = 0;
    for (int i = 0; i < 500; ++i) {
        Hand x = gen.generateHand(5);
        std::set<std::string> seen; std::string key;
        for (const Card& c : x.cards) {
            if (c.rank < 2 || c.rank > 14) inRange = false;
            if (std::string("HDCS").find(c.suit) == std::string::npos) inRange = false;
            if (!seen.insert(toString(c)).second) noDuplicate = false;
            key += toString(c) + ",";
        }
        distinctHands.insert(key);
        firstRanks.insert(x.cards[0].rank); firstSuits.insert(x.cards[0].suit);
        if (rules.detectRank(x) == HandRank::RoyalFlush) ++royal;
    }
    CHECK(inRange);
    CHECK(noDuplicate);                                     // tidak ada kartu kembar dalam 1 hand
    CHECK(distinctHands.size() > 490);                      // hand berbeda-beda (bukan hard-coded)
    CHECK(firstRanks.size() == 13 && firstSuits.size() == 4);
    CHECK(royal < 5);                                       // dulu SELALU royal flush

    HandGenerator a(42), b(42), c(43);
    Hand ha = a.generateHand(), hb = b.generateHand(), hc = c.generateHand();
    bool same = true, diff = false;
    for (size_t i = 0; i < 8; ++i) {
        if (toString(ha.cards[i]) != toString(hb.cards[i])) same = false;
        if (toString(ha.cards[i]) != toString(hc.cards[i])) diff = true;
    }
    CHECK(same && diff);                                    // seed sama -> sama, beda -> beda

    std::cout << (total - failures) << "/" << total << " tes lulus\n";
    return failures == 0 ? 0 : 1;
}
