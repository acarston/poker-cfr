#include "KuhnCfrBot.h"

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
};

Node KuhnCfrBot::GetNode(string history) {
};

void KuhnCfrBot::UpdateNodeRegrets(vector<double>& cumulativeRegrets) {
};