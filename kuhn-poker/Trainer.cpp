#include <random>
#include <iostream>

#include "Trainer.h"

void Trainer::Train(int numIterations) {
    SetDeckSize(deck, 3);
    vector<vector<int>> dealPermutations = GetDealPermutations(deck);
    vector<int> permutation = {};

    auto time0 = chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < numIterations; ++i) {
        permutation = GetRandomPermutation(dealPermutations);
        rootNodeUtility += bot.CalculateUtilities(permutation);
    }
    auto time1 = chrono::high_resolution_clock::now();
    executionTime = chrono::duration_cast<chrono::milliseconds>(time1 - time0);
};

void Trainer::DisplayNodeStrategies(int numIterations) {
    cout << "Average utility of the root node:\n" << (rootNodeUtility / numIterations) << "\n\n";
    cout << "Node Strategies:\n";

    unordered_map<string, Node>::iterator it;
    vector<double> strategy = {};

    for (it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        strategy = it->second.GetAverageStrategy();
        cout << it->first << ": ";
        cout << "bet " << strategy[0] << ", pass " << strategy[1] << "\n";
    }
    cout << "\n\nThis program was trained for " << numIterations << " iterations, taking " << executionTime.count() << " milliseconds to execute." << endl;
};

vector<vector<int>> Trainer::GetDealPermutations(vector<int> deck) {
    vector<vector<int>> dealPermutations = {};

    for (int i = deck[FIRST_CARD]; i < deck.size() - CARD_INCREMENT; ++i) {
        for (int j = i + CARD_INCREMENT; j < deck.size(); ++j) {
            dealPermutations.push_back({i, j});
        }
    }
    for (int i = deck.size() - CARD_INCREMENT; i > FIRST_CARD; --i) {
        for (int j = i - CARD_INCREMENT; j > FIRST_CARD - CARD_INCREMENT; --j) {
            dealPermutations.push_back({i, j});
        }
    }

    return dealPermutations;
};

void Trainer::SetDeckSize(vector<int>& deck, int size) {
    deck = {};
    for (unsigned int i = 0; i < size; ++i) deck.push_back(i);
};

vector<int> Trainer::GetRandomPermutation(vector<vector<int>> permutations) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(dev());
    uniform_int_distribution<int> dist(FIRST_CARD, permutations.size() - CARD_INCREMENT);
    return permutations[dist(rng)];
};