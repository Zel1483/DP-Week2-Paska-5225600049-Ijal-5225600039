#ifndef ADVANCE_ROUND_H
#define ADVANCE_ROUND_H

// Menjalankan satu ronde penuh: mencetak header ronde, lalu memanggil
// fase PLAY -> SCORE -> REWARD -> SHOP secara berurutan.
void advanceRound(int roundNumber, int &money);

#endif // ADVANCE_ROUND_H