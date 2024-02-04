#ifndef TRAINER_H
#define TRAINER_H

#include <random>
#include <iostream>
#include <algorithm>
#include <bitset>

#include "CFR.h"

class Trainer {
public:
    void train(const unsigned int iterations);
    void display_strats() const;
private:
    CFR bot;
    double rootNodeUtil = 0.0;
    int iterations = 0;

    std::vector<int> get_deck(const int size, const int repeats) const;
    std::vector<std::vector<int>> get_deal_perms(const std::vector<int>& deck) const; // deprecate pls
    // std::vector<int> get_rand_indexes(const int range) const;
    std::vector<std::vector<std::vector<int>>> get_deals(std::vector<int>& cards, const int numHoleCards, const int numStreetCards) const;
};

#endif
