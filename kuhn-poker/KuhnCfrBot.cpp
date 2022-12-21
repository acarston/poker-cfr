#include "KuhnCfrBot.h"

//FIXME: cards do not need to be a parameter due to chance sampling
double KuhnCfrBot::Cfr(vector<int> cards, string history, vector<double> reachProbabilities) {
    double nodeUtility = 0.0;
    int currentPlayer = (history.size() % 2 == 0) ? PLAYER_1 : PLAYER_2;

    int terminalHistoryStart = history.size() - 2;
    string terminalHistory = history.substr(terminalHistoryStart, TERMINAL_HISTORY_LENGTH);

    if (terminalHistory == "bp") return nodeUtility - 1.0;
    else if (terminalHistory == "bb") nodeUtility = 2.0;
    else if (terminalHistory == "pp") nodeUtility = 1.0;
    if (nodeUtility != 0.0) {
        int loser = (cards.at(PLAYER_1) < cards.at(PLAYER_2)) ? PLAYER_1 : PLAYER_2;
        bool isLoser = currentPlayer == loser;
        if (isLoser) nodeUtility *= -1.0;

        return nodeUtility;
    }
    /* Get information set node or create it if nonexistant
    For each action, recursively call cfr with additional history and probability
    For each action, compute and accumulate counterfactual regret */
    string key = to_string(cards.at(currentPlayer)) + history;
    Node currentNode = GetNode(key);

    vector<double> counterfactualUtilities(ACTIONS.size(), 0.0);
    vector<double> nodeStrategy = currentNode.GetCurrentStrategy(reachProbabilities.at(currentPlayer));

    for (unsigned int i = 0; i < ACTIONS.size(); ++i) {
        history += ACTIONS.at(i);
        reachProbabilities.at(currentPlayer) *= nodeStrategy.at(i);
        counterfactualUtilities.at(i) = -Cfr(cards, history, reachProbabilities);
    }

    for (double utility : counterfactualUtilities) nodeUtility += utility;
    for (unsigned int i = 0; i < counterfactualUtilities.size(); ++i) {
        currentNode.cumulativeRegrets.at(i) += counterfactualUtilities.at(i) - nodeUtility;
    }

    return nodeUtility;
};

Node KuhnCfrBot::GetNode(string key) {
    unordered_map<string, Node>::iterator it = nodes.find(key);

    if (it != nodes.end()) return it->second;
    Node node;
    nodes.insert({key, node});
    return node;
};

void KuhnCfrBot::UpdateNodeRegrets(vector<double>& cumulativeRegrets) {
};