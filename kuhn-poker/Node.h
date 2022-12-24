#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;

class Node {
    public:
        vector<double> cumulativeRegrets = {0.0, 0.0};

        vector<double> GetCurrentStrategy(double reachProbability);
        vector<double> GetAverageStrategy(); 
    private:
        const int NUM_ACTIONS = cumulativeRegrets.size();
        vector<double> sumOfStrategies = {0.0, 0.0};

        vector<double> NormalizeVector(vector<double> vector);
};

#endif