#include "Node.h"

/* normalize positive regrets and update strategy sum */
vector<double> Node::GetCurrentStrategy(double reachProbablilty) {
    vector<double> strategy(NUM_ACTIONS, 0.0); 

    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        strategy[i] = (cumulativeRegrets[i] > 0) ? cumulativeRegrets[i] : 0.0;
    }
    strategy = NormalizeVector(strategy);
    for (unsigned int i = 0; i < NUM_ACTIONS; ++i){
        sumOfStrategies[i] += strategy[i] * reachProbablilty;
    }
    
    return strategy;
};

vector<double> Node::GetAverageStrategy() {
    return NormalizeVector(sumOfStrategies);
};

vector<double> Node::NormalizeVector(vector<double> vector) {
    double vectorSum = 0.0;
    for (unsigned int i = 0; i < vector.size(); ++i) vectorSum += vector[i];

    if (vectorSum > 0) {
        for (unsigned int i = 0; i < vector.size(); ++i) vector[i] /= vectorSum;
        return vector;
    }
    // give an even distribution for a vector of zeros
    for (unsigned int i = 0; i < vector.size(); ++i) vector[i] = 1.0 / NUM_ACTIONS;
    return vector;
};