#pragma once

#include "interfaces.h"

class FixedInputGenerator : public IInputGenerator {
public:
    TurnInput generate(int round) override;
};

class SimpleScoringRule : public IScoringRule {
public:
    int computeBase(const TurnInput& input) const override;
};

class DirectRewardRule : public IRewardRule {
public:
    int computeGain(int baseScore, int round) const override;
};