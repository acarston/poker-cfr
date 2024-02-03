#ifndef TRAINER_H
#define TRAINER_H

#include <iostream>
#include <algorithm>

#include "CFR.h"

class Trainer {
public:
    void train(const unsigned int iterations);
    void display_strats() const;
private:
    CFR bot;
    double rootNodeUtil = 0.0;
    int iterations = 0;

    std::vector<int> get_deck(const int size) const;
    std::vector<std::vector<int>> get_deal_perms(const std::vector<int>& deck) const;
    std::vector<int> get_rand_indexes(const int range) const;
};

#endif
