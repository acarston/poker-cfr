#include "KuhnCfrBot.h"

double KuhnCfrBot::Cfr(vector<int> cards, string history = "", vector<double> reachProbabilities = {1.0, 1.0}) {
    int currentPlayer = (history.size() % 2 == 0) ? PLAYER_1 : PLAYER_2;

    int terminalHistoryStart = history.size() - 2;
    string terminalHistory = history.substr(terminalHistoryStart, TERMINAL_HISTORY_LENGTH);
};

bool KuhnCfrBot::IsTerminal(string history) {
};

int KuhnCfrBot::GetTerminalPayoff(vector<int> cards, string terminalHistory, int currentPlayer) {
    if (terminalHistory == "bp") return -1;

    int loser = (cards.at(PLAYER_1) < cards.at(PLAYER_2)) ? PLAYER_1 : PLAYER_2;
    bool isLoser = currentPlayer == loser;
    int nodeUtility = 0;

    if (terminalHistory == "bb") nodeUtility = 2;
    else if (terminalHistory == "pp") nodeUtility = 1;
    else exit(0); //bootleg error throw. For testing purposes; delete later

    if (isLoser) nodeUtility *= -1;

    return nodeUtility;
};

Node KuhnCfrBot::GetNode(string history) {
};

void KuhnCfrBot::UpdateNodeRegrets(vector<double>& cumulativeRegrets) {
};