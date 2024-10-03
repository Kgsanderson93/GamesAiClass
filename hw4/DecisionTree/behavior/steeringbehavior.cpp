
#include <SFML/Graphics.hpp>
#include <vector>

#include "../behavior/steeringbehavior.h"
#include "../character/kinematic.h"
#include <math.h>
#include "../utilities/util.h"

SteeringData Align::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
  std::cout<<("\n\nIn Align\n");
    float maxAngularAcceleration=cur->MAX_VEL;
    float maxRotation=cur->MAX_ROT;

// # The radius for arriving at the target.
    float targetRadius= cur->RAD_SAT;
// # The radius for beginning to slow down.
    float slowRadius= cur->RAD_DEC;

// # The time over which to achieve target speed.
    float timeToTarget= cur->TIME_TO_TARGET;
    
    SteeringData result = SteeringData();
    result.linear=sf::Vector2f(0,0);
    result.angular=0;

// # Get the naive direction to the target.
    
   // std::cout<<("curr: %f, %f\n", cur.sprite.getPosition().y, cur.sprite.getPosition().x);
    std::cout<<"goal "<< goal->_position.y<<" "<< goal->_position.x<<"/n";
    goal->_orientation=VecToDegAngle(goal->_position-cur->sprite.getPosition());
    
    std::cout<<"goal in deg: "<<goal->_orientation<<"\n";
    std::cout<<("Char.roatation in deg")<< cur->sprite.getRotation()<<"\n";
    //std::cout<<("Char.roatation in deg mapped%f\n",mapToRange(cur->sprite.getRotation()));

    float pretendRotation=goal->_orientation-mapToRange(cur->sprite.getRotation());
    pretendRotation=mapToRange(pretendRotation);

    
    //std::cout<<("YOU SPIN ME PRE-MAPPED:%f\n", rotation);
// # Map the result to the (-pi, pi) interval.
   

    std::cout<<"Niave direction-cur orientation:" << pretendRotation<<"\n";
    float rotationSize = abs(pretendRotation);
    std::cout<<"abs" << rotationSize<<"\n";


// # Check if we are there, return no steering.
    if (cur->_satisfied==true||rotationSize<1.f){
        std::cout<<("You need to stop now\n");
        return result;  
    }
    float targetRotation;
// # If we are outside the slowRadius, then use maximum rotation.
     if (rotationSize > 25.0f){
        targetRotation=maxRotation;
    }   
    // # Otherwise calculate a scaled rotation.
    else{
        targetRotation =(rotationSize*maxRotation)/25.0f;
    }
//# The final target rotation combines speed (already in the
// # variable) and direction.
    std::cout<<"targetRotation: "<<targetRotation<<"\n";

    targetRotation*=pretendRotation/rotationSize;
    
    result.angular=targetRotation-mapToRange(cur->_rotation);

   std::cout<<"targetRotation post direction: "<<targetRotation<<"\n";
// # Acceleration tries to get to the target rotation.
    //result.angular = targetRotation;
    //std::cout<<(" YOU SPIN ME RIGHT RIGHT2:%f\n", result.angular);

    result.angular*=cur->TIME_TO_TARGET;
    std::cout<<"rotation div time: "<<result.angular<<"\n";
    //result.angular=mapToRange(result.angular)+.000001f;

    
    return result;

}
SteeringData Dance::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    cur->doneEating=false;
    cur->isEating=false;
    cur->dancing=3;
    SteeringData result = SteeringData();
    result.linear = sf::Vector2f(0, 0);
    result.angular = 25*-1;
    return result;
}

SteeringData EatFruit::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData result = SteeringData();
    result.linear = sf::Vector2f(0, 0);
    result.angular = 0;
    return result;
}


SteeringData Arrive::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{

    /**
     * class Arrive:
2 character: Kinematic (cur)
3 target: Kinematic (goal)
*/
    float maxAcceleration = cur->MAX_ACC;
    float maxSpeed = cur->MAX_VEL;
    // # The radius for arriving at the target.

    float targetRadius = cur->RAD_SAT;

    // # The radius for beginning to slow down.
    float slowRadius = cur->RAD_DEC;
    // # The time over which to achieve target speed.
    float timeToTarget = cur->TIME_TO_TARGET;
    SteeringData result = SteeringData();
    float targetSpeed;
    sf::Vector2f targetVelocity;
    // # Get the direction to the target.
    sf::Vector2f direction;
    direction.x = goal->_position.x - (cur->sprite.getPosition().x) + .000001f;

    direction.y = goal->_position.y - (cur->sprite.getPosition().y) + .000001f;
    ////std::cout<<("direction: %f %f\n",direction.x, direction.y);

    // # Check if we are there, return no

    if (sqrMag(direction) <= (targetRadius * targetRadius))
    {
        ////std::cout<<("stop please\n\n");
        result.linear = sf::Vector2f(0, 0);
        result.angular = 0;
        cur->_satisfied = true;
        return result;
    }
    // # If we are outside the slowRadius, then move at max speed.
    if (sqrMag(direction) > slowRadius * slowRadius)
    {
        targetSpeed = maxSpeed;
        cur->_satisfied = false;
    }
    // # Otherwise calculate a scaled speed.
    else
    {
        targetSpeed = maxSpeed * length(direction) / slowRadius;
        cur->_satisfied = false;
    }
    // # The target velocity combines speed and direction.
    ////std::cout<<("target speed: %f\n",targetSpeed);
    targetVelocity = direction;
    ////std::cout<<("target: %f %f\n",targetVelocity.x, targetVelocity.y);
    targetVelocity = normalize(targetVelocity);
    ////std::cout<<("target post norm: %f %f\n",targetVelocity.x, targetVelocity.y);

    targetVelocity *= targetSpeed;
    ////std::cout<<("target post maxed: %f %f\n",targetVelocity.x, targetVelocity.y);
    ////std::cout<<("length:%f\n",length(targetVelocity));

    // # Acceleration tries to get to the target velocity.
    result.linear = targetVelocity - cur->_velocity;
    result.linear /= timeToTarget;

    // # Check if the acceleration is too fast.->not really do this in update

    return result;
}

SteeringData Seek::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData vals = SteeringData();
    vals.linear = goal->_position - cur->sprite.getPosition();
    vals.linear = normalize(vals.linear);
    vals.linear = vals.linear * cur->MAX_VEL;
    vals.angular = 0;
    return vals;
}
SteeringData Flee::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData vals = SteeringData();
    vals.linear = cur->sprite.getPosition() - goal->_position;
    vals.linear = normalize(vals.linear);
    vals.linear = vals.linear * cur->MAX_VEL;
    vals.angular = 0;
    return vals;
}
SteeringData GoInDirection::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData vals = SteeringData();
    vals.linear = getOrientationAsVector(goal->_orientation);
   //std::cout<<("direction %f, %f\n", vals.linear.x, vals.linear.y);
    vals.linear = normalize(vals.linear);
    vals.linear = vals.linear * fmodf(rand(), cur->MAX_VEL);
    vals.angular = 0;
    return vals;
}

SteeringData VelocityMatch::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData vals = SteeringData();
    //    if (goal._velocity.x == 0.00 && goal._velocity.y == 0.00)
    //    {
    //       //std::cout<<("no change!\n");
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
        vals.linear.x = goal->_velocity.x - cur->_velocity.x;
        vals.linear.y = goal->_velocity.y - cur->_velocity.y;
        // //std::cout<<("Pretime x:%f\n", vals.linear.x);
        // //std::cout<<("preTime y:%f\n", vals.linear.y);
        vals.linear.x = vals.linear.x / cur->TIME_TO_TARGET;
        vals.linear.y = vals.linear.y / cur->TIME_TO_TARGET;
        // //std::cout<<("PostTime x:%f\n", vals.linear.x);
        // //std::cout<<("PostTime y:%f\n", vals.linear.y);
        vals.angular = 0;
        return vals;
    }
}

Kinematic *Wander::calculateTarget(Kinematic cur, Kinematic *goal)
{

    float bin = randomBinomial();
    ////std::cout<<("bin= %f\n", bin);
    wanderOrientation += bin * cur.MAX_WANDER;
    // float direction=randomDirection();
    // wanderOrientation+=(direction*bin);
    ////std::cout<<("rand direction %f,\n bin %f\n", direction, bin);
    float char_orient = cur.sprite.getRotation();
    // char_orient=mapToRange(char_orient);
    goal->_orientation = wanderOrientation - char_orient;
    goal->_orientation = mapToRange(goal->_orientation);
    goal->_position = cur.sprite.getPosition();
    goal->_position += wanderOffset * getOrientationAsVector(cur.sprite.getRotation());
    goal->_position += wanderRadius * getOrientationAsVector(goal->_orientation);
    return goal;
}

SteeringData CollisionAvoidance::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData vals = SteeringData();
    float shortestTime = MAXFLOAT;

    Kinematic *firstTarget = nullptr;
    float firstMinSeparation;
    float firstDistance;
    sf::Vector2f firstRelativePos;
    sf::Vector2f firstRelativeVel;

    for (Kinematic &target : targets)
    {
        sf::Vector2f relativePos;
        relativePos.x = target.sprite.getPosition().x - cur->sprite.getPosition().x;
        relativePos.y = target.sprite.getPosition().x - cur->sprite.getPosition().y;
        sf::Vector2f relVel;
        relVel.x = target._velocity.x - cur->_velocity.x;
        relVel.y = target._velocity.y - cur->_velocity.y;
        float relativeSpeed = length(relVel);
        float timeToCollision = dotProduct(relativePos, relVel) / (relativeSpeed * relativeSpeed);
        float distance = length(relativePos);
        float MinSep = distance - (relativeSpeed * timeToCollision);
        if (MinSep > 2 * radius)
        {
            continue;
        }
        if (timeToCollision > 0 && timeToCollision < shortestTime)
        {
            shortestTime = timeToCollision;
            firstTarget = &target;
            firstMinSeparation = MinSep;
            firstDistance = distance;
            firstRelativePos = relativePos;
            firstRelativeVel = relVel;
        }
    }
    if (!firstTarget)
    {
        return vals;
    }
    sf::Vector2f relativePos;
    if (firstMinSeparation <= 0 || firstDistance < 2 * radius)
    {
        relativePos.x = firstTarget->sprite.getPosition().x - cur->sprite.getPosition().x;
        relativePos.y = firstTarget->sprite.getPosition().y - cur->sprite.getPosition().y;
    }
    else
    {
        relativePos.x = firstRelativePos.x + firstRelativeVel.x * shortestTime;
        relativePos.y = firstRelativePos.y + firstRelativeVel.y * shortestTime;
    }
    relativePos = normalize(relativePos);

    vals.linear.x = relativePos.x * cur->MAX_ACC;
    vals.linear.y = relativePos.y * cur->MAX_ACC;
    vals.angular = 0;
    return vals;
}

SteeringData Flocking2::calculateAcceleration(Kinematic *cur, flock *flock)
{
    SteeringData vals = SteeringData();
    // std::vector<Kinematic> *neighbors;
    // sf::Vector2f pos;
    // sf::Vector2f relVel;

    sf::Vector2f close = sf::Vector2f(0, 0);
    sf::Vector2f pos_avg = sf::Vector2f(0, 0);
    sf::Vector2f vel_avg = sf::Vector2f(0, 0);
    sf::Vector2f align;
    sf::Vector2f coh;
    // protectedRan=length(cur->spriteSize)*2;
    ////std::cout<<("Protections Size %f\n", protectedRan);
    visual_rad = protectedRan * 3;

    // float firstMinSeparation;
    float firstDistance;
    // sf::Vector2f firstRelativePos;
    // sf::Vector2f firstRelativeVel;
    // protectedRan=length(cur->spriteSize)/2;
    int closeindx = -1;
    int num_neighbors = 0;
    // float shortestTime;
    std::vector<int> neighborind;
    // std::vector<Kinematic> *neighbors;
    for (int i = 0; i < 30; i++)
    {
        if (flock->getBoidRef(i).target_idx == cur->target_idx)
        {
            continue;
        }
        if (length(flock->getBoidRef(i).sprite.getPosition() - cur->sprite.getPosition()) < length(flock->getBoidRef(closeindx).sprite.getPosition() - cur->sprite.getPosition()))
        {
            closeindx = i;
        }
        firstDistance = length(flock->getBoidRef(i).sprite.getPosition() - cur->sprite.getPosition());
        if (firstDistance > 0 && firstDistance < visual_rad)
        {
            pos_avg += flock->getBoidRef(i).sprite.getPosition();
            vel_avg += flock->getBoidRef(i)._velocity;
            num_neighbors++;
            sf::Vector2f diff;
            diff = cur->sprite.getPosition() - flock->getBoidRef(i).sprite.getPosition();
            diff = normalize(diff);
            diff *= (1 / (firstDistance + .001f));
            close += diff;
        }
    }
    if (num_neighbors > 0)
    {
        pos_avg *= (1.001f / num_neighbors);
        coh = pos_avg - cur->sprite.getPosition();
        coh = normalize(coh);
        coh *= centering;

        vel_avg *= (1.001f / num_neighbors);
        align = vel_avg - cur->_velocity;
        align = normalize(align);
        align *= matching;

        close *= (1.001f / num_neighbors);
        close = normalize(close);
        close *= avoid;
    }

    vals.linear += align + close + coh;
    cur->closestNeighbor = closeindx;

    // vals.linear*=cur->TIME_TO_TARGET*cur->MAX_ACC;
    return vals;
}
SteeringData Flocking::calculateAcceleration(Kinematic *cur, flock *flock)
{
    SteeringData vals = SteeringData();

    std::vector<Kinematic> *neighbors;
    sf::Vector2f pos;
    sf::Vector2f close = sf::Vector2f(0, 0);
    sf::Vector2f pos_avg = sf::Vector2f(0, 0);
    sf::Vector2f vel_avg = sf::Vector2f(0, 0);
    int closeindx = 0;
    int num_neighbors = 0;

    // flock->getBoid(0).isVisible;
    ////std::cout<<("in flock\n");
    for (int i = 0; i < 20; i++)
    {
        ////std::cout<<("target %d\n", flock->getBoid(i).target_idx);
        if (flock->getBoidRef(i).target_idx == cur->target_idx)
        {
            continue;
        }
        sf::Vector2f flockpos;
        flockpos.x = flock->getBoidRef(i).sprite.getPosition().x;
        pos.x = cur->sprite.getPosition().x - flock->getBoidRef(i).sprite.getPosition().x;
        ////std::cout<<("posx %f\n", flockpos.x);
        pos.y = cur->sprite.getPosition().y - flock->getBoidRef(i).sprite.getPosition().y;
        if (abs(pos.x) < visual_rad && abs(pos.y) < visual_rad)
        {
            float sqdis = sqrMag(pos);
            if (length(flock->getBoidRef(i).sprite.getPosition() - cur->sprite.getPosition()) < length(flock->getBoidRef(closeindx).sprite.getPosition() - cur->sprite.getPosition()))
            {
                closeindx = i;
            }
            if (sqdis < (protectedRan * protectedRan))
            {
                close.x += pos.x;
                close.y = pos.y;
            }
            else if (sqdis < (visual_rad * visual_rad))
            {
                pos_avg.x += flock->getBoidRef(i).sprite.getPosition().x;
                pos_avg.y += flock->getBoidRef(i).sprite.getPosition().y;
                vel_avg.x += flock->getBoidRef(i)._velocity.x;
                vel_avg.y += flock->getBoidRef(i)._velocity.y;
                num_neighbors++;
            }
        }
    }
    if (num_neighbors > 0)
    {
        pos_avg.x /= num_neighbors;
        pos_avg.y /= num_neighbors;
        vel_avg.x /= num_neighbors;
        vel_avg.y /= num_neighbors;

        vals.linear.x = cur->_velocity.x + (pos_avg.x - cur->_position.x) * centering + (vel_avg.x - cur->_velocity.x) * matching;
        vals.linear.y = cur->_velocity.y + (pos_avg.y - cur->_position.y) * centering + (vel_avg.y - cur->_velocity.y) * matching;
    }
    else
    {
        Seek seek = Seek();
        vals = seek.calculateAcceleration(cur, &flock->getBoidRef(closeindx));
    }
    vals.linear.x += (close.x * avoid);
    vals.linear.y += (close.y * avoid);

    if (cur->sprite.getPosition().y > (cur->windowy - (cur->windowy * .25)))
    {
        vals.linear.y -= .2f;
    }
    if (cur->sprite.getPosition().y < (cur->windowy * .25))
    {
        vals.linear.y += .2f;
    }
    if (cur->sprite.getPosition().x > cur->windowx - (cur->windowx * .25))
    {
        vals.linear.x -= .2f;
    }
    if (cur->sprite.getPosition().x < (cur->windowx * .25))
    {
        vals.linear.x += .2f;
    }
    // if(sqrMag(vals.linear)==0){
    // GoInDirection go=GoInDirection();
    // vals=go.calculateAcceleration(cur, cur);

    //}
    cur->closestNeighbor = closeindx;
    ////std::cout<<("see me%f, %f\n\n\n", vals.linear.x, vals.linear.y);
    return vals;
}
SteeringData LookWhereYoureGoing::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData vals = SteeringData();
    vals.linear = sf::Vector2f(0, 0);
    vals.angular = 0;
    if (sqrMag(cur->_velocity) == 0)
    {
        return vals;
    }
    goal->_orientation = atan2(-cur->_velocity.y, cur->_velocity.x);
    return Align::calculateAcceleration(cur, goal);
}

SteeringData Face::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    SteeringData vals = SteeringData();
    vals.linear = sf::Vector2f(0, 0);
    vals.angular = 0;
    if (sqrMag(cur->_velocity) == 0)
    {
        return vals;
    }
    float dx = goal->_position.x - cur->sprite.getPosition().x;
    float dy = goal->_position.y - cur->sprite.getPosition().y;
    goal->_orientation = atan2(-dy, dx);
    return Align::calculateAcceleration(cur, goal);
}

SteeringData Wander::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    wanderOrientation += randomBinomial() * cur->MAX_WANDER;
   //std::cout<<("wander %f\n", wanderOrientation);
    goal->_orientation = randomBinomial() * (wanderOrientation + cur->sprite.getRotation()) + .00001;
    goal->_position = cur->sprite.getPosition() + wanderOffset * getOrientationAsVector(cur->sprite.getRotation());
    goal->_position = wanderRadius * getOrientationAsVector(goal->_orientation);

    goal->sprite.setPosition(goal->_position);
    goal->sprite.setRotation(goal->_orientation);
    Face face = Face();
    SteeringData vals = face.calculateAcceleration(cur, goal);
    vals.linear = cur->MAX_ACC * getOrientationAsVector(vals.angular);
    return vals;
}

SteeringData Wander2::calculateAcceleration(Kinematic *cur, Kinematic *goal)
{
    Kinematic x = Kinematic(480, 480, -1);
   //std::cout<<("wander %f %f\n", x.sprite.getPosition().x, x.sprite.getPosition().y);
    goal->_position = x._position;
    goal->_velocity = x._position;
    goal->_orientation = atan2(-x._velocity.y + .001f, x._velocity.x + .001f);

    Arrive align = Arrive();
    Align face = Align();
    SteeringData vals;
    vals.linear = align.calculateAcceleration(cur, goal).linear;
    vals.linear.x += .0001f;
    vals.linear.y += .0001f;

   //std::cout<<("wander post face%f %f %f\n", vals.linear.x, vals.linear.y, vals.angular);
    vals.angular = face.calculateAcceleration(cur, goal).angular + .0001f;
    return vals;
}
