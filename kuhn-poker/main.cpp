#include "Trainer.cpp"

int main() {
    // KuhnCFR bot;
    // Node node;
    // bot.nodes.insert({"2pb", node});
    // node.cumulativeRegrets = {0,200};
    // bot.CalculateUtilities({2,1}, "pb");

    //TODO: function that permutates all chance nodes, explainer branch

    //Deals that work: {2, 1}, {2, 0}, 
    //on {1, 2} p0 always acts as if he will lose, ie p1 will never have jack. I guess this makes
    //sense when isolating this deal; because with {2, x} or {0, x} p1's card does not matter.
    //It's like he knows the game state instead of the information set
    //TODO: test {0, x}, {1, 0} to confirm theory

    Trainer trainer;
    int numIterations = 100000;

    trainer.Train(numIterations);
    trainer.DisplayNodeStrategies(numIterations);

    return 0;
};
/* 
For key 2pb:

strategy is always 0.5 0.5
if you run cfr specifically for info set 2pb it works as expected (1.0 0.0)
it is ONLY REACHED THE FIRST TIME because reach probability for that node becomes 0 on the next iteration
    why? maybe because negative cumulative regrets count

Ideas:
cross-check code with others
something is effed with the hashmap

shuffling does not work uniformly, but probably not related
*/