#include <random>
#include <iostream>

#include "Trainer.h"

void Trainer::Train() {
    cout << "How many iterations?" << endl;
    cin >> numIterations;

    for (unsigned int i = 0; i < numIterations; ++i) {
        Shuffle(currentDeal);
        rootNodeUtility += bot.CalculateUtilities(currentDeal);
    }
};

void Trainer::DisplayNodeStrategies() {
    cout << "Utility of the root node: " << rootNodeUtility << "\n\n";
    cout << "Node Strategies:\n";

    unordered_map<string, Node>::iterator it;
    vector<double> strategy(currentDeal.size(), 0.0);

    for (it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        strategy = it->second.GetAverageStrategy();
        cout << it->first << ": ";
        cout << "bet " << strategy.at(0) << ", pass " << strategy.at(1) << "\n";
    }
    cout << endl;
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