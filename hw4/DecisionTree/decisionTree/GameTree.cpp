#include "GameTree.h"
#include "TreeFunctions.h"
#include "../behavior/steeringbehavior.h"
GameTree::GameTree(Kinematic *cur, GameWorld *game)
{
    printf("Making tree\n");
    this->actions = std::vector<GameTreeAction *>();
    this->decisions = std::vector<GameTreeDecision *>();
    this->decisions.emplace_back(new GameTreeDecision());
    this->root = this->decisions.at(0);
    this->root->decisionMaker = new ActionComplete();
    this->root->name=std::string("Action Complete");
    this->root->cur = cur;
    this->root->game = game;
     printf("Making tree 1\n");
    // make false branch last action so it continues;
    this->root->falseBranch = lastAction;

    // make true branch decision if done eating
    this->decisions.emplace_back(new GameTreeDecision());
    this->decisions.at(1)->decisionMaker = new DoneEating();
    this->decisions.at(1)->name=std::string("Done eating");
    this->decisions.at(1)->cur = cur;
    this->decisions.at(1)->game = game;
    this->root->trueBranch = this->decisions.at(1);
     printf("Making tree 2\n");

    // make true branch of is done eating dance action
    this->actions.emplace_back(new GameTreeAction());
    this->actions.at(0)->action = new Dance();
    this->actions.at(0)->name=std::string("Dance");
    this->actions.at(0)->cur = cur;
    this->actions.at(0)->goal = cur;
    this->decisions.at(1)->trueBranch = this->actions.at(0);
     printf("Making tree 3\n");

    // make false branch of done eating is there fruit?
    this->decisions.emplace_back(new GameTreeDecision());
    this->decisions.at(2)->decisionMaker = new FruitExist();
    this->decisions.at(2)->name=std::string("Fruit Exists");
    this->decisions.at(2)->cur = cur;
    this->decisions.at(2)->game = game;
    this->decisions.at(1)->falseBranch = this->decisions.at(2);
     printf("Making tree 4\n");

    // true branch of is there fruit->can i see it?
    this->decisions.emplace_back(new GameTreeDecision());
    this->decisions.at(3)->decisionMaker = new FruitVis();
    this->decisions.at(3)->name=std::string("Fruit Vis");
    this->decisions.at(3)->cur = cur;
    this->decisions.at(3)->game = game;
    this->decisions.at(2)->trueBranch = this->decisions.at(3);
     printf("Making tree 5\n");

    // false branch of is there fruit? -> in corner
    this->decisions.emplace_back(new GameTreeDecision());
    this->decisions.at(4)->decisionMaker = new InCorner();
    this->decisions.at(4)->name=std::string("In Corner");
    this->decisions.at(4)->cur = cur;
    this->decisions.at(4)->game = game;
    this->decisions.at(2)->falseBranch = this->decisions.at(4);
     printf("Making tree 6\n");

    // true branch of in corner
    this->actions.emplace_back(new GameTreeAction());
    this->actions.at(1)->action = new Seek();
    this->actions.at(1)->name=std::string("Seek");
    this->actions.at(1)->cur = cur;
    this->actions.at(1)->goal = new Kinematic(640, 640, -1);
    this->actions.at(1)->truegoal=sf::Vector2f(340, 340);
    this->actions.at(1)->goal->_position = sf::Vector2f(340, 340);
    this->decisions.at(4)->trueBranch = this->actions.at(1);
     printf("Making tree 7\n");

    //false branch in corner
    this->actions.emplace_back(new GameTreeAction());
    this->actions.at(2)->action = new Wander();
    this->actions.at(2)->name=std::string("Wander");
    this->actions.at(2)->cur = cur;
    this->actions.at(2)->goal = new Kinematic(640, 640, -1);
    this->actions.at(2)->goal->_position = randomPosition(604, 640);
    this->decisions.at(4)->falseBranch = this->actions.at(2);
    lastAction = this->actions.at(2);
    this->root->falseBranch = lastAction;
    

     printf("Making tree 8\n");

    //false branch vis is in corner
    this->decisions.at(3)->falseBranch = this->decisions.at(4);

    //true branch vis is can reach
    this->decisions.emplace_back(new GameTreeDecision());
    this->decisions.at(5)->decisionMaker = new CanReachFruit();
    this->decisions.at(5)->name=std::string("Can Reach Fruit");
    this->decisions.at(5)->cur = cur;
    this->decisions.at(5)->game = game;
    this->decisions.at(3)->trueBranch = this->decisions.at(5);
     printf("Making tree 9\n");

    //true branch can reach
    this->actions.emplace_back(new GameTreeAction());
    this->actions.at(3)->action = new EatFruit();
    this->actions.at(3)->name=std::string("Eat Fruit");
    this->actions.at(3)->cur = cur;    
    this->actions.at(3)->goal = new Kinematic(640, 640, -1);
    this->actions.at(3)->goal->_position = randomPosition(604, 640);
    this->decisions.at(5)->trueBranch = this->actions.at(3);
     printf("Making tree 10\n");

    this->actions.emplace_back(new GameTreeAction());
    this->actions.at(4)->action = new Seek();
    this->actions.at(4)->name=std::string("Seek Fruit");
    this->actions.at(4)->cur = cur;
    this->actions.at(4)->goal = new Kinematic(640, 640, -1);
    this->actions.at(4)->goal->_position =randomPosition(640, 640);
    this->actions.at(4)->truegoal=this->actions.at(4)->goal->_position;
    this->decisions.at(5)->falseBranch = this->actions.at(4);
     printf("Making tree 11\n");



}
void GameTree::updateTree(Kinematic *cur, GameWorld *game, float time){
    if(!game->fruitList.empty()){
        game->updategoals(cur->_position, time);
        this->actions.at(3)->goal->_position=game->goals.top().second->coords;
        this->actions.at(4)->goal->_position=game->goals.top().second->coords;


    }
    this->actions.at(2)->goal->_position = randomPosition(604, 640);
    for(GameTreeAction *a: this->actions){
        a->cur=cur;
    }
    for(GameTreeDecision *a: this->decisions){
        a->cur=cur;
    }
}