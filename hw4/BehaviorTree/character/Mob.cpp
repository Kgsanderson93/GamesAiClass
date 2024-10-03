#include "Mob.h"


    Mob::Mob(Kinematic * movement, GameWorld *game,Kinematic *goal, Kinematic *player ){
        this->movement=movement;
        this->game=game;
        this->goal=goal;
        this->player=player;
        this->tree= new BehaviorTree(game, this->movement, goal, player);
    }
    void Mob::tick(float time, GameWorld *game, Kinematic *player){
       //std::cout<<tree->root->getStatus()<<"\n";
        float m=distancelength(this->movement->_position, player->_position);
        float sp=distancelength(this->movement->sprite.getPosition(), player->sprite.getPosition());
        //std::cout<<"dis m "<< m<< " dis sp "<<sp<<"player dead? "<<player->dead<<"\n";
        if(tree->root->isTerminated()&&!player->dead){
            this->tree= new BehaviorTree(game, this->movement, this->goal, player);
        }
        
          
        
        this->tree->tick(time);
        this->player=this->tree->player;
        this->game=this->tree->game;
        
        
    }

