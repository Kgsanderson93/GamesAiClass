
#pragma once
#include "Observation.h"

class LearnedTree:public BehaviorTree{
	public:
	LearnedTree(LearningTree *t, GameWorld *game, Kinematic *mob, Kinematic *goal, Kinematic *player);
	Behavior *addBaby( LearningTreeDecisionNode *n);
	Behavior *switcher(std::string);
	int countHit;
};