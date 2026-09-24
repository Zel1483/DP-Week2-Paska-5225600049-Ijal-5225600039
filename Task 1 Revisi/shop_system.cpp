#include "shop_system.h"
#include <iostream>
#include <string>

using namespace std;

void ShopSystem::run(RunState& state, bool canBuy) const {
    const string name = "Bonus(+2)";
    const int cost = 2;
    const int bonus = 2;

    cout << "[SHOP] offered: " << name << " cost " << cost
         << " | money: " << state.money << "\n";

    if (!canBuy) {
        cout << "[SHOP] last round, no next round to use it -> skipped\n";
        return;
    }

    if (state.money < cost) {
        cout << "[SHOP] not enough money -> skipped\n";
        return;
    }

    int choice = askChoice();
    if (choice == 1) {
        state.pendingCost += cost;  // ditagih ronde depan
        state.bonus += bonus;       // dipakai ronde depan
        cout << "[SHOP] bought: " << name << " (cost is paid next round)\n";
    } else {
        cout << "[SHOP] skipped\n";
    }
}

int ShopSystem::askChoice() {
    string line;
    while (true) {
        cout << "  1) Buy   2) Skip\n> ";
        if (!getline(cin, line)) return 2;  // input habis -> skip
        if (line == "1") return 1;
        if (line == "2") return 2;
        cout << "  Invalid choice.\n";
    }
}