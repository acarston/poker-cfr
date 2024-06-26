#include "Node.h"

Node::Node() {
    this->numActions = 2;
    this->strat = new double[this->numActions];
    this->stratSum = new double[this->numActions];
    this->cumulRegrets = new double[this->numActions];
    this->avgStrat = new double[this->numActions];

    for (int i = 0; i < this->numActions; ++i) {
        this->strat[i] = 0.0;
        this->stratSum[i] = 0.0;
        this->cumulRegrets[i] = 0.0;
        this->avgStrat[i] = 0.0;
    }
}

Node::~Node() {
    delete[] this->cumulRegrets;
    delete[] this->strat;
    delete[] this->stratSum;
    delete[] this->avgStrat;
}

double* Node::strategy() {
    for (int i = 0; i < numActions; ++i) strat[i] = (cumulRegrets[i] > 0) ? cumulRegrets[i] : 0.0;
    normalize(strat, numActions);
    return strat;
}

void Node::update_sum(const unsigned int iteration, const double iterWeight) {
    for (int i = 0; i < numActions; ++i) {
        if (iteration > WARMUP) stratSum[i] += strat[i] * iterWeight;
    }
}

double* Node::avg_strategy() {
    for (int i = 0; i < numActions; ++i) avgStrat[i] = stratSum[i];
    normalize(avgStrat, numActions);
    return avgStrat;
}

void Node::normalize(double* arr, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) sum += arr[i];
    if (sum > 0) {
        for (int i = 0; i < n; ++i) arr[i] /= sum;
    }
    else {
        for (int i = 0; i < n; ++i) arr[i] = 1.0 / n;
    }
}
