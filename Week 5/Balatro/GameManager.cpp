#include "GameManager.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "ChosenHand.h"

namespace {

struct Blind {
    const char* name;
    int         target;   // skor minimum agar blind dianggap menang
};

const Blind kBlinds[] = {
    {"Small Blind",  50},
    {"Big Blind",    80},
    {"Boss Blind",  120},
};

constexpr int kHandsPerBlind = 4;

// Menampilkan kartu beserta nomor urutnya (1-8) untuk mempermudah pemain
void printCards(const char* label, const Hand& hand) {
    std::cout << label << ":";
    for (size_t i = 0; i < hand.cards.size(); ++i) {
        std::cout << " [" << (i + 1) << ": " << toString(hand.cards[i]) << "]";
    }
    std::cout << '\n';
}

}  // namespace

void GameManager::runSession() {
    std::cout << "=== Run Started ===\n";
    int money = 0;

    for (const Blind& blind : kBlinds) {
        blindRule = BlindRule(blind.target);
        int totalScore = 0;
        std::cout << "\n=========================================\n";
        std::cout << "--- " << blind.name << " (Target Skor: " << blind.target << ") ---\n";

        for (int played = 0; played < kHandsPerBlind; ++played) {
            std::cout << "\n[ Hand Ke-" << (played + 1) << " dari " << kHandsPerBlind << " ]\n";
            
            // 1. Bagi 8 kartu awal
            Hand dealt = handGenerator.generateHand();
            
            int phase = 1;
            bool playNow = false;

            // --- LOOP 3 PHASE (Mencari Hand Terbaik) ---
            while (phase <= 3 && !playNow) {
                std::cout << "\n--- Phase " << phase << " / 3 ---\n";
                printCards("Kartu Anda", dealt);

                if (phase < 3) {
                    std::cout << "\nMasukkan nomor kartu (1-" << dealt.cards.size() 
                              << ") yang ingin di-KEEP (sisanya diganti), pisahkan spasi (cth: 1 3 5 8).\n";
                    std::cout << "Atau ketik 'PLAY' jika ingin langsung memilih dan memainkan kartu saat ini: ";

                    std::string input;
                    std::getline(std::cin, input);

                    if (input.empty()) continue;

                    std::string upperInput = input;
                    for (char& c : upperInput) c = static_cast<char>(std::toupper(c));

                    if (upperInput == "PLAY") {
                        playNow = true;
                    } else {
                        // Tandai kartu mana yang di-keep
                        std::istringstream iss(input);
                        std::vector<bool> keep(dealt.cards.size(), false);
                        int idx;
                        while (iss >> idx) {
                            if (idx >= 1 && idx <= static_cast<int>(dealt.cards.size())) {
                                keep[idx - 1] = true;
                            }
                        }

                        // Ganti kartu yang TIDAK di-keep dengan kartu baru dari deck
                        Hand freshDeck = handGenerator.generateHand(16);
                        int freshIdx = 0;

                        for (size_t i = 0; i < dealt.cards.size(); ++i) {
                            if (!keep[i]) {
                                // Cari kartu baru yang belum ada di tangan
                                while (freshIdx < static_cast<int>(freshDeck.cards.size())) {
                                    const Card& candidate = freshDeck.cards[freshIdx++];
                                    bool duplicate = false;
                                    for (const Card& c : dealt.cards) {
                                        if (c.rank == candidate.rank && c.suit == candidate.suit) {
                                            duplicate = true;
                                            break;
                                        }
                                    }
                                    if (!duplicate) {
                                        dealt.cards[i] = candidate;
                                        break;
                                    }
                                }
                            }
                        }
                        phase++;
                    }
                } else {
                    std::cout << "\nPhase 3 selesai! Mengevaluasi pemilihan kartu akhir...\n";
                    break;
                }
            }

            // --- INPUT MANUAL: PILIH 1-5 KARTU UNTUK DIMAINKAN ---
            ChosenHand chosen(dealt);
            bool validSelection = false;

            while (!validSelection) {
                std::cout << "\nKartu Akhir: ";
                printCards("Meja", dealt);
                std::cout << "Pilih 1 hingga 5 kartu untuk DIMAINKAN (masukkan angka 1-" 
                          << dealt.cards.size() << " dipisah spasi, cth: 1 2 4 5 7): ";

                std::string input;
                std::getline(std::cin, input);

                if (input.empty()) continue;

                std::istringstream iss(input);
                int index;
                chosen.clear();

                while (iss >> index) {
                    if (index >= 1 && index <= static_cast<int>(dealt.cards.size())) {
                        chosen.select(index - 1);
                    }
                }

                if (chosen.isValid()) {
                    validSelection = true;
                } else {
                    std::cout << "[!] Pilihan tidak valid! Anda harus memilih minimal 1 dan maksimal 5 kartu.\n";
                }
            }

            Hand hand = chosen.toHand();

            // 2. Mainkan & Hitung Skor
            handPlayer.playHand(hand);
            HandRank rank  = scoringRule.detectRank(hand);
            int      score = scoringRule.convertRankToScore(rank);
            totalScore += score;
            std::cout << "=> " << toString(rank) << " (+" << score << ") | Total Skor: "
                      << totalScore << '/' << blind.target << '\n';

            // 3. Target Blind tercapai -> Blind selesai lebih awal
            if (blindRule.checkBlind(totalScore)) break;
        }

        // 4. Evaluasi Menang / Kalah per Blind
        bool win = blindRule.checkBlind(totalScore);
        int reward = rewardRule.earnMoney(win, totalScore);
        money += reward;
        std::cout << "\nHASIL: " << (win ? "MENANG" : "KALAH") << " | Hadiah Uang: $" << reward
                  << " | Total Uang: $" << money << '\n';

        if (!win) break;
    }
    std::cout << "\n=== Run Ended ===\n";
}