#include "compute_reward.h"
#include "update_money.h"
#include <iostream>

int computeReward(int baseScore, int &money) {
    int gain = baseScore; // aturan reward sederhana
    updateMoney(money, gain);
    std::cout << "[REWARD] gain: " << gain << " | money: " << money << std::endl;
    return gain;
}