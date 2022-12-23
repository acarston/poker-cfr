#include "Node.h"

/* normalize positive regrets and update strategy sum */
vector<double> Node::GetCurrentStrategy(double reachProbablilty) {
    vector<double> strategy(cumulativeRegrets.size(), 0.0); 

    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        strategy.at(i) = cumulativeRegrets.at(i) > 0 ? cumulativeRegrets.at(i) : 0.0;
    }
    strategy = NormalizeVector(strategy);
    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        sumOfStrategies.at(i) += strategy.at(i) * reachProbablilty;
    }

    return strategy;
};

vector<double> Node::GetAverageStrategy() {
    return NormalizeVector(sumOfStrategies);
};

vector<double> Node::NormalizeVector(vector<double> vector) {
    double vectorSum = 0.0;
    for (unsigned int i = 0; i < vector.size(); ++i) vectorSum += vector.at(i);

    if (vectorSum > 0) {
        for (unsigned int i = 0; i < vector.size(); ++i) vector.at(i) /= vectorSum;
    }
    else {  //default case (give an even distribution)
        for (unsigned int i = 0; i < vector.size(); ++i) vector.at(i) = 1.0 / NUM_ACTIONS;
    }

    return vector;
};