#ifndef TRAINER_H
#define TRAINER_H

#include <random>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <unordered_map>

#include "CFR.h"

class Trainer {
private:
    CFR bot;

    const int NUM_HOLE_CARDS = 2;
    const int NUM_STREET_CARDS = 1;
    const int DECK_SIZE = 3;
    const int DECK_REPEATS = 2;
    const int CARD_LEN = 2;
    const int ACTION_LEN = 3;
    const int NUM_STREETS = 1;

    int iterations = 0;

    std::random_device dev;
    std::mt19937_64 rng { dev() };

    std::vector<int> deck;
    std::vector<std::vector<int>> deal { { 0, 0 }, { 0 } };

    void shuffle();

public:
    Trainer();

    void train(const unsigned int iterations);
    void display_strats() const;
    void cont_explore(const std::vector<int>& holeCards, const std::vector<int>& streetCards, std::vector<int> pot, int passedStreets = 0, int sinceChance = 0, int infoset = 0, int numPastActions = 0);
    void explore();
};

#endif
