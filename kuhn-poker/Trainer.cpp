#include <random>
#include <iostream>
#include <algorithm>

#include "Trainer.h"

/* use CFR starting with a predetermined random deal for a number of iterations */
void Trainer::Train() {
    vector<int> deck = GetDeck(3);
    vector<vector<int>> dealPermutations = GetDealPermutations(deck);
    vector<int> randomIndexes = GetRandomIndexes(dealPermutations.size() - 1);

    for (unsigned int i = 0; i < numIterations; ++i) {
        rootNodeUtility += bot.CalculateUtilities(dealPermutations[randomIndexes[i]]);
    }
};

void Trainer::DisplayNodeStrategies() {
    cout << "Average utility of the root node:\n" << (rootNodeUtility / numIterations) << "\n\n";
    cout << "Node Strategies:\n";

    unordered_map<string, Node>::iterator it;
    vector<double> strategy = {};

    for (it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        strategy = it->second.GetAverageStrategy();
        cout << it->first << ": ";
        cout << "bet " << strategy[0] << ", pass " << strategy[1] << "\n";
    }
    cout << "\n\nThis program was trained for " << numIterations << " iterations." << endl;
};

vector<int> Trainer::GetDeck(int size) {
    vector<int> deck;
    for (unsigned int i = 0; i < size; ++i) deck.push_back(i);
    return deck;
};

/* get all possible deals from a deck of integers */
vector<vector<int>> Trainer::GetDealPermutations(vector<int> deck) {
    vector<vector<int>> dealPermutations = {};

    for (int i = deck[0]; i < deck.size() - 1; ++i) {
        for (int j = i + 1; j < deck.size(); ++j) {
            dealPermutations.push_back({i, j});
        }
    }
    for (int i = deck.size() - 1; i > 0; --i) {
        for (int j = i - 1; j > 0 - 1; --j) {
            dealPermutations.push_back({i, j});
        }
    }

    return dealPermutations;
};

/* generate a vector of random indexes with a size equal to the number of iterations */
vector<int> Trainer::GetRandomIndexes(int range) {
    vector<int> indexes;

    std::random_device dev;
    std::mt19937_64 rng(dev());
    uniform_int_distribution<int> dist(0, range);

    for (unsigned int i = 0; i < numIterations; ++i) indexes.push_back(dist(rng));
    shuffle(indexes.begin(), indexes.end(), rng);

    return indexes;
};