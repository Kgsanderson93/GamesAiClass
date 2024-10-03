#include <SFML/Graphics.hpp>
#include <vector>

#include "steeringbehavior.h"
#include "kinematic.h"
#include <math.h>
#include "util.h"




SteeringData Align::calculateAcceleraction(Kinematic cur, Kinematic goal){

    float maxAngularAcceleration=cur.MAX_VEL;
    float maxRotation=cur.MAX_ROT;

// # The radius for arriving at the target.
    float targetRadius= cur.RAD_SAT;
// # The radius for beginning to slow down.
    float slowRadius= cur.RAD_DEC;

// # The time over which to achieve target speed.
    float timeToTarget= cur.TIME_TO_TARGET;
    
    SteeringData result = SteeringData();
    result.linear=sf::Vector2f(0,0);
    result.angular=0;

// # Get the naive direction to the target.
    
   // printf("curr: %f, %f\n", cur.sprite.getPosition().y, cur.sprite.getPosition().x);
   // printf("goal %f %f\n", goal._position.y, goal._position.x);
    float dx=goal._position.x-cur.sprite.getPosition().x;
    float dy=goal._position.y-cur.sprite.getPosition().y;
    goal._orientation=atan2(-dy, dx);
    
    //printf("goal in r:%f\n", goal._orientation);
    goal._orientation=radiansToDegrees(goal._orientation);
    //printf("goal in deg: %f\n", goal._orientation);
    //printf("Char.roatation in deg%f\n", cur.sprite.getRotation());
   float pretendRotation=mapToRange(cur.sprite.getRotation());
    
    //printf("Char.roatation in deg Mapped%f\n", pretendRotation);

    
   // printf("GOAL I GUESS %f\n", goal._orientation);
    float rotation=goal._orientation-pretendRotation;
    //printf("YOU SPIN ME PRE-MAPPED:%f\n", rotation);
// # Map the result to the (-pi, pi) interval.
   

    rotation = mapToRange(rotation);
    //printf("YOU SPIN ME MAPPED:%f\n", rotation);
    float rotationSize = abs(rotation);
    //printf("YOU SPIN ME NO NEGSS:%f\n", rotationSize);


// # Check if we are there, return no steering.
    if (cur._satisfied==true){
        //printf("You need to stop now\n");
        return result;  
    }
    float targetRotation;
// # If we are outside the slowRadius, then use maximum rotation.
     if (rotationSize > slowRadius){

      
        targetRotation=rotation;
    
    }   
    // # Otherwise calculate a scaled rotation.
    else{
        targetRotation =rotation/slowRadius;
    }
//# The final target rotation combines speed (already in the
// # variable) and direction.
    //printf("targetRotation: %f\n", targetRotation);

    

   //rintf("targetRotation: %f\n", targetRotation);
// # Acceleration tries to get to the target rotation.
    result.angular = targetRotation;
    //printf(" YOU SPIN ME RIGHT RIGHT2:%f\n", result.angular);

    result.angular /= (radiansToDegrees(timeToTarget));
    //printf(" YOU SPIN ME RIGHT RIGHT1:%f\n", result.angular);

    
    return result;



  
}


SteeringData Arrive::calculateAcceleraction(Kinematic cur, Kinematic goal){

    /**
     * class Arrive:
2 character: Kinematic (cur)
3 target: Kinematic (goal)
*/
    float maxAcceleration= cur.MAX_ACC;
    float maxSpeed= cur.MAX_VEL;
// # The radius for arriving at the target.

    float targetRadius= cur.RAD_SAT;

// # The radius for beginning to slow down.
    float slowRadius= cur.RAD_DEC;
// # The time over which to achieve target speed.
    float timeToTarget= cur.TIME_TO_TARGET;
    SteeringData result = SteeringData();
    float targetSpeed;
    sf::Vector2f targetVelocity;
// # Get the direction to the target.
    sf::Vector2f direction;
    direction.x=goal._position.x-(cur.sprite.getPosition().x);
    direction.y=goal._position.y-(cur.sprite.getPosition().y);
    //printf("direction: %f %f\n",direction.x, direction.y);
   
// # Check if we are there, return no 
   

     if (sqrMag(direction)<= (targetRadius*targetRadius)){
       // printf("stop please\n\n");
        result.linear=sf::Vector2f(0,0);
        result.angular=0;
        cur._satisfied=true;
        return result;
    }else{
        cur._satisfied=false;
    }

//# If we are outside the slowRadius, then move at max speed.
    if (sqrMag(direction) > slowRadius*slowRadius){
        targetSpeed = maxSpeed;
    }
//# Otherwise calculate a scaled speed.
    else{
    targetSpeed = maxSpeed * length(direction) / slowRadius;
    }
// # The target velocity combines speed and direction.
    //printf("target speed: %f\n",targetSpeed);
    targetVelocity = direction;
   // printf("target: %f %f\n",targetVelocity.x, targetVelocity.y);
    targetVelocity = normalize(targetVelocity);
    //printf("target post norm: %f %f\n",targetVelocity.x, targetVelocity.y);

    targetVelocity *= targetSpeed;
   // printf("target post maxed: %f %f\n",targetVelocity.x, targetVelocity.y);
   // printf("length:%f\n",length(targetVelocity));

// # Acceleration tries to get to the target velocity.
    result.linear = targetVelocity - cur._velocity;
    result.linear /= timeToTarget;

// # Check if the acceleration is too fast.->not really do this in update
 
    result.angular = 0;
    return result;

   
    
}

SteeringData Seek::calculateAcceleraction(Kinematic cur, Kinematic goal){
    SteeringData vals= SteeringData();
    vals.linear=goal._position-cur.sprite.getPosition();
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
      //  printf("Pretime x:%f\n", vals.linear.x);
      //  printf("preTime y:%f\n", vals.linear.y);
        vals.linear.x = vals.linear.x / cur.TIME_TO_TARGET;
        vals.linear.y = vals.linear.y / cur.TIME_TO_TARGET;
      //  printf("PostTime x:%f\n", vals.linear.x);
      //  printf("PostTime y:%f\n", vals.linear.y);
        vals.angular = 0;
        return vals;
    }
}



