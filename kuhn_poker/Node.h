#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;

class Node {
    public:
        vector<double> cumulativeRegrets = {0.0, 0.0};

        // ~Node() { delete[] strategyArr; };
        vector<double>& GetCurrentStrategy(const unsigned int iteration, double reachProbability);
        double *Strategy(const int iter, const double iterWeight);
        vector<double> GetAverageStrategy(); 
    private:
        const int NUM_ACTIONS = cumulativeRegrets.size();
        const int WARMUP = 100000;
        vector<double> sumOfStrategies = {0.0, 0.0};
        vector<double> strategy = { 0.0, 0.0 };
        double *strategyArr = new double[NUM_ACTIONS];

        void NormalizeVector(vector<double>& vector);
};

#endif