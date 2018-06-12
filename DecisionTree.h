
#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <map>
#include <list>
#include <vector>
#include <queue>

class DecisionTree {
    
struct node {
    int attr;
    node * positiveBranch;
    node * negativeBranch;
    bool classification;
};

public:
    node * getRootNode();
    bool evaluate(bool * bools);
    void printTree(node *, int depth);
    DecisionTree(std::list< std::map<int, bool> > sampleList);
private:
    int rec[1000006];
    node * learnDecisionTree(std::list< std::map<int, bool> > sampleList,
            std::list< std::map<int, bool> > parentList);
    node * plurality(std::list< std::map<int, bool> > sampleList);
    bool listIsHomogeneous(std::list< std::map<int, bool> > sampleList);
    int max(std::map<int, double> importanceVals);
    double entropy(double zeroProb, double oneProb);
    double remainder(int attr, std::list< std::map<int, bool> > sampleList);
    std::map<int, double> infoGainVals(std::list< std::map<int, bool> > sampleList);
    node * root;
};

#endif /* DECISIONTREE_H */

