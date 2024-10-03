
#include <SFML/Graphics.hpp>
#include <vector>
#include "kinematic.h"
#include "util.h"
#include "SteeringData.h"

Kinematic::Kinematic(int windowx, int windowy){
    windowX=windowx;
    windowY=windowy;
    
}
void Kinematic::update(SteeringData data, float time){
    _position=_position+(_velocity*time);

    _orientation=_orientation+(_rotation*time);
    
    _velocity=_velocity+(data.linear*time);
    _rotation=_rotation+(data.angular*time);
    printf("length %f\n", length(_velocity));
    if(length(_velocity)>MAX_VEL){
        printf("toobig x:%f y:%f\n", _velocity.x, _velocity.y);
        _velocity=normalize(_velocity);
        _velocity=_velocity*MAX_VEL;
         printf("postNorm x:%f y:%f\n", _velocity.x, _velocity.y);
    }

    sprite.move(_velocity);
    sprite.rotate(_rotation);


}
