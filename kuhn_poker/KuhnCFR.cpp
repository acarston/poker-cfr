#include "KuhnCFR.h"

/* pass forward reach probabilities and pass back game-state utilities; calculate and update regrets */
double KuhnCFR::CalculateUtilities(const unsigned int iteration, const vector<int>& cards, const string& history, const vector<double>& reachProbabilities) {
    double nodeUtility = 0.0;
    const int currentPlayer = (history.size() % 2 == 0) ? PLAYER_1 : PLAYER_2; 

    // return payoff for terminal nodes
    if (history.size() > 1) nodeUtility = GetTerminalUtility(history, cards, currentPlayer);
    if (nodeUtility != 0.0) return nodeUtility;

    string key = to_string(cards[currentPlayer]) + history;
    Node& currentNode = GetNode(key);

    // compute current game-state and counterfactual utilities for current and reachable nodes
    const vector<double>& nodeStrategy = currentNode.GetCurrentStrategy(iteration, reachProbabilities[currentPlayer]);
    vector<double> nextReachProbabilities(NUM_ACTIONS, 0.0), counterfactualUtilities(NUM_ACTIONS, 0.0);
    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        nextReachProbabilities = reachProbabilities;
        nextReachProbabilities[currentPlayer] *= nodeStrategy[i]; 
        counterfactualUtilities[i] = -CalculateUtilities(iteration, cards, (history + ACTIONS[i]), nextReachProbabilities);
        nodeUtility += counterfactualUtilities[i] * nodeStrategy[i];
    }

    // add positive regrets to node's cumulative regrets
    for (unsigned int i = 0; i < counterfactualUtilities.size(); ++i) {
        currentNode.cumulativeRegrets[i] += (counterfactualUtilities[i] - nodeUtility) * reachProbabilities[!currentPlayer] * (double(iteration) / (iteration + 1));
    }

    return nodeUtility;
};

double KuhnCFR::MCCFR(const int targetPlayer, const unsigned int iteration, const vector<int>& cards, const string& history) {
    double nodeUtility = 0.0;
    const int currentPlayer = (history.size() % 2 == 0) ? PLAYER_1 : PLAYER_2;
    const double iterWeight = double(iteration) / (iteration + 1000);

    // return payoff for terminal nodes
    if (history.size() > 1) nodeUtility = GetTerminalUtility(history, cards, currentPlayer);
    if (nodeUtility != 0.0) return nodeUtility;

    string key = to_string(cards[currentPlayer]) + history;
    Node& node = GetNode(key);
    const double *nodeStrategy = node.Strategy(iteration, iterWeight);

    // if current player is not the target player, sample a single action and recursively call cfr
    if (currentPlayer != targetPlayer) {
        std::discrete_distribution<int> dist(nodeStrategy, nodeStrategy + NUM_ACTIONS); 
        int action = dist(engine);

        return -MCCFR(targetPlayer, iteration, cards, (history + ACTIONS[action]));
    }

    vector<double> counterfactualUtilities(NUM_ACTIONS);
    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
        counterfactualUtilities[i] = -MCCFR(targetPlayer, iteration, cards, (history + ACTIONS[i]));
        nodeUtility += counterfactualUtilities[i] * nodeStrategy[i];
    }

    // add positive regrets to node's cumulative regrets
    for (unsigned int i = 0; i < counterfactualUtilities.size(); ++i) {
        node.cumulativeRegrets[i] += (counterfactualUtilities[i] - nodeUtility) * iterWeight;
    }

    return nodeUtility;
}

//double KuhnCFR::NewMCCFR(Game& game, const int targetPlayer, const unsigned int iteration, const vector<double>& reachProbs) {
//    double nodeUtility = 0.0;
//    const double iterWeight = double(iteration) / (iteration + 1000);
//    const int curPlayer = game.CurPlayer();
//
//    if (game.Done()) return game.Payoff();
//
//    Node& node = GetNode(game.Infoset());
//    const double *nodeStrat = node.Strategy(iterWeight, reachProbs[curPlayer]);
//
//    // if current player is not the target player, sample a single action and recursively call cfr
//    if (curPlayer != targetPlayer) {
//        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + NUM_ACTIONS);
//        const int iAction = game.Sample(dist);
//
//        game.Step(iAction);
//        return -NewMCCFR(game, targetPlayer, iteration, reachProbs);
//    }
//
//    vector<double> nextReachProbabilities(NUM_ACTIONS, 0.0), counterfactualUtilities(NUM_ACTIONS, 0.0);
//    for (unsigned int i = 0; i < NUM_ACTIONS; ++i) {
//        nextReachProbabilities = reachProbabilities;
//        nextReachProbabilities[currentPlayer] *= nodeStrategy[i];
//        counterfactualUtilities[i] = -MCCFR(targetPlayer, iteration, cards, (history + ACTIONS[i]), nextReachProbabilities);
//        nodeUtility += counterfactualUtilities[i] * nodeStrategy[i];
//    }
//
//    // add positive regrets to node's cumulative regrets
//    for (unsigned int i = 0; i < counterfactualUtilities.size(); ++i) {
//        currentNode.cumulativeRegrets[i] += (counterfactualUtilities[i] - nodeUtility) * reachProbabilities[!currentPlayer] * (double(iteration) / (iteration + 1));
//    }
//
//    return nodeUtility;
//}

/* return payoff for current player according to the rules of Kuhn poker */
double KuhnCFR::GetTerminalUtility(const string& history, const vector<int>& cards, const int currentPlayer) {
    int terminalHistoryStart = history.size() - TERMINAL_HISTORY_LENGTH;
    string terminalHistory = history.substr(terminalHistoryStart, TERMINAL_HISTORY_LENGTH);

    int nodeUtility = 0;
    if (terminalHistory == "bp") return nodeUtility = 1;
    else if (terminalHistory == "bb") nodeUtility = 2;
    else if (terminalHistory == "pp") nodeUtility = 1;

    int loser = (cards[PLAYER_1] < cards[PLAYER_2]) ? PLAYER_1 : PLAYER_2;
    if (currentPlayer == loser) nodeUtility *= -1;

    return nodeUtility;
};

/* create new key-value pair if necessary and return node */
Node& KuhnCFR::GetNode(const std::string& key) {
    auto it = nodes.find(key);

    if (it == nodes.end()) {
        Node node;
        nodes.insert({key, node});
        it = nodes.find(key);
    }

    return it->second;
};