#pragma once
#include "types.h"

class ShopSystem {
public:
    void run(RunState& state, bool canBuy) const;

private:
    static int askChoice();
};