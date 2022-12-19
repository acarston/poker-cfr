#include <random>
using namespace std;

#include "Trainer.h"

void Trainer::Train() {
};

void Trainer::DisplayNodeStrategies() {
};

//shuffles deck and deals from top
void Trainer::Shuffle(vector<int>& currentDeal) { 
    int j, temp;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist6(0, deck.size() - 1);
    
    //Fisher-Yates shuffling algorithm
    for (unsigned int i = deck.size() - 1; i > 0; --i) {
        j = dist6(rng);
        temp = deck.at(i);
        deck.at(i) = deck.at(j);
        deck.at(j) = temp;
    }

    for (unsigned int i = 0; i < currentDeal.size(); ++i) currentDeal.at(i) = deck.at(i);
};