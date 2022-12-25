#include <random>
#include <iostream>

#include "Trainer.h"

void Trainer::Train(int numIterations) {
    SetDeckSize(deck, 3);
    vector<vector<int>> dealPermutations = GetDealPermutations(deck);

    for (unsigned int i = 0; i < numIterations; ++i) {
        for (vector<int> permutation : dealPermutations) {
            rootNodeUtility += bot.CalculateUtilities(permutation);
        }
    }
};

void Trainer::DisplayNodeStrategies(int numIterations) {
    cout << "Average utility of the root node:\n" << (rootNodeUtility / numIterations) << "\n\n";
    cout << "Node Strategies:\n";

    unordered_map<string, Node>::iterator it;
    vector<double> strategy(2, 0.0);

    for (it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        strategy = it->second.GetAverageStrategy();
        cout << it->first << ": ";
        cout << "bet " << strategy.at(0) << ", pass " << strategy.at(1) << "\n";
    }
    cout << "\n\nThis program was trained for " << numIterations << " iterations." << endl;
};

vector<vector<int>> Trainer::GetDealPermutations(vector<int> deck) {
    const int FIRST_CARD = 0, CARD_INCREMENT = 1;
    vector<vector<int>> dealPermutations = {};

    for (int i = deck.at(FIRST_CARD); i < deck.size() - CARD_INCREMENT; ++i) {
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

/* shuffle deck and deal from top */
void Trainer::Shuffle(vector<int>& currentDeal) { 
    vector<int> deck = {0, 1, 2};

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist6(0, deck.size() - 1);
    
    // Fisher-Yates shuffling algorithm
    int j, temp;
    for (unsigned int i = deck.size() - 1; i > 0; --i) {
        j = dist6(rng);
        temp = deck.at(i);
        deck.at(i) = deck.at(j);
        deck.at(j) = temp;
    }

    for (unsigned int i = 0; i < currentDeal.size(); ++i) currentDeal.at(i) = deck.at(i);
};

void Trainer::SetDeckSize(vector<int>& deck, int size) {
    deck = {};
    for (unsigned int i = 0; i < size; ++i) deck.push_back(i);
};