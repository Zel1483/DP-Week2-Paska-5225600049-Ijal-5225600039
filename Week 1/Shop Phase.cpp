#include "shop_phase.h"
#include "update_money.h"
#include <cstdlib>
#include <iostream>

void shopPhase(int &money) {
    int bonus = rand() % 3 + 1; // bonus +1 sampai +3
    int cost  = rand() % 5 + 1; // harga 1 sampai 5
    std::cout << "[SHOP] offered: Bonus(+" << bonus << ") cost " << cost << std::endl;

    // Beli hanya jika uang cukup DAN "lemparan koin" 50% berhasil
    bool buy = (money >= cost) && (rand() % 2 == 0);
    if (buy) {
        updateMoney(money, -cost);
        updateMoney(money, bonus);
        std::cout << "[SHOP] purchased | money: " << money << std::endl;
    } else {
        std::cout << "[SHOP] skipped" << std::endl;
    }
}