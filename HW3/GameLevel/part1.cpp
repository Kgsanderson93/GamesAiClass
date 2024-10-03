#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "kinematic.h"
#include "steeringbehavior.h"
#include "SteeringData.h"
#include <math.h>
// #include "flock.h"
// #include "crumb.h"
// #include "fixedQ.h"
#include "gameWorld.h"
// #include "Node.h"
#include "Edge.h"

int main()
{
    srand(static_cast<unsigned>(time(0)));
    std::ofstream out("seriously.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); // save old buf
    std::cout.rdbuf(out.rdbuf());
    // size of window in x direction

    // size of window in x direction
    const float windowy = 640;
    const float windowx = 640;
    sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Demo Game");
    window.setFramerateLimit(60);

    // number of time steps it should take to move across the window edge
    // a timer to track when to update sprites
    sf::Clock timer;
    float deltaTime;

    GameWorld gameWorld = GameWorld();
    printf("made the world\n");
    // Theoretically you could generate more sprites but would not for this
    const int spriteCounter = 1;

    // not sure this actually matters but caps frame rate at 60

    // load texture in for sprites
    sf::Texture texture;
    texture.loadFromFile("boid.png");
    sf::Sprite sprite(texture);
    sprite.setTexture(texture);
    sprite.setScale(.02, .02);
    sf::Vector2f spriteSize = sf::Vector2f(
        sprite.getTexture()->getSize().x * sprite.getScale().x,
        sprite.getTexture()->getSize().y * sprite.getScale().y);
    // printf("%f %f\n", spriteSize.x, spriteSize.y);
    // creates an array of sprites and their corresponding visibility marker...I want dodisplay to be bools but apparently this is not ok..when I make make my own sprite class this wll be a field

    Kinematic *temp = new Kinematic(windowx, windowy, -1);
    // printf("Try to make a flock\n");
    flock flock;
    for (int i = 0; i < spriteCounter; i++)
    {
        Kinematic k(windowx, windowy, i);
        k.sprite.setTexture(texture);
        k.sprite.setScale(.02, .02);
        k.sprite.setOrigin(spriteSize.x / 2, spriteSize.y / 2);

        flock.addBoid(k);
    }
    flock.getGoing(timer.getElapsedTime().asMicroseconds());
    flock.flocking(timer.getElapsedTime().asMicroseconds());

    // printf("done flocking\n");

    Kinematic *target = new Kinematic(windowx, windowy, -1);
    // target._position=(sf::Vector2f(windowx/2, windowy/2));
    bool update = false;

    // creates a vector of how large a sprite is. This mostly matters for y directed movement as for whatever reason it wants to run off screen

    // printf("Try to make a Flock obj\n");
    Flocking flocking = Flocking();

    // printf("Try to make a Look obj\n");

    LookWhereYoureGoing look = LookWhereYoureGoing();
    GoInDirection go = GoInDirection();
    Seek seek = Seek();
    Align align = Align();
    Arrive arrive = Arrive();
    Face face = Face();

    SteeringData loopbehavior;
    SteeringData loopbehavior2;
    // bool keepUpdate=true;
    Wander wander = Wander();

    Wander2 wander2 = Wander2();

    // printf("char or %f %f\n", chars.sprite.getOrigin().x, chars.sprite.getOrigin().y);
    // printf("char size %f %f\n", spriteSize.x, spriteSize.y);
    int loopcount = 0;
    // printf("Char.roatation %f\n", chars.sprite.getRotation());
    // printf("update? %d", update);
    //  run the program as long as the window is open

    // std::vector<Node> Nodelist=std::vector<Node>();
    // for(std::vector<gameTile *> row:gameWorld.tiles){
    //     for(gameTile *tile:row){
    //         if(tile->ispassable){
    //             std::string name="";
    //             name+=tile->idCol+" "+tile->idRow;
    //             std::cout<<name<<"\n";
    //             Nodelist.emplace_back(tile->idCol*100+tile->idRow, name, tile->pos.x, tile->pos.y);
    //         }
    //     }
    // }
    // std::vector<Edge> edgelist=std::vector<Edge>();
    // for(Node node:Nodelist){

    // }

    std::vector<gameTile *> tileList;
    for (std::vector<gameTile *> row : gameWorld.tiles)
    {
        for (gameTile *tile : row)
        {
            if (tile->ispassable)
            {
                tileList.emplace_back(tile);
            }
        }
    }

    for (gameTile *tile : tileList)
    {
        std::cout << tile->getName() << "\n";
    }
    printf("list size %ld\n", tileList.size());
    flock.getBoidRef(0).sprite.setPosition(tileList.at(2)->coords);
    printf("gonna try searching\n");
    out.close();
    std::cout.rdbuf(coutbuf);
    printf("gonna try moving the list\n");
    testSearchRef(tileList, gameWorld.map, 10, true);

    std::unordered_map<int, int> no_edge = std::unordered_map<int, int>();
    std::unordered_map<gameTile *, gameTile *> came_from = std::unordered_map<gameTile *, gameTile *>();
    std::unordered_map<gameTile *, gameTile *> *came_fromptr = &came_from;
    std::unordered_map<gameTile *, float> cost_so_far = std::unordered_map<gameTile *, float>();
    std::unordered_map<gameTile *, float> *cost_so_farptr = &cost_so_far;
    std::vector<gameTile *> returnedpath = std::vector<gameTile *>();
    int index = 1;
    std::ofstream out2("loop.txt");
    std::streambuf *coutbuf2 = std::cout.rdbuf(); // save old buf
    std::cout.rdbuf(out2.rdbuf());
    while (window.isOpen())
    {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                // printf("got a click\n");
                came_from.clear();
                cost_so_far.clear();
                returnedpath.clear();
                temp->_position = ((sf::Vector2f)sf::Mouse::getPosition(window));
                printf("mouse position: %f %f\n", temp-> _position.x, temp->_position.y);
                printf("boid position: %f %f\n",flock.getBoidRef(0).sprite.getPosition().x, flock.getBoidRef(0).sprite.getPosition().y);
                gameTile *goal = gameWorld.getTile(temp->_position);
                gameTile *start = gameWorld.getTile(flock.getBoid(0).sprite.getPosition());
                aStarRef(tileList, gameWorld.map, start->getID(), goal->getID(), came_from, cost_so_far, true, true, true);
                make_pathRef(start, goal, came_from, returnedpath);
                
                if (returnedpath.empty() == true)
                {
                    printf("NO PATH!\n");
                    update = false;
                }
                else
                {

                    prettyprintRef(returnedpath);
                    
                    update = true;
                    index = 0;
                    temp->_position = returnedpath.at(index++)->coords;
                    timer.restart();
                }
                
                // window.close();
            }
        }

        // should spawn new sprite when last sprite hits right top edge
        // when a sprite reaches start position delete sprite
        if (timer.getElapsedTime().asSeconds() >= (.1f / 60.f)&&update==true)
        {

            //printf("time %f\n", deltaTime);

            // printf("in loop\n");

            // loopbehavior=flocking.calculateAcceleration(&flock.getBoidRef(i), &flock);
            // printf("in loop try flock%f %f\n", loopbehavior.linear.x, loopbehavior.linear.y);

            // loopbehavior2=look.calculateAcceleration(&flock.getBoidRef(i), temp);
            // printf("in loop try look %f\n", loopbehavior2.angular);

            // loopbehavior.angular=loopbehavior2.angular;

            // flock.getBoidRef(i).updateData=std::move(loopbehavior);

            // printf("Check transfer %f %f ang: %f\n\n\n\n", flock.getBoid(i).updateData.linear.x,flock.getBoid(i).updateData.linear.y, flock.getBoid(i).updateData.angular );

            // printf("goal:%f\n", loopbehavior2.angular);
       
        
            for (int i = 0; i < spriteCounter; i++)
            {
                    deltaTime = timer.restart().asSeconds();

                    printf("Not satifsfied!\n");
                    if(index<returnedpath.size()&& flock.getBoid(i)._satisfied){
                    sf::Vector2f tile = returnedpath.at(index++)->coords;
                    tile.x+=10.f;
                    tile.y+=10.f;
                    temp->_position=tile;
                    }
                    loopbehavior = arrive.calculateAcceleration(&flock.getBoidRef(i), temp);
                    printf("accelerating: loopbehavior%f, %f\n", loopbehavior.linear.x, loopbehavior.linear.y);
                    loopbehavior.angular = align.calculateAcceleration(&flock.getBoidRef(i), temp).angular;
                    printf("accelerating: loopbehavior%f\n", loopbehavior.angular);

                    flock.getBoidRef(i).updateData = std::move(loopbehavior);
                    flock.flocking(deltaTime);
                
                if (flock.getBoidRef(0)._satisfied == true && index == returnedpath.size())
                {
                    update = false;
                }
            }
        
            // }else if(fmod(loopcount,4)==0 ){
            //     //printf("Switch\n\n\n");
            //      loopbehavior=wander.calculateAcceleration(&flock.getBoidRef(i), temp);
            //     //loopbehavior2.angular*=.5f;

            //     //loopbehavior2.linear=go.calculateAcceleration(&flock.getBoidRef(i), &flock.getBoidRef(0)).linear*.5f;
            //     //loopbehavior.angular=align.calculateAcceleration(&flock.getBoidRef(i), temp).angular;
            //     //loopbehavior.linear+=go.calculateAcceleration(&flock.getBoidRef(i), temp).linear;
            //     loopbehavior.linear*=temp->MAX_VEL;
            //     flock.getBoidRef(i).updateData=std::move(loopbehavior);

            // }
        }

        window.clear(sf::Color(255, 255, 255));
        for (int i = 0; i < gameWorld.gridx; i++)
        {

            for (int j = 0; j < gameWorld.gridx; j++)
            {
                // printf("%f %f\n",gameWorld.tiles[i][j]->pos.x, gameWorld.tiles[i][j]->pos.x);

                window.draw(gameWorld.tiles[i][j]->sprite);
            }
        }
        crumb bread=crumb();

        for (int k=0; k<returnedpath.size();k++){
            if(index==k){
                bread.setColor(sf::Color::Green);
            }if(index<k){
                bread.setColor(sf::Color::Cyan);

            }else{
            bread.setColor(sf::Color::Magenta);
            }
            bread.drop(returnedpath.at(k)->coords);
            bread.draw(&window);

        }
        for (int j = 0; j < spriteCounter; j++)
        {
            FixedQueue<crumb, 150> temp;
            target = &flock.getBoidRef(j);
            while (!flock.getBoidRef(j).crumbs.empty())
            {
                // if(chars.crumb_idx==15){
                //     window.close();

                //}
                // printf("PLEASE GIVE A CRUMB");
                crumb bready = target->crumbs.front();

                // printf(" Where crumb at: %f, %f\n", temp.query().x, temp.query().y);
                bready.draw(&window);

                target->crumbs.pop();
                temp.push(bready);
            }
            while (!temp.empty())
            {
                target->crumbs.push(std::move(temp.front()));
                temp.pop();
            }
            window.draw(target->sprite);
        }

        window.display();

        // printf("curr: %f, %f\n", chars._position.y, chars._position.y);

        // printf("endloop\n\n");
    }
    out2.close();
    std::cout.rdbuf(coutbuf2);
    return 0;
}