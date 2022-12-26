#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
#include <chrono>
using namespace std;

#include "KuhnCFR.cpp"

class Trainer {
    public:
        void Train(int numIterations);
        void DisplayNodeStrategies(int numIterations);
    private:
        KuhnCFR bot;

        const int FIRST_CARD = 0, CARD_INCREMENT = 1;
        chrono::seconds executionTime;
        double rootNodeUtility = 0.0;
        vector<int> deck = {};

        vector<vector<int>> GetDealPermutations(vector<int> deck);
        void SetDeckSize(vector<int>& deck, int size);
        void Shuffle(vector<int>& currentDeal);
        vector<int> GetRandomPermutation(vector<vector<int>> permutations);
};

#endif