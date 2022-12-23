#include "Node.cpp"
#include "Trainer.cpp"
#include "KuhnCFR.cpp"

#include <assert.h>
#include <iostream>

void testCfr() {
    KuhnCFR bot;
    // Test 1: Check that the function returns the correct utility for a terminal state
    vector<int> cards1 = {1, 0};
    string history1 = "pp";
    vector<double> reachProbabilities1 = {1.0, 1.0};
    double expectedUtility1 = 1;
    double actualUtility1 = bot.CalculateUtilities(cards1, history1, reachProbabilities1);
    assert(expectedUtility1 == actualUtility1);

    // Test 2: Check that the function returns the correct utility for a non-terminal state
    vector<int> cards2 = {1, 2};
    string history2 = "pb";
    vector<double> reachProbabilities2 = {1.0, 1.0};
    double expectedUtility2 = -1.5;
    double actualUtility2 = bot.CalculateUtilities(cards2, history2, reachProbabilities2);
    assert(expectedUtility2 == actualUtility2);

    // Test 3: Check that the cumulative regrets match their expected values
    string key3 = to_string(cards2.at(0)) + history2;
    Node node3 = bot.GetNode(key3);
    vector<double> expectedRegrets3 = {-0.5, 0.5};
    vector<double> actualRegrets3 = node3.cumulativeRegrets;
    assert(expectedRegrets3 == actualRegrets3);
};


int main() {
    testCfr();
    return 0;
};