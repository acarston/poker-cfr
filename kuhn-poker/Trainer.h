#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
using namespace std;

#include "KuhnCfrBot.cpp"

class Trainer {
    public:
        void Train();
        void DisplayNodeStrategies(); //retrieves hashmap and prints values
    private:
        KuhnCfrBot bot;
        vector<int> deck = {0, 1, 2};
        vector<int> currentDeal = {0, 1};

        void Shuffle(vector<int>& currentDeal);
};

#endif