#include "GameTreeNode.h"

bool GameTreeDecision::getBranch(){
    return this->decisionMaker->makeDecision(this->cur, this->game);
}
SteeringData GameTreeAction::getAction(){
    std::cout<<this->name<<"\n";
    return this->action->calculateAcceleration(this->cur, this->goal);
}