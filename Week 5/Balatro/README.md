# Balatro - Design Pattern Week 5 (Chain of Responsibility)

Build (butuh C++17):

    make game && ./game      # jalankan game
    make test                # jalankan 43 tes otomatis

Tanpa make:

    g++ -std=c++17 main.cpp HandGenerator.cpp ChosenHand.cpp HandPlayer.cpp GameManager.cpp -o game
    cl /std:c++17 /EHsc main.cpp HandGenerator.cpp ChosenHand.cpp HandPlayer.cpp GameManager.cpp   (MSVC)

Pemetaan ke soal:
1. ChosenHand        -> ChosenHand.h / ChosenHand.cpp
2. Logic checker     -> PokerHandChecker.h (base) + PokerHandCheckers.h (12 checker) + Rules.h (rantai)
3. HandGenerator acak -> HandGenerator.cpp (shuffle deck 52 kartu)
