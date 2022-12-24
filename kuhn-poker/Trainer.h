#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
using namespace std;

#include "KuhnCFR.cpp"

class Trainer {
    public:
        void Train(int numIterations);
        void DisplayNodeStrategies(int numIterations);
    private:
        KuhnCFR bot;

        double rootNodeUtility = 0.0;
        vector<int> deck;
        vector<int> currentDeal = {0, 1};

        void SetDeck(vector<int>& deck);
        void Shuffle(vector<int>& currentDeal);
};

#endif