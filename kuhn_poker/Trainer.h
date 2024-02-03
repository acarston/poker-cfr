#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
using namespace std;

#include "KuhnCFR.h"
#include "CFR.h"

class Trainer {
    public:
        int numIterations = 0;

        void Train();
        void TrainMCCFR();
        void TrainTest();
        void DisplayNodeStrategies();
    private:
        KuhnCFR bot;
        CFR testBot;
        double rootNodeUtility = 0.0;

        vector<int> GetDeck(int size);
        vector<vector<int>> GetDealPermutations(vector<int> deck);
        vector<vector<char>> GetDealPermutationsChar(vector<int> deck);
        vector<int> GetRandomIndexes(int range);
};

#endif