#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "kinematic.h"
#include "steeringbehavior.h"
#include "SteeringData.h"
#include <math.h>
//#include "flock.h"
//#include "crumb.h"
//#include "fixedQ.h"



int main()
{    
    freopen("output.txt","w",stdout);

    //size of window in x direction
    const int windowx = 640*1.5;
    //size of window in y direction
    const int windowy = 480*1.5;
    sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Demo Game");
    window.setFramerateLimit(60);

    //number of time steps it should take to move across the window edge
    //a timer to track when to update sprites
    sf::Clock timer;
    float deltaTime;

   


    //Theoretically you could generate more sprites but would not for this
    const int spriteCounter = 60;


    //not sure this actually matters but caps frame rate at 60
    
    //load texture in for sprites
    sf::Texture texture;
    texture.loadFromFile("boid.png");
    sf::Sprite sprite(texture);
    sprite.setTexture(texture);
    sprite.setScale(.05, .05);
    sf::Vector2f spriteSize=sf::Vector2f(
    sprite.getTexture()->getSize().x * sprite.getScale().x,
    sprite.getTexture()->getSize().y * sprite.getScale().y);
    //printf("%f %f\n", spriteSize.x, spriteSize.y);
    //creates an array of sprites and their corresponding visibility marker...I want dodisplay to be bools but apparently this is not ok..when I make make my own sprite class this wll be a field 
  
    Kinematic *temp=new Kinematic(windowx, windowy,-1);
    //printf("Try to make a flock\n");
    flock flock;
    for(int i =0; i<spriteCounter; i++){
        Kinematic k(windowx, windowy, i);
        k.sprite.setTexture(texture);
        k.sprite.setScale(.03, .03);
        k.sprite.setOrigin(spriteSize.x/2, spriteSize.y/2);

        flock.addBoid(k);
        
    }
    flock.getGoing(timer.getElapsedTime().asMicroseconds());
    flock.flocking(timer.getElapsedTime().asMicroseconds());
    
    //printf("done flocking\n");

    

    Kinematic *target=new Kinematic(windowx, windowy,-1);
    //target._position=(sf::Vector2f(windowx/2, windowy/2));
    bool update=false;

    //creates a vector of how large a sprite is. This mostly matters for y directed movement as for whatever reason it wants to run off screen
   
    
    //printf("Try to make a Flock obj\n");
    Flocking2 flocking2=Flocking2();
   
    //printf("Try to make a Look obj\n");

    LookWhereYoureGoing look= LookWhereYoureGoing();
    GoInDirection go=GoInDirection();
    Seek seek=Seek();
    Face face=Face();


    SteeringData loopbehavior;
    SteeringData loopbehavior2;
    //bool keepUpdate=true;
    Wander wander=Wander();
    //printf("char or %f %f\n", chars.sprite.getOrigin().x, chars.sprite.getOrigin().y);
    //printf("char size %f %f\n", spriteSize.x, spriteSize.y);
    int loopcount=0;
    //printf("Char.roatation %f\n", chars.sprite.getRotation());
    //printf("update? %d", update);
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
            //else if(event.type==sf::Event::MouseButtonPressed){
                //printf("got a click\n");
                 //target._position=((sf::Vector2f)sf::Mouse::getPosition(window));
                 //update=true;
                //window.close();
            //}
        }

        // should spawn new sprite when last sprite hits right top edge
        // when a sprite reaches start position delete sprite
          if(timer.getElapsedTime().asSeconds()>=(.1f/20.f) ){
            
            deltaTime=timer.restart().asSeconds();
            //printf("time %f\n", deltaTime);


            //printf("in loop\n");
                for(int i=0; i<spriteCounter; i++){  
                    

                    loopbehavior=flocking2.calculateAcceleration(&flock.getBoidRef(i), &flock);
                    printf("in loop try flock%f %f\n", loopbehavior.linear.x, loopbehavior.linear.y);


                    loopbehavior2=face.calculateAcceleration(&flock.getBoidRef(i), temp);
                    printf("in loop try look %f\n", loopbehavior2.angular);

                    loopbehavior.angular=loopbehavior2.angular;

                    
                    flock.getBoidRef(i).updateData=std::move(loopbehavior);

                  
                    //printf("Check transfer %f %f ang: %f\n\n\n\n", flock.getBoid(i).updateData.linear.x,flock.getBoid(i).updateData.linear.y, flock.getBoid(i).updateData.angular );

            
                //printf("goal:%f\n", loopbehavior2.angular);

                loopcount++;
                if(loopcount=40){
                    wander.calculateTarget(flock.getBoidRef(i), &flock.getBoidRef(0));
                    loopbehavior2.linear=seek.calculateAcceleration(&flock.getBoidRef(i), &flock.getBoidRef(0)).linear*.5f;
                    //loopbehavior2.angular*=.5f;
                    
                    loopbehavior.linear+=loopbehavior2.linear;
                    //loopbehavior2.linear=go.calculateAcceleration(&flock.getBoidRef(i), &flock.getBoidRef(0)).linear*.5f;
                    loopbehavior2.angular+=look.calculateAcceleration(&flock.getBoidRef(i), temp).angular;
                    loopbehavior.angular+=loopbehavior2.angular;
                    loopbehavior.linear+=loopbehavior2.linear;

                    flock.getBoidRef(i).updateData=std::move(loopbehavior);

                    loopcount=0;
                }
                }
                
            flock.flocking(deltaTime);

        }
       
       
        
        window.clear(sf::Color(255, 255, 255));
        for(int j=0; j<spriteCounter; j++){  
            FixedQueue<crumb, 15> temp;
            target=&flock.getBoidRef(j);
            while(!flock.getBoidRef(j).crumbs.empty())  {
            //if(chars.crumb_idx==15){
            //    window.close();
            
            //}
                //printf("PLEASE GIVE A CRUMB");
                crumb bready=target->crumbs.front();

                //printf(" Where crumb at: %f, %f\n", temp.query().x, temp.query().y);
                bready.draw(&window);

                target->crumbs.pop();
                temp.push(bready);

        }
        while(!temp.empty()){
            target->crumbs.push(std::move(temp.front()));
            temp.pop();
        }
        window.draw(target->sprite);

        }
        
        
        window.display();
      

    
       
       


        //printf("curr: %f, %f\n", chars._position.y, chars._position.y);

        //printf("endloop\n\n");
        
          
       

        
        
       
       
    }
    

    return 0;
}