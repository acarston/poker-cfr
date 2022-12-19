#include <iostream>
#include <vector>
#include <stdlib.h>
#include <random>
using namespace std;

class Cfr {
    private:
        //returns a sum of all items or all positive items in a vector
        double sumVector(vector<double> vector, bool onlyPositive) {
            if (onlyPositive) {
                for (int i = 0; i < vector.size(); i ++) {
                    if (vector[i] < 0) vector[i] = 0.0;
                }
            }

            double sum = 0.0;
            for (double num : vector) sum += num;
            return sum;
        }

    public:
        //avoid putting all weight into one choice to prevent NaN errors on first iteration
        vector<double> weights = {0.9, 0.1, 0.0};

        vector<double> getWeights() {
            return weights;
        }

        vector<double> setWeights(vector<double> vect) {
            weights = vect;
            return vect;
        }

        //returns how 'regretful' computer is for not choosing each option
        vector<double> getRegrets(string oppChoice, string myChoice) {
            vector<int> counterfactualRewards;
            vector<double> regrets = {0,0,0};

            //conterfactual rewards for each choice, where the rewards are rock, paper, scissors from left to right
            if (oppChoice == "rock") counterfactualRewards = {0,1,-1};
            else if (oppChoice == "paper") counterfactualRewards = {-1,0,1};
            else if (oppChoice == "scissors") counterfactualRewards = {1,-1,0};

            double realReward = 0.0;
            for (int i = 0; i < weights.size(); i ++) {
                realReward += counterfactualRewards[i] * weights[i];
            }
        
            for (int i = 0; i < regrets.size(); i ++) {
                regrets[i] = counterfactualRewards[i] - realReward;
            }

            return regrets;
        }

        //returns a choice of rock, paper, or scissors based on their respective likelihood of being chosen (weights)
        //choices can be ints instead of strings for more efficiency
        string getChoice(vector<double> weights) {
            //creates a random number in set range
            int RANGE_MIN = 0;
            int RANGE_MAX = 2147483647;
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(RANGE_MIN, RANGE_MAX);
            
            int randNum = dist6(rng);

            //uses weighted probabilities to make a choice from randNum; more accurate with larger values of RANGE_MAX
            if (randNum <= weights[0] * RANGE_MAX) return "rock";
            else if (randNum <= (weights[0] + weights[1]) * RANGE_MAX) return "paper";
            else return "scissors";
        }

        //calculates weighted probabilities based on regrets
        vector<double> calculateWeights(vector<double> regrets) {
            vector<double> weights = {0,0,0};

            //sums positive regrets for subsequent calculations
            double totalRegrets = sumVector(regrets, true);

            //avoiding div by zero errors
            if (totalRegrets == 0) weights = {1.0, 0, 0};

            for (int i = 0; i < regrets.size(); i ++) {
                weights[i] = (regrets[i] / totalRegrets);
                if (weights[i] < 0) weights[i] = 0;
            }
            
            // cout << "weights: ";
            // for (double weight : weights) cout << weight << " ";
            // cout << endl;

            return weights;
        }

        vector<vector<double>> strategySum {{0.0,0.0,0.0},
                                            {0.0,0.0,0.0}};
        void updateStrategySum(vector<vector<double>> strategy) {
            for (unsigned int i = 0; i < strategy.size(); i ++) {
                for (unsigned int j = 0; j < strategy.at(0).size(); j ++) {
                    strategySum.at(i).at(j) += strategy.at(i).at(j);
                }
            }
        }

        vector<vector<double>> getAverageStrategy(int iterations) {
            for (unsigned int i = 0; i < strategySum.size(); i ++) {
                for (unsigned int j = 0; j < strategySum.at(0).size(); j ++) {
                    strategySum.at(i).at(j) /= iterations;
                }
            }
            return strategySum;
        }

        vector<double> totalRegrets = {0.0,0.0,0.0};
        vector<double> updateRegrets(vector<double> roundRegrets) {
            for (int i = 0; i < totalRegrets.size(); i ++) {
                // necessary because weights are calculated using positive regrets only
                if (roundRegrets[i] < 0) roundRegrets[i] = 0;
                totalRegrets[i] += roundRegrets[i];
            }
            return totalRegrets;
        }
};

class Play {
    public:
        vector<string> choices = {"",""};
        vector<vector<double>> totalRegrets {{0.0,0.0,0.0},{0.0,0.0,0.0}};
        vector<vector<double>> newWeights {{0.0,0.0,0.0},{0.0,0.0,0.0}};
        vector<vector<double>> averageStrategy {{0.0,0.0,0.0},{0.0,0.0,0.0}};
        Cfr p0;

        //plays two Cfr bots against each other to reach a nash equilibrium
        void playGame(Cfr p1, Cfr p2, int NUM_ITERATIONS) {
            for (int i = 0; i < NUM_ITERATIONS; i ++) {
                choices[0] = p1.getChoice(p1.getWeights());
                choices[1] = p2.getChoice(p2.getWeights());

                // for (string choice : choices) cout << choice << endl;
                // cout << endl;

                totalRegrets[0] = p1.updateRegrets(p1.getRegrets(choices[0], choices[1]));
                totalRegrets[1] = p2.updateRegrets(p2.getRegrets(choices[1], choices[0]));

                newWeights = {p1.calculateWeights(totalRegrets[0]), p2.calculateWeights(totalRegrets[1])};
                p1.setWeights(newWeights[0]);
                p2.setWeights(newWeights[1]);
                p0.updateStrategySum(newWeights);

                // cout << "\n\n";
            }
            averageStrategy = p0.getAverageStrategy(NUM_ITERATIONS);
            //print statements
            vector<double> weights1 = p1.getWeights();
            vector<double> weights2 = p2.getWeights();
            cout << endl;
            cout << "(after " << NUM_ITERATIONS << " iterations)" << endl;
            cout << "player 1 strategy: ";
            for (double weight : averageStrategy.at(0)) cout << weight << " ";
            cout << endl << "player 2 strategy: ";
            for (double weight : averageStrategy.at(1)) cout << weight << " ";
            cout << endl << endl;
        }
};

int main() {
    string playerChoice, compChoice;
    vector<double> roundRegrets, gameRegrets = {0.0,0.0,0.0};
    int NUM_ITERATIONS = 1000000;
    Cfr p1, p2;

    Cfr player1, player2;
    Play game;
    game.playGame(player1, player2, NUM_ITERATIONS);
};

/* steps:
1) first-iteration weights are set
2) getChoice returns a random weighted choice based on the current weights
3) getRegrets finds real and conterfactual rewards, then calculates regrets using r = cr - rr
4) updateRegrets adds positive regrets to the total regrets
5) calculateWeights returns new weights using the total regrets
6) steps 2-5 are repeated for each player 

class Play puts two bots against each other to reach a nash equilibrium */