2 6 7 9#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "checker.h" // Mengambil Card, HandAnalysis, dan semua checker

using namespace std;

// --- Sistem Kartu Sederhana ---
const string RANKS[] = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
const string SUITS[] = {"H", "D", "C", "S"}; // Hearts, Diamonds, Clubs, Spades

Card drawRandomCard() {
    return {RANKS[rand() % 13], SUITS[rand() % 4]};
}

void printCards(const vector<Card>& cards) {
    for (size_t i = 0; i < cards.size(); ++i) {
        cout << "[" << i + 1 << "]" << cards[i].rank << cards[i].suit << "  ";
    }
    cout << endl;
}

// --- Sistem Skor ---
int getHandScore(const string& handType) {
    if (handType == "Flush House") return 150;
    if (handType == "Five of a Kind") return 120;
    if (handType == "Royal Flush") return 100;
    if (handType == "Straight Flush") return 80;
    if (handType == "Four of a Kind") return 60;
    if (handType == "Full House") return 40;
    if (handType == "Flush") return 35;
    if (handType == "Straight") return 30;
    if (handType == "Three of a Kind") return 20;
    if (handType == "Two Pair") return 15;
    if (handType == "Pair") return 10;
    return 5; // High Card
}

// Setup Chain of Responsibility
PokerHandChecker* setupCheckerChain() {
    auto* flushHouse = new FlushHouseChecker();
    auto* fiveOfAKind = new FiveOfAKindChecker();
    auto* royalFlush = new RoyalFlushChecker();
    auto* straightFlush = new StraightFlushChecker();
    auto* fourOfAKind = new FourOfAKindChecker();
    auto* fullHouse = new FullHouseChecker();
    auto* flush = new FlushChecker();
    auto* straight = new StraightChecker();
    auto* threeOfAKind = new ThreeOfAKindChecker();
    auto* twoPair = new TwoPairChecker();
    auto* pair = new PairChecker();
    auto* highCard = new HighCardChecker();

    flushHouse->setNext(fiveOfAKind);
    fiveOfAKind->setNext(royalFlush);
    royalFlush->setNext(straightFlush);
    straightFlush->setNext(fourOfAKind);
    fourOfAKind->setNext(fullHouse);
    fullHouse->setNext(flush);
    flush->setNext(straight);
    straight->setNext(threeOfAKind);
    threeOfAKind->setNext(twoPair);
    twoPair->setNext(pair);
    pair->setNext(highCard);

    return flushHouse; 
}

int main() {
    srand(static_cast<unsigned>(time(0))); 

    PokerHandChecker* handChecker = setupCheckerChain();

    int totalScore = 0;
    int targetScore = 500;
    int handsLeft = 4;

    cout << "=== Balatro-like Console Game ===" << endl;
    cout << "Target Skor: " << targetScore << endl;

    // --- Core Loop ---
    while (handsLeft > 0 && totalScore < targetScore) {
        cout << "\n=================================" << endl;
        cout << "RONDE BARU! (Skor: " << totalScore << "/" << targetScore << " | Sisa Hand: " << handsLeft << ")" << endl;
        
        vector<Card> currentCards(9);
        for (int i = 0; i < 9; ++i) {
            currentCards[i] = drawRandomCard();
        }

        int phase = 1;
        bool playHandNow = false;

        // --- Phase Loop (Maksimal 3 Phase) ---
        while (phase <= 3 && !playHandNow) {
            cout << "\n--- Phase " << phase << " / 3 ---" << endl;
            cout << "Kartu Anda di meja:" << endl;
            printCards(currentCards);

            if (phase < 3) {
                cout << "\nMasukkan nomor kartu (1-9) yang ingin di-KEEP, pisahkan spasi (cth: 1 3 5 9)." << endl;
                cout << "Atau ketik 'PLAY' untuk langsung memainkan kartu ini: ";
                
                string input;
                getline(cin >> ws, input);

                string upperInput = input;
                transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);

                if (upperInput == "PLAY") {
                    playHandNow = true;
                } else {
                    stringstream ss(input);
                    int index;
                    vector<bool> keepCard(9, false);
                    
                    while (ss >> index) {
                        if (index >= 1 && index <= 9) {
                            keepCard[index - 1] = true;
                        }
                    }

                    for (int i = 0; i < 9; ++i) {
                        if (!keepCard[i]) {
                            currentCards[i] = drawRandomCard();
                        }
                    }
                    phase++;
                }
            } else {
                cout << "Ini adalah Phase terakhir. Mengevaluasi kartu..." << endl;
                break;
            }
        }

        // --- Evaluasi Hand Aktual ---
        cout << "\n>> Kartu Final yang Dievaluasi: <<\n";
        printCards(currentCards);
        
        // 1. Buat analisis dari state kartu yang sekarang
        HandAnalysis currentAnalysis(currentCards);
        
        // 2. Kirim hasil analisis ke awal rantai checker
        string playedHand = handChecker->checkHand(currentAnalysis); 
        
        int scoreGained = getHandScore(playedHand);
        totalScore += scoreGained;
        handsLeft--;

        cout << "\nHASIL CHECKER: Anda memainkan kombinasi [" << playedHand << "]!" << endl;
        cout << "Mendapatkan +" << scoreGained << " poin." << endl;
        cout << "Total Skor Sekarang: " << totalScore << endl;
    }

    // --- Evaluasi Menang/Kalah ---
    cout << "\n=================================" << endl;
    if (totalScore >= targetScore) {
        cout << "SELAMAT! Anda MENANG dengan mencapai target skor (" << totalScore << "/" << targetScore << ")." << endl;
    } else {
        cout << "GAME OVER! Sisa Hand habis. Skor akhir Anda: " << totalScore << " / " << targetScore << endl;
    }

    return 0;
}