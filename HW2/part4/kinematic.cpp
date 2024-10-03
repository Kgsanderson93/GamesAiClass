
#include <SFML/Graphics.hpp>
#include <vector>
#include "kinematic.h"
#include "util.h"
#include "SteeringData.h"
#include <math.h>

Kinematic::Kinematic(int windowx, int windowy, int i){
    
    this->target_idx=i;
    this->last_time=sf::Time::Zero.asSeconds();
    this->crumbs=FixedQueue<crumb, 10>();
    this->_satisfied=false;
    this->windowx=windowx;
    this->windowy=windowy;
    this->sprite=sf::Sprite();
   
    

    //this->sprite.setTexture(texture);
    //this->sprite.setColor();

   

    //this->isVisible=1;
    
    this->sprite.setPosition(randomPosition(windowx, windowy));
    this->_position=this->sprite.getPosition();
    this->_velocity=sf::Vector2f(0,0);
    this->_orientation=sprite.getRotation();
    this->_rotation=0;
    rdmspdAdjust();
    _velocity*=3.f;


}
void Kinematic::update(SteeringData data, float time){
    bool drop=false;
    last_time+=time;
    //printf("time to drop: %f\n",last_time);

    if(last_time>drop_timer||crumb_idx==0){
        //printf("time to drop: %f",last_time);
            drop=true;
            last_time=0;
    } 
    if(drop==true){
        //printf("drop crumb?");

        
        crumb *newb=new crumb();
        if(crumb_idx<crumbs.size()){
            crumb_idx++;
        }
        newb->drop(sprite.getPosition());
        crumbs.push(*newb);
        //printf("length of crumbs %d", crumb_idx);
    }
    
    float half_t_sq=.5*time*time;
    _position.x=sprite.getPosition().x;
    _position.y=sprite.getPosition().y;


    
    _orientation=sprite.getRotation()+(_rotation*time);
    if(sqrMag(data.linear)!=0){

    _velocity=_velocity+(data.linear*time);
    _rotation=_orientation+(data.angular*time);
    }
    //printf("length %f\n", length(_velocity));
    if(sqrMag(_velocity)>MAX_VEL*MAX_VEL){
       // printf("toobig x:%f y:%f\n", _velocity.x, _velocity.y);
        trim();
        
    }
    if(sqrMag(_velocity)< MIN_VEL*MIN_VEL&& sqrMag(_velocity)!=0){
        expand();
    }
    if(sqrMag(_velocity)==0){
        rdmspdAdjust();
    }
    //printf("postNorm x:%f y:%f\n", _velocity.x, _velocity.y);
    
  
   
    sprite.move(_velocity);
    inBounds();
    sprite.setRotation(_rotation);
    
    //printf("id: %d, Pos: %f, %f Vel: %f, %f\n\n",target_idx, sprite.getPosition().x, sprite.getPosition().y, _velocity.x, _velocity.y);
   
    
}
void Kinematic::rdmspdAdjust(){
    float rand=fmod(random(), 3);
    rand=rand-1;

    if(sqrMag(_velocity)==0){
        _velocity.x+=rand;
        _velocity.y+=rand;
    }else{
        _velocity*=(1.f/(rand+.01f));
    }
}
void Kinematic::trim(){
    _velocity=normalize(_velocity);
    _velocity=_velocity*MAX_VEL;

}
void Kinematic::expand(){
    _velocity=normalize(_velocity);
    _velocity=_velocity*MIN_VEL;

}

void Kinematic::trimRotation(){
    if(abs(_rotation)>MAX_ROT){
        _rotation=(_rotation/_rotation)*MAX_ROT;
    }
}
void Kinematic::inBounds(){
    _position=sprite.getPosition();
    if(_position.x>=this->windowx){
       _position.x=0;
    }
    if(_position.x<0){
        _position.x=this->windowx;
    }
      if(_position.y>=this->windowy){
       _position.y=0;
    }
    if(_position.y<0){
        _position.y=this->windowy;
    }
    sprite.setPosition(_position);

}

const Kinematic Kinematic::operator=(const Kinematic &kin){
    sf::Texture texture;
    texture.loadFromFile("boid.png");
    Kinematic wtf=Kinematic(1,1,-1);
    return wtf;
}


