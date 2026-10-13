#pragma once
// RunSession (INVARIANT) - hanya mengatur urutan fase. Tidak ada
// logika scoring, reward, atau pembuatan input di dalam class ini.

#include <memory>
#include "interfaces.h"
#include "shop_system.h"  // kontrak ShopSystem::run(...) disepakati bareng Orang B

class RunSession {
public:
    RunSession(std::unique_ptr<IInputGenerator> inputGen,
               std::unique_ptr<IScoringRule> scoring,
               std::unique_ptr<IRewardRule> reward,
               ShopSystem shop);

    void run();

private:
    static constexpr int kTotalRounds = 3;

    std::unique_ptr<IInputGenerator> inputGen_;
    std::unique_ptr<IScoringRule> scoring_;
    std::unique_ptr<IRewardRule> reward_;
    ShopSystem shop_;
    RunState state_;
};