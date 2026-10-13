#include <memory>
#include "base_rules.h"
#include "run_session.h"
#include "shop_system.h"

using namespace std;

int main() {
    RunSession session(
        make_unique<FixedInputGenerator>(),
        make_unique<SimpleScoringRule>(),
        make_unique<DirectRewardRule>(),
        ShopSystem{});

    session.run();
    return 0;
}