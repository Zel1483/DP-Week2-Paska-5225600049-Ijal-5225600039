#pragma once
#include <cstddef>
#include <functional>
#include <vector>
#include "Hand.h"

// Kartu yang DIPILIH pemain dari hand yang dibagikan (maksimal 5, seperti Balatro).
// Index yang dipakai semua fungsi bersifat 0-based terhadap hand yang dibagikan.
class ChosenHand {
public:
    static constexpr std::size_t kMaxChosen = 5;

    explicit ChosenHand(const Hand& dealt);

    bool select(std::size_t index);     // false: index salah / sudah dipilih / sudah 5 kartu
    bool deselect(std::size_t index);   // false: index salah / belum dipilih
    bool toggle(std::size_t index);     // pilih bila belum, batalkan bila sudah
    void clear();                       // batalkan semua pilihan

    bool        isSelected(std::size_t index) const;
    std::size_t size() const;           // jumlah kartu terpilih
    bool        isValid() const;        // boleh dimainkan: 1..5 kartu
    std::vector<std::size_t> indices() const;

    Hand toHand() const;                // kartu terpilih (urutan sesuai hand) -> untuk dimainkan
    const Hand& dealtHand() const { return dealt; }

    // Bot: cari kombinasi (maks. 5 kartu) dengan skor tertinggi menurut `scorer`.
    void autoSelectBest(const std::function<int(const Hand&)>& scorer);

private:
    Hand              dealt;
    std::vector<bool> picked;
    std::size_t       count = 0;
};
