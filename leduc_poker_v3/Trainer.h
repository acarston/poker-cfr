#ifndef TRAINER_H
#define TRAINER_H

#include <random>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <unordered_map>

#include "CFR.h"

class Trainer {
public:
    Trainer();

    void train(const unsigned int iterations);
    void display_strats() const;
private:
    CFR bot;

    const int NUM_HOLE_CARDS = 2;
    const int NUM_STREET_CARDS = 1;
    const int DECK_SIZE = 3;
    const int DECK_REPEATS = 1;

    double rootNodeUtil = 0.0;
    int iterations = 0;

    std::random_device dev;
    std::mt19937_64 rng{ dev() };

    std::vector<int> deck;
    std::vector<std::vector<int>> deal{ { 0, 0 }, { 0 } };

    void shuffle();
};

#endif
