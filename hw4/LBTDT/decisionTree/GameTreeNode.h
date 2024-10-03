#pragma once
#include "DecisionTreeNode.h"
#include "../behavior/steeringbehavior.h"
#include "TreeFunctions.h"

class GameTreeDecision: public Decision
{
    public:
        Kinematic *cur;
        sf::Vector2f pos;
        GameWorld *game;
        TreeDecision *decisionMaker;
        virtual bool getBranch();


};
class GameTreeAction: public DecisionTreeAction{
    public:
        Kinematic *cur;
        Kinematic *goal;
        sf::Vector2f truegoal;
        SteeringBehavior *action;
        SteeringData getAction();
};
