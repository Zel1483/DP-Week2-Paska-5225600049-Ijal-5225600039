#include "advance_round.h"
#include "input_generator.h"
#include "compute_base_score.h"
#include "compute_reward.h"
#include "shop_phase.h"
#include <iostream>

void advanceRound(int roundNumber, int &money) {
    std::cout << "\nRound " << roundNumber << std::endl;

    int input     = generateInput();
    int baseScore = computeBaseScore(input);
    computeReward(baseScore, money);
    shopPhase(money);
}