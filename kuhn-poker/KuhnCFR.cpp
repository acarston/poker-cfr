#include "KuhnCFR.h"

double KuhnCFR::CalculateUtilities(vector<int> cards, string history, vector<double> reachProbabilities) {
    currentPlayer = (history.size() % 2 == 0) ? PLAYER_1 : PLAYER_2;

    if (history.size() > 1) nodeUtility = GetTerminalUtility(history, cards);
    if (nodeUtility != 0.0) return nodeUtility;

    string key = to_string(cards.at(currentPlayer)) + history;
    Node& currentNode = GetNode(key);

    vector<double> nextReachProbabilities = reachProbabilities;
    vector<double> counterfactualUtilities(NUM_ACTIONS, 0.0);
    vector<double> nodeStrategy = currentNode.GetCurrentStrategy(reachProbabilities.at(currentPlayer));

    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        nextReachProbabilities.at(currentPlayer) = reachProbabilities.at(currentPlayer) * nodeStrategy.at(i); 
        counterfactualUtilities.at(i) = -CalculateUtilities(cards, (history + ACTIONS.at(i)), nextReachProbabilities);
        nodeUtility += counterfactualUtilities.at(i) * nextReachProbabilities.at(currentPlayer);
    }

    int opponent = !bool(currentPlayer);
    for (unsigned int i = 0; i < counterfactualUtilities.size(); ++i) {
        currentNode.cumulativeRegrets.at(i) += counterfactualUtilities.at(i) - nodeUtility;
        currentNode.cumulativeRegrets.at(i) *= reachProbabilities.at(opponent);
    }

    return nodeUtility;
};

double KuhnCFR::GetTerminalUtility(string history, vector<int> cards) {
    int terminalHistoryStart = history.size() - TERMINAL_HISTORY_LENGTH;
    string terminalHistory = history.substr(terminalHistoryStart, TERMINAL_HISTORY_LENGTH);

    if (terminalHistory == "bp") return nodeUtility + 1.0;
    else if (terminalHistory == "bb") nodeUtility = 2.0;
    else if (terminalHistory == "pp") nodeUtility = 1.0;

    int loser = (cards.at(PLAYER_1) < cards.at(PLAYER_2)) ? PLAYER_1 : PLAYER_2;
    bool isLoser = currentPlayer == loser;
    if (isLoser) nodeUtility *= -1.0;

    return nodeUtility;
};

Node& KuhnCFR::GetNode(string key) {
    unordered_map<string, Node>::iterator it = nodes.find(key);

    if (it == nodes.end()) {
        Node node;
        nodes.insert({key, node});
        it = nodes.find(key);
    }
    return it->second;
};