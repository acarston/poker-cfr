#include <random>
#include <iostream>

#include "Trainer.h"

void Trainer::Train(int numIterations) {
    vector<int> count = {3,0}; //testing random distros
    for (unsigned int i = 0; i < numIterations; ++i) {
        Shuffle(currentDeal);
        // for (auto j : count) {
        //     //FIXME: test at shuffle method
        // }
        rootNodeUtility += bot.CalculateUtilities({1,2});
    }
    //cout << count << "\n" << bot.count << "\n\n\n";
};

void Trainer::DisplayNodeStrategies(int numIterations) {
    cout << "Average utility of the root node:\n" << (rootNodeUtility / numIterations) << "\n\n";
    cout << "Node Strategies:\n";

    unordered_map<string, Node>::iterator it;
    vector<double> strategy(currentDeal.size(), 0.0);

    for (it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        //if (it->first.size() < 2) continue; UPDATE
        strategy = it->second.GetAverageStrategy();
        cout << it->first << ": ";
        cout << "bet " << strategy.at(0) << ", pass " << strategy.at(1) << "\n";
    }
    cout << "\n\nThis program was trained for " << numIterations << " iterations." << endl;
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