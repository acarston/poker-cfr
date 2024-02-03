#include "Node2.h"

Node2::Node2(const std::string& history) : history(history) {
    this->numActions = 2; // to be decided here
    this->strat = new double[numActions];
    this->stratSum = new double[numActions];
    this->cumulRegrets = new double[numActions];
    this->avgStrat = new double[numActions];
    this->actions = new char[numActions] { 'b', 'p' }; // to be decided here
}

Node2::~Node2() {
    delete[] this->actions;
    delete[] this->cumulRegrets;
    delete[] this->strat;
    delete[] this->stratSum;
    delete[] this->avgStrat;
}

double* Node2::strategy(unsigned int iteration, double iterWeight) {
    for (int i = 0; i < numActions; ++i) strat[i] = (cumulRegrets[i] > 0) ? cumulRegrets[i] : 0.0;
    normalize(strat, numActions);
    for (unsigned int i = 0; i < numActions; ++i) {
        if (iteration > WARMUP) stratSum[i] += strat[i] * iterWeight;
    }
    return strat;
}

void Node2::update_sum(const int iteration, const double iterWeight) {
    for (unsigned int i = 0; i < numActions; ++i) {
        if (iteration > WARMUP) stratSum[i] += strat[i] * iterWeight;
    }
}

double* Node2::avg_strategy() {
    for (int i = 0; i < numActions; ++i) avgStrat[i] = stratSum[i];
    normalize(avgStrat, numActions);
    return avgStrat;
}

void Node2::normalize(double* arr, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) sum += arr[i];
    if (sum > 0) {
        for (int i = 0; i < n; ++i) arr[i] /= sum;
    }
    else {
        for (int i = 0; i < n; ++i) arr[i] = 1.0 / n;
    }
}
