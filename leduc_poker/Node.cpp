#include "Node.h"

Node::Node(const int lastActions, const int lastAction, const int passedStreets) {
    this->passedStreets = passedStreets;

    const int firstAction = lastActions >> 3;
    if (lastActions == 0 || lastAction == 0b001 || lastAction == 0b010) {
        this->numActions = 2;
        this->actions = new int[numActions] { 0b001, 0b011 };
    }
    else if (lastAction == 0b011) {
        if (firstAction == 0 || firstAction == 0b001) {
            this->numActions = 3;
            this->actions = new int[numActions] { 0b100, 0b010, 0b011 };
        }
        else if (firstAction == 0b011) {
            this->numActions = 2;
            this->actions = new int[numActions] { 0b100, 0b010 };
        }
    }

    this->strat = new double[numActions];
    this->stratSum = new double[numActions];
    this->cumulRegrets = new double[numActions];
    this->avgStrat = new double[numActions];
}

Node::~Node() {
    delete[] this->actions;
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
