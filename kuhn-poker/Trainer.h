#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
using namespace std;

#include "KuhnCFR.cpp"

class Trainer {
    public:
        void Train();
        void DisplayNodeStrategies(); //retrieves hashmap and prints valuess
    private:
        KuhnCFR bot;
        vector<int> deck = {0, 1, 2};
        vector<int> currentDeal = {0, 1};
        int numIterations = 0;

        void Shuffle(vector<int>& currentDeal);
};

#endif