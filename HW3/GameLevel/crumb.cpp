// 
// @author Derek Martin (dmartin7@ncsu.edu)
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "crumb.h"

//Breadcrumb class

        crumb::crumb()
        {   
            //set initial position and size breadcrumbs           
            this->setRadius(2.f);
            this->setFillColor(color);
            this->setPosition(-100, -100);
        }

        //tell breadcrumb to render self, using current render window
        void crumb::draw(sf::RenderWindow* window)
        {

            window->draw(*this);
        }

        //set position of breadcrumb
        void crumb::drop(float x, float y)
        {
            this->setPosition(x, y);
        }

        //set position of breadcrumb
        void crumb::drop(sf::Vector2f position)
        {
            this->setPosition(position);
        }

        sf::Vector2f crumb::query(){
            return this->getPosition();
        }
        void crumb::setColor(sf::Color newcolor){
            color=newcolor;
            this->setFillColor(color);
        }