#include "run_session.h"
#include <iostream>

using namespace std;

RunSession::RunSession(unique_ptr<IInputGenerator> inputGen,
                        unique_ptr<IScoringRule> scoring,
                        unique_ptr<IRewardRule> reward,
                        ShopSystem shop)
    : inputGen_(move(inputGen)),
      scoring_(move(scoring)),
      reward_(move(reward)),
      shop_(move(shop)) {}

void RunSession::run() {
    cout << "=== RUN START ===\n";
    for (int round = 1; round <= kTotalRounds; ++round) {
        cout << "\nRound " << round << "\n";

        // 1. Generate input
        TurnInput input = inputGen_->generate(round);
        cout << "[PLAY] input generated: " << input.value << "\n";

        // 2. Compute base score
        int baseScore = scoring_->computeBase(input);
        cout << "[SCORE] base score: " << baseScore << "\n";

        // 3. Compute reward
        int gain = reward_->computeGain(baseScore, round);
        cout << "[REWARD] gain: " << gain << "\n";

        // 4. Update money
        state_.money += gain + state_.bonus - state_.pendingCost;
        if (state_.bonus != 0 || state_.pendingCost != 0) {
            cout << "[MONEY] item bonus: +" << state_.bonus
                 << " | item cost: -" << state_.pendingCost << "\n";
        }
        state_.pendingCost = 0;
        cout << "[MONEY] money: " << state_.money << "\n";

        // 5. Shop phase
        shop_.run(state_, round < kTotalRounds);

        // 6. Advance round
        cout << "[ROUND] round " << round << " done\n";
    }
    cout << "\n=== RUN END ===\n";
    cout << "Final money: " << state_.money << "\n";
}