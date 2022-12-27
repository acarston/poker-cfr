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
        chrono::milliseconds executionTime;
        double rootNodeUtility = 0.0;
        vector<int> deck = {};

        void SetDeckSize(vector<int>& deck, int size);
        vector<vector<int>> GetDealPermutations(vector<int> deck);
        vector<int> GetRandomPermutation(vector<vector<int>> permutations);
};

#endif