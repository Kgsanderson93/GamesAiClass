
#include <SFML/Graphics.hpp>
#include <vector>
#include "kinematic.h"
#include "../utilities/util.h"
#include "../behavior/SteeringData.h"
#include <math.h>

Kinematic::Kinematic(int windowx, int windowy, int i, std::string path)
{
    this->beenhit=false;
    this->isVisible=true;
    this->dead=false;
    this->sprite = sf::Sprite();
    this->sprite.setPosition(randomPosition(windowx, windowy));
    this->texture.loadFromFile(path);
    this->sprite.setTexture(this->texture);
    this->sprite.scale(.02, .02);
    this->_position = this->sprite.getPosition();
    this->_velocity = sf::Vector2f(0, 0);
    this->_orientation = sprite.getRotation();
    this->_rotation = 0;
    this->_satisfied = false;
    this->_inDecel = false;
    this->windowx = windowx;
    this->windowy = windowy;
    this->crumbs = FixedQueue<crumb, 15>();
    this->last_time = sf::Time::Zero.asSeconds();
    this->spriteSize = sf::Vector2f(
        sprite.getTexture()->getSize().x * sprite.getScale().x,
        sprite.getTexture()->getSize().y * sprite.getScale().y);
    this->isEating = false;
    this->dancing = 0;
    this->doneEating = false;
    this->target_idx = i;

    this->closestNeighbor = 0;
}
void Kinematic::update(float time)
{
    bool drop = false;
    last_time += time;
   //std::cout<<("time to drop: %f\n",last_time);

    if (last_time > drop_timer || crumb_idx == 0)
    {
       //std::cout<<("time to drop: %f",last_time);
        drop = true;
        last_time = 0;
    }
    if (drop == true)
    {
       //std::cout<<("drop crumb?");

        crumb *newb = new crumb();
        if (crumb_idx < crumbs.size())
        {
            crumb_idx++;
        }
        newb->drop(sprite.getPosition());
        crumbs.push(*newb);
       //std::cout<<"length of crumbs "<<crumb_idx<<"\n";
    }
    if (this->isEating){

        this->_velocity=sf::Vector2f(0, 0);
        this->_rotation=0;
        if(targetFruit->frame.left<900 && targetFruit->timesinceupdate>drop_timer){
            targetFruit->frame.left+=100;
            targetFruit->sprite.setTextureRect(targetFruit->frame);
            targetFruit->timesinceupdate=0;
        }else{
            targetFruit->timesinceupdate+=time;
        }
        
        return;
    }
    
    if (this->dancing > 0)
    {
       //std::cout<<"Dancing! "<<dancing<<"\n";
        this->_velocity=sf::Vector2f(0, 0);
         _rotation += (updateData.angular * time);
        this->sprite.rotate(_rotation);
        this->dancing -= time;
        return;
    }
   //std::cout<<"Is satisfied?"<<_satisfied<<"\n";
    if (_satisfied == false)
    {

        //float half_t_sq = .5 * time * time;
        _position.x = sprite.getPosition().x;
        _position.y = sprite.getPosition().y;

        _orientation = sprite.getRotation();
        if (sqrMag(updateData.linear) != 0)
        {
           //std::cout<<"Vel before"<<_velocity.x<<" "<<_velocity.y<<"\n";
            _velocity = _velocity + (updateData.linear * time);
           //std::cout<<"Vel after"<<_velocity.x<<" "<<_velocity.y<<"\n";
           //std::cout<<"Rot before "<< _rotation<<"\n";
            _rotation += (updateData.angular * time);
           //std::cout<<"Rot after "<< _rotation<<"\n";

        }

       //std::cout<<("length %f\n", length(_velocity));
        if (sqrMag(_velocity) > MAX_VEL * MAX_VEL)
        {
           //std::cout<<("toobig x:%f y:%f\n", _velocity.x, _velocity.y);
            trim();

        }
        if (abs(_rotation) > MAX_ROT)
        {
            _rotation /= abs(_rotation);
            _rotation *= MAX_ROT;
        }

       //std::cout<<("postNorm x:%f y:%f\n", _velocity.x, _velocity.y);

       //std::cout<<("vel %f, %f\n", _velocity.x, _velocity.y);
        sprite.move(_velocity);
        inBounds();
        sprite.rotate(_rotation);
    }
    else
    {
        _velocity = sf::Vector2f(0, 0);
        _rotation = 0;
    }

   //std::cout<<("id: %d, Pos: %f, %f Vel: %f, %f\n\n",target_idx, sprite.getPosition().x, sprite.getPosition().y, _velocity.x, _velocity.y);
}
void Kinematic::rdmspdAdjust()
{
    float rand = fmod(random(), 3);
    rand = rand - 1;

    if (sqrMag(_velocity) == 0)
    {
        _velocity.x = (rand * _velocity.y) + .001;
        _velocity.y *= (rand * _velocity.x) + .001;
    }
    else
    {
        _velocity *= (1.f / (rand + .01f));
    }
    _velocity *MAX_ACC;
}
void Kinematic::trim()
{
    _velocity = normalize(_velocity);
    _velocity = _velocity * MAX_VEL;
}
void Kinematic::expand()
{
    _velocity = normalize(_velocity);
    _velocity = _velocity * MIN_VEL;
}

void Kinematic::trimRotation()
{
    if (abs(_rotation) > MAX_ROT)
    {
        _rotation = (_rotation / _rotation) * MAX_ROT;
    }
}
void Kinematic::inBounds()
{
    _position = sprite.getPosition();
    if (_position.x >= this->windowx)
    {
        _position.x = 0;
    }
    if (_position.x < 0)
    {
        _position.x = this->windowx;
    }
    if (_position.y >= this->windowy)
    {
        _position.y = 0;
    }
    if (_position.y < 0)
    {
        _position.y = this->windowy;
    }
    sprite.setPosition(_position);
}

const Kinematic Kinematic::operator=(const Kinematic &kin)
{
    sf::Texture texture;
    texture.loadFromFile("boid.png");
    Kinematic wtf = Kinematic(1, 1, -1, "");
    return wtf;
}

