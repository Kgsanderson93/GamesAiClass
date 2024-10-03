
#include <SFML/Graphics.hpp>
#include <vector>
#include "kinematic.h"
#include "util.h"
#include "SteeringData.h"
#include <math.h>

Kinematic::Kinematic(int windowx, int windowy){

    last_time=sf::Time::Zero.asSeconds();
    crumbs=FixedQueue<crumb, 5>();
    _satisfied=false;

}
void Kinematic::update(SteeringData data, float time){
    bool drop=false;
    last_time+=time;
    //printf("time to drop: %f",last_time);

    if(last_time>drop_timer||crumb_idx==0){
        //printf("time to drop: %f",last_time);
            drop=true;
            last_time-=drop_timer;
    } 
    if(drop==true){
        //printf("drop crumb?");

        crumb *newb=new crumb();
        if(crumb_idx<15){
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
    if(sqrMag(data.linear)!=0 && _satisfied==false){
    _velocity=_velocity+(data.linear*time);
    _rotation=_orientation+(data.angular*time);
    //printf("length %f\n", length(_velocity));
    if(sqrMag(_velocity)>MAX_VEL*MAX_VEL){
       // printf("toobig x:%f y:%f\n", _velocity.x, _velocity.y);
        trim();
        
    }
    //printf("postNorm x:%f y:%f\n", _velocity.x, _velocity.y);
    sprite.move(_velocity);
    sprite.setRotation(_rotation);
    
    }else{
        _velocity=sf::Vector2f(0,0);
        _rotation=0;
        //sprite.move(_velocity);

    }
    
}
void Kinematic::trim(){
    _velocity=normalize(_velocity);
    _velocity=_velocity*MAX_VEL;
}

void Kinematic::trimRotation(){
    if(abs(_rotation)>MAX_ROT){
        _rotation=(_rotation/_rotation)*MAX_ROT;
    }
}
