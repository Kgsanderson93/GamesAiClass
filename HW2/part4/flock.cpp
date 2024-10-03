#include "flock.h"
#include "steeringbehavior.h"
#include "util.h"
Kinematic flock::getBoid(int i){
    return boids[i];
}
Kinematic &flock::getBoidRef(int i){
    return boids[i];
}
void flock::addBoid(const Kinematic &b ){
    boids.push_back(std::move(b));
}
void flock::flocking(float time){
    for(int i=0; i<boids.size(); i++){
        boids[i].update(boids[i].updateData, time);
    }
}
void flock::getGoing(float time){
    GoInDirection arrive= GoInDirection();
    Wander wander = Wander();
    Align align= Align();
    SteeringData loopbehavior;
    SteeringData loopbehavior2;
    
    Kinematic *temp=new Kinematic(0,0, -1);
    Kinematic target=Kinematic(0,0,-1);

    for(int i=0; i<boids.size()-1; i++){
        Kinematic *temp=new Kinematic(0,0, -1);
        temp=wander.calculateTarget(getBoidRef(i), temp);
        target._position=temp->_position;
        target._orientation=temp->_orientation;
        loopbehavior2=align.calculateAcceleration(&boids[i], &target);
        loopbehavior=arrive.calculateAcceleration(&boids[i], &target);
        loopbehavior.angular=loopbehavior2.angular;
        loopbehavior.linear=normalize(loopbehavior.linear)*temp->MIN_VEL;
        getBoidRef(i).updateData=loopbehavior;


    }
    flocking(time);

}