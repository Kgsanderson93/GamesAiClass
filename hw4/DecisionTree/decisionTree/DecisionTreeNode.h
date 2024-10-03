//Represents a decision Tree node. 
#pragma once
#include <vector>
#include "../utilities/util.h"

/**
     * A decision tree node is a base class for anything that makes a
     * decision.
     */
    class DecisionTreeNode
    {
    public:
        /**
         * The make decision method carries out a decision making
         * process and returns the new decision tree node that we've
         * reached in the tree.
         */
        std::string name;
        virtual DecisionTreeNode* makeDecision() = 0;
    };

    /**
     * An action is a decision tree node at the end of the tree. It
     * simply returns itself as the result of the next decision.
     */
    class DecisionTreeAction : public DecisionTreeNode
    {
    public:
        /**
         * Makes the decision - in  this case there is no decision, so
         * this method returns the action back again..
         */
        virtual DecisionTreeNode* makeDecision()
        {
            return this;
        }
    };

    /**
     * Other than actions, the tree is made up of decisions, which
     * come up with some boolean result and choose a branch based on
     * that.
     */
    class Decision : public DecisionTreeNode
    {
    public:
        DecisionTreeNode* trueBranch;
        DecisionTreeNode* falseBranch;

        /**
         * This method actually does the checking for the decision.
         */
        virtual bool getBranch() = 0;

        /**
         * This is where the decision tree algorithm is located: it
         * recursively walks down the tree until it reaches the final
         * item to return (which is an action).
         */
        virtual DecisionTreeNode* makeDecision();
    };

    /**
     * This class represents a stable random decision. As long as the
     * decision is reached at each frame, the decision will be made
     * the same way each time. Otherwise the decision will be made at
     * random.
     */
    class RandomDecision : public Decision
    {
    public:
        /**
         * Holds the last decision that was made.
         */
        bool lastDecision;

        /** Creates a new random decision. */
        RandomDecision();

        /**
         * Works out which branch to follow.
         */
        virtual bool getBranch();
    };