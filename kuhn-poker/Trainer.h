#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
using namespace std;

#include "KuhnCFR.cpp"

class Trainer {
    public:
        int numIterations = 0;

        void Train();
        void DisplayNodeStrategies();
    private:
        KuhnCFR bot;
        double rootNodeUtility = 0.0;

        vector<int> GetDeck(int size);
        vector<vector<int>> GetDealPermutations(vector<int> deck);;
        vector<int> GetRandomIndexes(int range);
};

#endif