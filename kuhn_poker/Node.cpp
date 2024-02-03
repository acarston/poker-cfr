#include "Node.h"

/* normalize positive regrets and update strategy sum */
vector<double>& Node::GetCurrentStrategy(const unsigned int iteration, double reachProbablilty) {
    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        strategy[i] = (cumulativeRegrets[i] > 0) ? cumulativeRegrets[i] : 0.0;
    }
    NormalizeVector(strategy);
    for (unsigned int i = 0; i < NUM_ACTIONS; ++i){
        sumOfStrategies[i] += strategy[i] * reachProbablilty * (double(iteration) / (iteration + 1));
    }
    return strategy;
};

double *Node::Strategy(const int iter, const double iterWeight) {
    for (int i = 0; i < NUM_ACTIONS; ++i) {
        strategyArr[i] = (cumulativeRegrets[i] > 0) ? cumulativeRegrets[i] : 0.0;
    }

    double sum = 0.0;
    for (int i = 0; i < NUM_ACTIONS; ++i) sum += strategyArr[i];
    if (sum > 0) {
        for (int i = 0; i < NUM_ACTIONS; ++i) strategyArr[i] /= sum;
    }
    else {
        for (unsigned int i = 0; i < NUM_ACTIONS; ++i) strategyArr[i] = 1.0 / NUM_ACTIONS;
    }

    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        if (iter > WARMUP) sumOfStrategies[i] += strategyArr[i] * iterWeight;
    }
    return strategyArr;
};

vector<double> Node::GetAverageStrategy() {
    vector<double> averageStrategy = sumOfStrategies;
    NormalizeVector(averageStrategy);
    return averageStrategy;
};

void Node::NormalizeVector(vector<double>& v) {
    double vSum = 0.0;
    for (unsigned int i = 0; i < v.size(); ++i) vSum += v[i];

    if (vSum > 0) {
        for (unsigned int i = 0; i < v.size(); ++i) v[i] /= vSum;
        return;
    }
    // give an even distribution for a vector of zeros
    for (unsigned int i = 0; i < v.size(); ++i) v[i] = 1.0 / NUM_ACTIONS;
};