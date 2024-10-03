#include "Mob.h"

void updateTree(Behavior *n, GameWorld *game){
    
}

    Mob::Mob(LearningTree *lt, Kinematic * movement, GameWorld *game,Kinematic *goal, Kinematic *player ){
        this->movement=movement;
        this->game=game;
        this->goal=goal;
        this->player=player;
        this->lt=lt;
        this->tree= new LearnedTree(lt, game, this->movement, goal, player);
    }
    void Mob::tick(float time, GameWorld *game, Kinematic *player){
        this->tree->game=game;
        this->tree->player=player;

        //std::cout<<tree->root->getStatus()<<"\n";
        float m=distancelength(this->movement->_position, player->_position);
        float sp=distancelength(this->movement->sprite.getPosition(), player->sprite.getPosition());
        //std::cout<<"dis m "<< m<< " dis sp "<<sp<<"player dead? "<<player->dead<<"\n";
        if(tree->root->isTerminated()&&!player->dead){
            //std::cout<<"new tree\n";
            this->tree= new LearnedTree(lt, game, this->movement, this->goal, player);
        }
        
          
        
        this->tree->tick(time);
        this->player=this->tree->player;
        this->game=this->tree->game;
        
        
    }

