#pragma once
// Dipegang: ORANG A
// Struct data murni - tidak ada logika di sini.

struct TurnInput {
    int value;
};

struct RunState {
    int money = 0;
    int bonus = 0;        // efek item shop: tambahan uang tiap round
    int pendingCost = 0;  // biaya item shop, dipotong di Update Money round berikutnya
};