#include "base_rules.h"

TurnInput FixedInputGenerator::generate(int /*round*/) {
    return TurnInput{3};
}

int SimpleScoringRule::computeBase(const TurnInput& input) const {
    return input.value;
}

int DirectRewardRule::computeGain(int baseScore, int /*round*/) const {
    return baseScore;
}