#pragma once
#include <SFML/Graphics.hpp>
#include "kinematic.h"
#include "SteeringData.h"





class SteeringBehavior{
   
    public: virtual SteeringData calculateAcceleraction(Kinematic cur, Kinematic goal) = 0;
};

class Arrive: public SteeringBehavior{
       
    public: 
    SteeringData calculateAcceleraction(Kinematic cur, Kinematic goal) override;

};

class Align: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleraction(Kinematic cur, Kinematic goal) override;

};
class Seek: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleraction(Kinematic cur, Kinematic goal) override;

};

class VelocityMatch: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleraction(Kinematic cur, Kinematic goal) override;

};