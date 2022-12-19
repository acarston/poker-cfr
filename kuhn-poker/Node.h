#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;

class Node {
    public:
        const int NUM_ACTIONS = 2;
        vector<double> cumulativeRegrets = {0.0, 0.0};

        vector<double> GetCurrentStrategy(double reachProbability);
        vector<double> GetAverageStrategy(); 
    private:
        vector<double> sumOfStrategies = {0.0, 0.0};

        vector<double> NormalizeVector(vector<double> vector);
};

#endif