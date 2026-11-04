#include "ChosenHand.h"

#include <algorithm>

ChosenHand::ChosenHand(const Hand& dealtHand)
    : dealt(dealtHand), picked(dealtHand.cards.size(), false) {}

bool ChosenHand::select(std::size_t index) {
    if (index >= dealt.cards.size() || picked[index] || count >= kMaxChosen) return false;
    picked[index] = true;
    ++count;
    return true;
}

bool ChosenHand::deselect(std::size_t index) {
    if (index >= dealt.cards.size() || !picked[index]) return false;
    picked[index] = false;
    --count;
    return true;
}

bool ChosenHand::toggle(std::size_t index) {
    if (index >= dealt.cards.size()) return false;
    return picked[index] ? deselect(index) : select(index);
}

void ChosenHand::clear() {
    std::fill(picked.begin(), picked.end(), false);
    count = 0;
}

bool ChosenHand::isSelected(std::size_t index) const {
    return index < picked.size() && picked[index];
}

std::size_t ChosenHand::size() const { return count; }

bool ChosenHand::isValid() const { return count >= 1 && count <= kMaxChosen; }

std::vector<std::size_t> ChosenHand::indices() const {
    std::vector<std::size_t> result;
    for (std::size_t i = 0; i < picked.size(); ++i)
        if (picked[i]) result.push_back(i);
    return result;
}

Hand ChosenHand::toHand() const {
    Hand hand;
    for (std::size_t i : indices()) hand.cards.push_back(dealt.cards[i]);
    return hand;
}

void ChosenHand::autoSelectBest(const std::function<int(const Hand&)>& scorer) {
    const std::size_t n = dealt.cards.size();
    if (n == 0) return;
    const std::size_t k = std::min(kMaxChosen, n);

    // Menelusuri semua kombinasi k dari n kartu (C(8,5) = 56 -> sangat ringan).
    std::vector<char> mask(n, 0);
    std::fill(mask.begin(), mask.begin() + k, 1);

    int best = -1;
    std::vector<char> bestMask = mask;
    do {
        Hand candidate;
        for (std::size_t i = 0; i < n; ++i)
            if (mask[i]) candidate.cards.push_back(dealt.cards[i]);
        int score = scorer(candidate);
        if (score > best) { best = score; bestMask = mask; }
    } while (std::prev_permutation(mask.begin(), mask.end()));

    clear();
    for (std::size_t i = 0; i < n; ++i)
        if (bestMask[i]) select(i);
}
