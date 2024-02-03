#include <random>
#include <iostream>
#include <algorithm>

#include "Trainer.h"

/* use CFR starting with a predetermined random deal for a number of iterations */
void Trainer::train(const unsigned int iterations) {
    this->iterations = iterations;

    std::vector<int> deck = get_deck(3);
    std::vector<std::vector<int>> dealPerms = get_deal_perms(deck);
    std::vector<int> randIndexes = get_rand_indexes(dealPerms.size() - 1);

    for (unsigned int i = 0; i < iterations; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (j == 1) rootNodeUtil += bot.mccfr(j, i + 1, dealPerms[randIndexes[i]]);
            else bot.mccfr(j, i + 1, dealPerms[randIndexes[i]]);
        }
    }
};

void Trainer::display_strats() const {
    std::cout << "Average utility of the root node:\n" << (rootNodeUtil / iterations) << "\n\n";
    std::cout << "Node Strategies:\n";

    double* strategy = nullptr;
    for (auto it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        strategy = it->second->avg_strategy();
        std::cout << it->first << ": ";
        std::cout << "bet " << strategy[0] << ", pass " << strategy[1] << "\n";
    }
    std::cout << "\n\nThis program was trained for " << iterations << " iterations." << std::endl;
};

std::vector<int> Trainer::get_deck(const int size) const {
    std::vector<int> deck;
    for (int i = 0; i < size; ++i) deck.push_back(i);
    return deck;
};

std::vector<std::vector<int>> Trainer::get_deal_perms(const std::vector<int>& deck) const {
    std::vector<std::vector<int>> dealPerms;

    for (int i = deck[0]; i < deck.size() - 1; ++i) {
        for (int j = i + 1; j < deck.size(); ++j) {
            dealPerms.push_back({ i, j });
        }
    }
    for (int i = deck.size() - 1; i > 0; --i) {
        for (int j = i - 1; j > 0 - 1; --j) {
            dealPerms.push_back({ i, j });
        }
    }

    return dealPerms;
};


/* generate a vector of random indexes with a size equal to the number of iterations */
std::vector<int> Trainer::get_rand_indexes(const int range) const {
    std::vector<int> indexes(iterations);

    std::random_device dev;
    std::mt19937_64 rng(dev());
    std::uniform_int_distribution<int> dist(0, range);

    for (int i = 0; i < iterations; ++i) indexes[i] = dist(rng);

    return indexes;
};