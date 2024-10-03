#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "kinematic.h"
#include "steeringbehavior.h"
#include "SteeringData.h"



int main()
{
    srand (static_cast <unsigned> (time(0)));
    freopen("output.txt","w",stdout);

    //size of window in x direction
    const int windowx = 640;
    //size of window in y direction
    const int windowy = 480;
    
    //number of time steps it should take to move across the window edge
    const int moveRate = 45;
    //a timer to track when to update sprites
    sf::Clock timer;

    //Theoretically you could generate more sprites but would not for this
    const int counter = 1;
    //amount to move in the y direction so that it takes moverate to cross the window
    int movey= windowy/moveRate;
    //amount to move in the x direction so that it takes moverate to cross the window
    int movex=windowx/moveRate;

    sf::Time previousTime=timer.getElapsedTime();
    sf::Time currentTime;
    float deltaTime;
    sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Demo Game");
    //not sure this actually matters but caps frame rate at 60
    window.setFramerateLimit(60);
    //load texture in for sprites
    sf::Texture texture;
    texture.loadFromFile("boid.png");

    //creates an array of sprites and their corresponding visibility marker...I want dodisplay to be bools but apparently this is not ok..when I make make my own sprite class this wll be a field 
  
    

    //initialize sprites with textures and update how large they are as well as initialize dodisplays to false
 
    Kinematic chars=Kinematic(windowx, windowy);
    chars.sprite.setTexture(texture);
    chars.sprite.setScale(.10, .10);
    chars.isVisible=1;
    chars.sprite.setPosition(windowx/2, windowy/2);
    
    
    sf::Vector2i mouse=sf::Mouse::getPosition();
    sf::Vector2i  newMouse;
    Kinematic target=Kinematic(windowx, windowy);
    
    //creates a vector of how large a sprite is. This mostly matters for y directed movement as for whatever reason it wants to run off screen
    const sf::Vector2f spriteSize(
    chars.sprite.getTexture()->getSize().x * chars.sprite.getScale().x,
    chars.sprite.getTexture()->getSize().y * chars.sprite.getScale().y);
    
     VelocityMatch matcher= VelocityMatch();
     SteeringData loopbehavior;
    // run the program as long as the window is open
    while (window.isOpen())
    {
      
        
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
       
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

       
        if(timer.getElapsedTime().asSeconds()>=.01f){
             newMouse = sf::Mouse::getPosition(window);
             deltaTime=timer.restart().asSeconds();
             if(newMouse!=mouse){
                
                target._velocity=sf::Vector2f((newMouse.x-mouse.x)/(deltaTime), (newMouse.y-mouse.y)/deltaTime);
                loopbehavior=matcher.calculateAcceleraction(chars, target);
                chars.update(loopbehavior,deltaTime);
                mouse=newMouse;
             }
            
            }
        



        
        window.clear(sf::Color(0, 95, 0));
        window.draw(chars.sprite);
        window.display();
        //printf("end loop\n\n\n");
    }
    

    return 0;
}