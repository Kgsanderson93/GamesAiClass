#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "character/character.h"
#include "behavior/steeringbehavior.h"
#include "behavior/SteeringData.h"
#include "decisionTree/GameTree.h"
#include <math.h>

#include "World/gameWorld.h"
#include "Graph/Edge.h"

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

    sf::Clock timer;
    float deltaTime;
    sf::Clock animateTimer;
    sf::Clock generatefruittimer;

    character *curr = new character(new Kinematic(windowx, windowy, 1));
    Kinematic *temp = new Kinematic(windowx, windowy, -1);
    Kinematic *target = new Kinematic(windowx, windowy, -1);
    curr->charmove->sprite.setPosition(curr->world->tileList.at(2)->coords);
    curr->charmove->_position=curr->charmove->sprite.getPosition();

    curr->world->addFruit(sf::String("peach"));
    std::cout << ("afterFruit\n");

    bool start = false;
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

                start = true;
                // window.close();
            }
        }
        if(start==false){
            timer.restart();
        }
        if (timer.getElapsedTime().asSeconds() >= (1.0f/60.0f) && start == true)
        {

            curr->updateChar(timer.restart().asSeconds());
        }
        // draw world
        window.clear(sf::Color(255, 255, 255));
        for (int i = 0; i < curr->world->gridx; i++)
        {

            for (int j = 0; j < curr->world->gridx; j++)
            {
                // std::cout<<("%f %f\n",curr->world->tiles[i][j]->pos.x, curr->world->tiles[i][j]->pos.x);

                window.draw(curr->world->tiles[i][j]->sprite);
            }
        }
        crumb bread = crumb();

        for (int k = 0; k < curr->path->returnedpath.size(); k++)
        {
            if (curr->pathidx == k)
            {
                bread.setColor(sf::Color::Green);
            }
            if (curr->pathidx < k)
            {
                bread.setColor(sf::Color::Cyan);
            }
            else
            {
                bread.setColor(sf::Color::Magenta);
            }
            bread.drop(curr->path->returnedpath.at(k)->coords);
            bread.draw(&window);
        }

        FixedQueue<crumb, 150> temp;
        target = curr->charmove;
        while (!curr->charmove->crumbs.empty())
        {
            // if(chars.crumb_idx==15){
            //     window.close();

            //}
            // std::cout<<("PLEASE GIVE A CRUMB");
            crumb bready = target->crumbs.front();

            // std::cout<<(" Where crumb at: %f, %f\n", temp.query().x, temp.query().y);
            bready.draw(&window);

            target->crumbs.pop();
            temp.push(bready);
        }
        while (!temp.empty())
        {
            target->crumbs.push(std::move(temp.front()));
            temp.pop();
        }
        if(target->dancing>0){
            if(target->sprite.getColor()==sf::Color::Black){
            target->sprite.setColor(sf::Color::Green);
            }
            if(target->sprite.getColor()==sf::Color::Green){
                target->sprite.setColor(sf::Color::Magenta);
            }
            else{
                 target->sprite.setColor(sf::Color::Red);
            }

        }
        window.draw(target->sprite);

        if (!curr->world->fruitList.empty())
        {
            for (fruit *f : curr->world->fruitList)
            {

                window.draw(f->sprite);
            }

            window.display();

            // std::cout<<("curr: %f, %f\n", chars._position.y, chars._position.y);

            // std::cout<<("endloop\n\n");
        }
    }
    out.close();
    std::cout.rdbuf(coutbuf);
    return 0;
}