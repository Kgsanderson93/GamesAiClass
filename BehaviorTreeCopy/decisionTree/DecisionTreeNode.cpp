#include "DecisionTreeNode.h"

#include "DecisionTreeNode.h"


 DecisionTreeNode* Decision::makeDecision()
    {
        //std::cout<<this->name;
        // Choose a branch based on the getBranch method
        if (getBranch()) {
            // Make sure its not null before recursing.
            if (trueBranch == NULL) return NULL;
            else return trueBranch->makeDecision();
        } else {
            // Make sure its not null before recursing.
            if (falseBranch == NULL) return NULL;
            else return falseBranch->makeDecision();
        }
    }

    RandomDecision::RandomDecision()
        :
        lastDecision(false)
    {
    }

    bool RandomDecision::getBranch()
    {

        int i= randomBinomial();
        if(i==0){
            lastDecision=false;
        }else{
            lastDecision=true;
        }

        // And return the stored value
        return lastDecision;
    }

