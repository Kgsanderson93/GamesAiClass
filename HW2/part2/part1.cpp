#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "kinematic.h"
#include "steeringbehavior.h"
#include "SteeringData.h"
#include <math.h>
//#include "crumb.h"
//#include "fixedQ.h"



int main()
{
    srand (static_cast <unsigned> (time(0)));
    freopen("output.txt","w",stdout);

    //size of window in x direction
    const int windowx = 640*1.5;
    //size of window in y direction
    const int windowy = 480*1.5;
    
    //number of time steps it should take to move across the window edge
    const int moveRate = 45;
    //a timer to track when to update sprites
    sf::Clock timer;

    //Theoretically you could generate more sprites but would not for this
    const int spriteCounter = 1;

    float deltaTime;
    int loop_counter=0;

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
    chars._position=chars.sprite.getPosition();
     
    Kinematic target=Kinematic(windowx, windowy);
    target._position=(sf::Vector2f(windowx/2, windowy/2));
    bool update=false;

    //creates a vector of how large a sprite is. This mostly matters for y directed movement as for whatever reason it wants to run off screen
    const sf::Vector2f spriteSize(
    chars.sprite.getTexture()->getSize().x * chars.sprite.getScale().x,
    chars.sprite.getTexture()->getSize().y * chars.sprite.getScale().y);
    chars.sprite.setOrigin(spriteSize.x/2, spriteSize.y/2);;
    
    Arrive arrive= Arrive();
    Align align= Align();


    SteeringData loopbehavior;
    SteeringData loopbehavior2;
    bool keepUpdate=true;
    //freopen("output.txt","w",stdout);
    //printf("char or %f %f\n", chars.sprite.getOrigin().x, chars.sprite.getOrigin().y);
    //printf("char size %f %f\n", spriteSize.x, spriteSize.y);

    //printf("Char.roatation %f\n", chars.sprite.getRotation());
    //printf("update? %d", update);
    // run the program as long as the window is open
    while (window.isOpen() && keepUpdate==true)
    {
      
        
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
       
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type==sf::Event::MouseButtonPressed){
                //printf("got a click\n");
                 target._position=((sf::Vector2f)sf::Mouse::getPosition(window));
                 update=true;
                //window.close();
            }
        }

        // should spawn new sprite when last sprite hits right top edge
        // when a sprite reaches start position delete sprite
          if(timer.getElapsedTime().asSeconds()>=(1.f/60.f) ){
            
            deltaTime=timer.restart().asSeconds();
            //printf("in loop\nupdate? %d\n", update);
            if(update==true){
                loopbehavior=arrive.calculateAcceleraction(chars, target);
                loopbehavior2=align.calculateAcceleraction(chars, target);
            
                //printf("goal:%f\n", loopbehavior2.angular);
                loopbehavior.angular=loopbehavior2.angular;

                chars.update(loopbehavior,deltaTime);
                if(chars._satisfied==true){
                    update=false;
                }
            }
            loop_counter++;

        }
       
       
        
      

    
       
       


        //printf("curr: %f, %f\n", chars._position.y, chars._position.y);

        //printf("endloop\n\n");
        window.clear(sf::Color(255, 255, 255));
        for(int i =0; i<chars.crumb_idx;i++){
            //if(chars.crumb_idx==15){
            //    window.close();
            //}
                //printf("PLEASE GIVE A CRUMB");
                crumb temp=chars.crumbs.front();

                //printf(" Where crumb at: %f, %f\n", temp.query().x, temp.query().y);
                temp.draw(&window);
                chars.crumbs.pop();
                chars.crumbs.push(temp);
        }
        window.draw(chars.sprite);
        window.display();
 
       

        
        
       
       
    }
    

    return 0;
}