#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


int main()
{
    //size of window in x direction
    const int windowx = 640;
    //size of window in y direction
    const int windowy = 480;
    
    //number of time steps it should take to move across the window edge
    const int moveRate = 45;
    //a timer to track when to update sprites
    sf::Clock timer;
    //Theoretically you could generate more sprites but would not for this
    const int counter = 4;
    //amount to move in the y direction so that it takes moverate to cross the window
    int movey= windowy/moveRate;
    //amount to move in the x direction so that it takes moverate to cross the window
    int movex=windowx/moveRate;

    
    sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Demo Game");
    //not sure this actually matters but caps frame rate at 60
    window.setFramerateLimit(60);
    //load texture in for sprites
    sf::Texture texture;
    texture.loadFromFile("boid.png");

    //creates an array of sprites and their corresponding visibility marker...I want dodisplay to be bools but apparently this is not ok..when I make make my own sprite class this wll be a field 
    std::array <sf::Sprite, counter> sprite;
    std::array <int, counter> dodisplay;

    //initialize sprites with textures and update how large they are as well as initialize dodisplays to false
    for(int i = 0; i <counter; i++){
        sprite[i].setTexture(texture);
        sprite[i].setScale(.10, .10);
        dodisplay[i]=0;
    }
    //initialize sprite 1 to visible
    dodisplay[0]=1;
    
    //creates a vector of how large a sprite is. This mostly matters for y directed movement as for whatever reason it wants to run off screen
    const sf::Vector2f spriteSize(
    sprite[0].getTexture()->getSize().x * sprite[0].getScale().x,
    sprite[0].getTexture()->getSize().y * sprite[0].getScale().y);
    
    
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

        // should spawn new sprite when last sprite hits right top edge
        // when a sprite reaches start position delete sprite

        if(timer.getElapsedTime().asMilliseconds() >= 100){

                //try to check if next move will collide with window if so rotate 90 degrees
                //dir = 0 going r 
                //dir = 90 going down
                //dir = 180 going left
                //dir = 270 going up
            for(int i = 0; i <counter; i++){
                if (dodisplay[i]==1){
                    sf::Vector2f pos=sprite[i].getPosition();
                
                    float dir = sprite[i].getRotation();

                    if(dir == 0){
                        if(pos.x + movex+spriteSize.x >= windowx){
                            sprite[i].rotate(90);
                            sprite[i].setPosition(windowx, 0.f);
                            //reached top right corner so as long as we are not the last sprite we should trigger a new sprite
                            if (i < 3){
                                dodisplay[i+1]=1;
                            }
        
                        }else{
                            sprite[i].move(movex, 0);
                        }
                       
                    }
                    else if(dir == 90){
                        if(pos.y+(spriteSize.y/2) + movey+spriteSize.y >= windowy){
                            sprite[i].rotate(90);
                            sprite[i].setPosition(windowx, windowy);
                        }else{
                            sprite[i].move(0,movey);
                        }
                    }

                    else if(dir == 180){
                        if(pos.x - (movex+spriteSize.x) <=0){
                            sprite[i].rotate(90);
                            sprite[i].setPosition(0, windowy);
                        }else{
                            sprite[i].move(-movex, 0);
                        }
                        timer.restart();
                    
                    }   
                    else if (dir == 270){
                        if(pos.y-(spriteSize.y/2) - (movey+spriteSize.y) <=0){
                            sprite[i].rotate(90);
                            sprite[i].setPosition(0, 0);
                            dodisplay[i]=0;
                            //if this is the last sprite then our pattern is complete. Turn on sprite 1 to restart
                            if(i==3){
                                dodisplay[0]=1;
                            }
                            
                        }else{
                            sprite[i].move(0,-movey);
                        }
                        
                    }
                }
            }
           
           
            timer.restart();
        }
        
        window.clear(sf::Color(0, 95, 0));
        for (unsigned int i = 0; i < counter; ++i)
            if(dodisplay[i]==1)
                window.draw(sprite[i]);
        window.display();
    }

    return 0;
}