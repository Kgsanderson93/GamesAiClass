#include <SFML/Graphics.hpp>
#include <vector>

#include "steeringbehavior.h"
#include "kinematic.h"
#include <math.h>
#include "util.h"




SteeringData Align::calculateAcceleraction(Kinematic cur, Kinematic goal){
    SteeringData vals= SteeringData();
    float rotation= goal._rotation-cur._rotation;
    rotation=mapToRange(rotation);
    float rotationSize=abs(rotation);
    float targetRotation;
    if(inRadiusSat(rotationSize)){
        return  vals;
    }
    else if(inRadiusDecel(rotationSize)){
        targetRotation=cur.MAX_ROT*rotationSize/cur.RAD_DEC;
    }else{
        targetRotation=cur.MAX_ROT;
    }
    targetRotation= targetRotation*rotation/rotationSize;
    vals.angular=targetRotation-cur._rotation;
    vals.angular=vals.angular/cur.TIME_TO_TARGET;
    vals.linear=sf::Vector2f(0,0);
    return vals;
}


SteeringData Arrive::calculateAcceleraction(Kinematic cur, Kinematic goal){
    SteeringData vals= SteeringData();
    sf::Vector2f direction=goal._position-cur._position;
    
 
    float distance=length(direction);
   
    float goalSpeed;
    if (cur._satisfied=inRadiusSat(distance)){
         goalSpeed=0;
    }
    else if(inRadiusDecel(distance)){
         goalSpeed=cur.MAX_VEL*(distance/cur.RAD_DEC);
    }
    else{
        goalSpeed=cur.MAX_VEL;
    }
    sf::Vector2f goalVel=direction;
    goalVel=normalize(goalVel);
    goalVel.x=goalVel.x*goalSpeed;
    goalVel.y=goalVel.y*goalSpeed;
    vals.linear=goalVel-cur._velocity;
    vals.angular=0;
    return vals;

}

SteeringData Seek::calculateAcceleraction(Kinematic cur, Kinematic goal){
    SteeringData vals= SteeringData();
    vals.linear=goal._position-cur._position;
    vals.linear=normalize(vals.linear);
    vals.linear=vals.linear*cur.MAX_VEL;
    vals.angular=0;
    return vals;
}

SteeringData VelocityMatch::calculateAcceleraction(Kinematic cur, Kinematic goal){
    SteeringData vals= SteeringData();
//    if (goal._velocity.x == 0.00 && goal._velocity.y == 0.00)
//    {
//        printf("no change!\n");
//        vals.linear.x = cur._velocity.x * cur.MAX_DECEL;
//        vals.linear.y = cur._velocity.y * cur.MAX_DECEL;
//        if (vals.linear.x < .01)
//        {
//            vals.linear.x = 0;
//        }
//        if (vals.linear.y < .01)
//        {
//            vals.linear.y = 0;
//        }

//        return vals;
//   }
//   else
    {
        vals.linear.x = goal._velocity.x - cur._velocity.x;
        vals.linear.y = goal._velocity.y - cur._velocity.y;
   
        vals.linear.x = vals.linear.x/cur.TIME_TO_TARGET;
        vals.linear.y = vals.linear.y/cur.TIME_TO_TARGET;

        vals.angular = 0;
        return vals;
    }
}

