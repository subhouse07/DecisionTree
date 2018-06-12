
#include "DecisionTree.h"
#include <list>
#include <map>
#include <math.h>
#include <cmath>
#include <queue>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

DecisionTree::DecisionTree(std::list< std::map<int, bool> > sampleList) {
    root = new node;
    root = learnDecisionTree(sampleList, sampleList);
}

DecisionTree::node * DecisionTree::getRootNode() {
    return root;
}

DecisionTree::node * DecisionTree::learnDecisionTree(
        std::list< std::map<int, bool> > sampleList,
        std::list< std::map<int, bool> > parentList) {
    
    //Check for empty list 
    if (sampleList.empty()) {
        return plurality(parentList);
    
    //Check for list where all elements have the same classification
    } else if (listIsHomogeneous(sampleList)) {
        node * n = new node;
        n->attr = 0;
        n->negativeBranch = nullptr;
        n->positiveBranch = nullptr;
        n->classification = sampleList.front().at(0);
        return n;
        
    //Check for data with no attributes
    } else if (sampleList.front().size() == 1) {
        return plurality(parentList);
        
    } else {
        
        //Find attribute with max info gain and create new tree with this attr
        //at the root.
        int attr = max(infoGainVals(sampleList));
        node * tree = new node;
        tree->attr = attr;
        tree->negativeBranch = nullptr;
        tree->positiveBranch = nullptr;
        
        //Iterate through data set, splitting data on the new tree's root attr
        //Erase the map entry with this attribute as each set of values is added
        std::list< std::map<int, bool> > negBranchList;
        std::list< std::map<int, bool> > posBranchList;
        for (std::map<int, bool> s : sampleList) {
            if (s.at(attr)) {
                posBranchList.push_back(s);
                posBranchList.back().erase(attr);
            } else {
                negBranchList.push_back(s);
                negBranchList.back().erase(attr);
            }
        }
        
        //Create pointers to positive and negative subtrees
        //Call the decision tree learning algorithm for each.
        //Then return this tree's root node.
        node * negSubTree = new node;
        negSubTree = learnDecisionTree(negBranchList, sampleList);
        node * posSubTree = new node;
        posSubTree = learnDecisionTree(posBranchList, sampleList);
        tree->negativeBranch = negSubTree;
        tree->positiveBranch = posSubTree;
        
        return tree;
    }
}

//In the event of a sample set with no attributes or empty data set, return
//a node with a classification indicating the more common among the data set
DecisionTree::node * DecisionTree::plurality(
        std::list< std::map<int, bool> > sampleList) {
    
    int onesCount = 0,
        zerosCount = 0;
    node * n = new node;
    n->attr = 0;
    n->negativeBranch = nullptr;
    n->positiveBranch = nullptr;
    
    for (std::map<int, bool> s : sampleList) {
        if (s.at(0))
            onesCount++;
        else
            zerosCount++;
    }
    
    if (onesCount > zerosCount)
        n->classification = true;
    else
        n->classification = false;
    
    return n;
}

bool DecisionTree::listIsHomogeneous(
        std::list< std::map<int, bool> > sampleList) {
    
    bool classification = sampleList.front().at(0);
    for (std::map<int, bool> s : sampleList) {
        if (s.at(0) != classification)
            return false;
    }
    return true;
}

std::map<int, double> DecisionTree::infoGainVals(
        std::list<std::map<int,bool> > sampleList) {
    
    std::map<int, double> igVals;
        
    //Calculate probabilities of each value for each attribute  for each sample
    for (const auto& pair : sampleList.front()) {
        if (pair.first != 0) {
            int onesCount = 0;
            int zerosCount = 0;
            for (std::map<int, bool> s : sampleList) {
                if (s.at(pair.first))
                    onesCount++;
                else
                    zerosCount++;
            }
            
            //Probability values
            double zeroProb = zerosCount / (double)sampleList.size();
            double oneProb = onesCount / (double)sampleList.size();
            
            //Calculate entropy of this attribute
            double attrEntropy = entropy(zeroProb, oneProb);
            
            //Information gain = entropy - remainder
            igVals[pair.first] = entropy(zeroProb, oneProb) -
                    remainder(pair.first, sampleList);
        }
    }
    
    return igVals;   
}

//Entropy calculation. Returns zero in the event of an infinite result
double DecisionTree::entropy(double zeroProb, double oneProb) {

    double entropy = (-1*( (zeroProb * log2 (zeroProb)) +
                     (oneProb * log2 (oneProb)) ));
    if (isnan(entropy))
        return 0;
    else
        return entropy;
}

//Entropy remainder calculation
double DecisionTree::remainder(int attr,
        std::list< std::map<int, bool> > sampleList) {
    
    int posOnes = 0,        //attr value is positive, classifcation is positive
        posZeros = 0,       //attr value is positive, classification is negative
        negOnes = 0,        //attr value is negative, classification is positive
        negZeros = 0;       //attr value is negative, classification is negative
    
    double posOneProb = 0,  //prob. that classification is 1 in positive subset
           posZeroProb = 0, //prob. that classification is 0 in positive subset
           negOneProb = 0,  //prob. that classification is 1 in negative subset
           negZeroProb = 0; //prob. that classification is 0 in negative subset
    
    for (std::map<int, bool> s : sampleList) {
        if (s.at(attr) && s.at(0))
            posOnes++;
        if (s.at(attr) && !s.at(0))
            posZeros++;
        if (!s.at(attr) && s.at(0))
            negOnes++;
        if (!s.at(attr) && !s.at(0))
            negZeros++;
    }
    
    posOneProb = (posOnes) / (double)(posOnes + posZeros);
    posZeroProb = (posZeros) / (double)(posOnes + posZeros);
    negOneProb = (negOnes) / (double)(negOnes + negZeros);
    negZeroProb = (negZeros) / (double)(negOnes + negZeros);
    
    //Probability terms for remainder calculation
    double p1 = (posOnes + posZeros) / (double)sampleList.size();
    double p2 = (negOnes + negZeros) / (double)sampleList.size();
    
    //Entropy terms for remainder calculation
    double entPos = entropy(posZeroProb, posOneProb);
    double entNeg = entropy(negZeroProb, negOneProb);
    
    double remainder;
    remainder = (p1 * entPos) + (p2 * entNeg);
    
    return remainder;
            
}

//Returns attribute with maximum information gain.
//In the event of a tie, returns whichever attribute it checked first
int DecisionTree::max(std::map<int, double> igVals) {
    
    int maxKey = igVals.begin()->first;
    double maxVal = igVals.begin()->second;
    
    for (const auto& pair : igVals) {
        if (pair.second > maxVal) {
            maxVal = pair.second;
            maxKey = pair.first;
        }
    }
    
    return maxKey;
}

//Decision tree display function
//Taken from stackoverflow user Sarthak Manna
//URL: https://stackoverflow.com/a/47400572
void DecisionTree::printTree(DecisionTree::node * curr, int depth) {
    
    if(curr==NULL)
        return;
    printf("\t");
    
    for(int i=0; i<depth; i++) {
        if(i == depth-1)
            printf("%s\u2014\u2014\u2014", rec[depth-1] ? "\u0371" : "\u221F");
        else
            printf("%s   ", rec[i] ? "\u23B8" : "  ");
    }
    
    if (curr->attr > 0)
        printf("a%d\n", curr->attr);
    else
        printf("%d\n", curr->classification);
    
    rec[depth] = 1;
    printTree(curr->negativeBranch, depth+1);
    rec[depth]=0;
    printTree(curr->positiveBranch, depth+1);
}

//User input evaluation function
bool DecisionTree::evaluate(bool * bools) {
    node n = *root;
    while (n.attr != 0) {
        if (bools[n.attr-1] == 0) {
            n = *n.negativeBranch;
        } else {
            n = *n.positiveBranch;
        }
    }
    return n.classification;
}