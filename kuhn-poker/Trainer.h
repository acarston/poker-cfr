#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
using namespace std;

#include "KuhnCFR.cpp"

class Trainer {
    public:
        void Train();
        void DisplayNodeStrategies();
    private:
        KuhnCFR bot;

        int numIterations = 0;
        double rootNodeUtility = 0.0;
        vector<int> currentDeal = {0, 1};

        void Shuffle(vector<int>& currentDeal);
};

#endif