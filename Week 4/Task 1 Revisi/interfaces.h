#pragma once
// Dipegang: ORANG A
// Bagian MUTABLE - titik ekstensi sistem. RunSession hanya akan
// bergantung pada interface ini, tidak pada implementasi konkretnya.

#include "types.h"

class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;
    virtual TurnInput generate(int round) = 0;
};

class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int computeBase(const TurnInput& input) const = 0;
};

class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int computeGain(int baseScore, int round) const = 0;
};