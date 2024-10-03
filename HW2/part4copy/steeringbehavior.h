#pragma once
#include <SFML/Graphics.hpp>
#include "kinematic.h"
#include "SteeringData.h"
#include "flock.h"





class SteeringBehavior{
   
    public: virtual SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) = 0;
};

class Arrive: public SteeringBehavior{
       
    public: 
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};

class Align: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};
class Seek: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};

class VelocityMatch: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};
class GoInDirection: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};

class Flee: public SteeringBehavior{
    public: 
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};

class Wander{
    public:
    float wanderOrientation;
    float wanderOffset=10;
    float wanderRadius=10;
    Kinematic *calculateTarget(Kinematic cur, Kinematic *goal);

};
class CollisionAvoidance: public SteeringBehavior{
    public: 
    Kinematic targets[20];
    float radius=4;
    
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};
class Flocking{
    public:     
    float minspeed=3.f;
    float maxspeed=6.f;
    float visual_rad=120.f;
    float protectedRan=40.f;
    float centering=0.1f;
    float matching=.3f;
    float avoid=.6f;
   // flock boids;
    
    
    SteeringData calculateAcceleration(Kinematic *cur, flock *flock);

};

class Flocking2{
    public:     
    float minspeed=3.f;
    float maxspeed=8.f;
    float visual_rad=120.f;
    float protectedRan=40.f;
    float centering=0.33f;
    float matching=.33f;
    float avoid=.33f;
   // flock boids;
    
    
    SteeringData calculateAcceleration(Kinematic *cur, flock *flock);

};
class LookWhereYoureGoing: public Align{
    public:
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};
class Face: public Align{
    public:
    SteeringData calculateAcceleration(Kinematic *cur, Kinematic *goal) override;

};
