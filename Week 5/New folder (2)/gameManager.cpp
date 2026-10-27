#include "GameManager.h"

#include <iostream>

namespace {

struct Blind {
    const char* name;
    int         target;   // skor minimum agar blind dianggap menang
};

// Target sengaja kecil karena skor di sini hanya chips x mult dari jenis hand
// (belum ada chip kartu / joker). Ubah sesuai kebutuhan.
const Blind kBlinds[] = {
    {"Small Blind",  50},
    {"Big Blind",    80},
    {"Boss Blind",  120},
};

constexpr int  kHandsPerBlind = 4;
constexpr bool kShowDetails   = false;   // true -> cetak juga skor, menang/kalah, uang

}  // namespace

void GameManager::runSession() {
    int money = 0;

    for (const Blind& blind : kBlinds) {
        blindRule = BlindRule(blind.target);
        int totalScore = 0;

        for (int played = 0; played < kHandsPerBlind; ++played) {
            // 1. Buat hand
            Hand hand = handGenerator.generateHand();

            // 2. Cek jenis hand lewat rantai checker, lalu CETAK jenis hand-nya
            HandRank rank = scoringRule.detectRank(hand);
            std::cout << toString(rank) << '\n';

            // 3. Hitung skor & akumulasi
            totalScore += scoringRule.convertRankToScore(rank);

            // 4. Target tercapai -> blind selesai lebih awal
            if (blindRule.checkBlind(totalScore)) break;
        }

        // 5. Menang/kalah blind, lalu hadiah uang
        bool win = blindRule.checkBlind(totalScore);
        money += rewardRule.earnMoney(win, totalScore);

        if constexpr (kShowDetails) {
            std::cout << "[" << blind.name << "] skor " << totalScore << '/' << blind.target
                      << (win ? " MENANG" : " KALAH") << " | uang $" << money << '\n';
        }

        // 6. Kalah -> sesi berakhir
        if (!win) break;
    }
}