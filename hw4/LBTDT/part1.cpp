#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "character/character.h"
#include "behavior/steeringbehavior.h"
#include "behavior/SteeringData.h"
#include "decisionTree/GameTree.h"
#include <math.h>

#include "World/gameWorld.h"
#include "Graph/Edge.h"
#include "character/Mob.h"
#include "Data/LearnedTree.h"

void printTreeInMAIN(LearningTreeDecisionNode *n);
int main()
{
    int GameId = 0;
    std::list<std::string> ships;
    ships.emplace_back(std::string("Ship1.png"));
    ships.emplace_back(std::string("Ship2.png"));
    ships.emplace_back(std::string("Ship3.png"));
    ships.emplace_back(std::string("Ship4.png"));
    ships.emplace_back(std::string("Ship5.png"));
    ships.emplace_back(std::string("Ship6.png"));
    ships.emplace_back(std::string("Ship7.png"));
    ships.emplace_back(std::string("Ship8.png"));
    bool test =true;
    bool verbose;
    std::string pathforfile;
    if (test)
    {
        verbose = false;
        pathforfile = "nonsense.txt";
    }
    else
    {
        verbose = true;
        pathforfile = "DataHw4.txt";
    }
    srand(static_cast<unsigned>(time(0)));
    std::ofstream out(pathforfile);
    std::streambuf *coutbuf;
    if (verbose)
    {
        coutbuf = std::cout.rdbuf(); // save old buf
        std::cout.rdbuf(out.rdbuf());
    }
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

    character *curr = new character(new Kinematic(windowx, windowy, 1, "boid.png"), GameId, test);
    Kinematic *temp = new Kinematic(windowx, windowy, -1, "");
    Kinematic *target = new Kinematic(windowx, windowy, -1, "");
    curr->charmove->sprite.setPosition(curr->world->tileList.at(2)->coords);
    curr->charmove->_position = curr->charmove->sprite.getPosition();

    curr->world->addFruit(sf::String("peach"));

    LearningTree *lt=new LearningTree("data4att.txt", "DataHw4.txt");
    printTreeInMAIN(lt->root);
    std::cout << ("post print\n");

    LearnedTree *learnedTree;
    bool start = false;
    std::cout<<"start\n";
    Mob *mob = new Mob(lt, new Kinematic(windowx, windowy, 2, ships.front()), curr->world, temp, curr->charmove);
    mob->movement->sprite.setPosition(curr->world->getTile(curr->world->tileList.at(500)->coords)->coords);
    mob->movement->_position = mob->movement->sprite.getPosition();
    mob->movement->sprite.setScale(sf::Vector2f(.5, .5));

    mob->tree->tick(timer.restart().asSeconds());

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
        if (start == false)
        {
            timer.restart();
        }
        if (curr->charmove->dead)
        {
            //std::cout << "\n\n\n\n PLAYER IS DEAD\n\n";
            mob->tick(timer.restart().asSeconds(), curr->world, curr->charmove);
        }
        else
        {
            if (timer.getElapsedTime().asSeconds() >= (1.0f / 30.0f) && start == true)
            {
                float delta = timer.restart().asSeconds();
                //std::cout << "\n\nupdate char\n";

                curr->updateChar(delta);

                //std::cout << "\n\ntick mob\n";
                mob->tick(delta, curr->world, curr->charmove);
                curr->charmove->dead = mob->player->dead;
                curr->charmove->beenhit = mob->player->beenhit;
                //std::cout<<"post tick/n";
            }
            if (curr->charmove->dead)
            {
                //std::cout << "\n\n\n\n PLAYER IS DEAD\n\n";
            }
        }

        /**std::cout << animateTimer.getElapsedTime().asSeconds()<< "," << curr->charmove->isEating << "," << curr->charmove->beenhit<<","<<curr->charmove->dead <<","<<curr->charmove->dancing
        << "," << curr->charmove->_position.x << "," <<curr->charmove->_position.y <<"," <<curr->tree->lastAction->name<<","<< curr->charmove->_velocity.x<<", "<<curr->charmove->_velocity.y<<","<<curr->charmove->_rotation <<"," << mob->movement->_position.x << ","
        << mob->movement->_position.y << "," <<mob->movement->_velocity.x<<","<< mob->movement->_velocity.y<<","<<mob->movement->_rotation<<"," << curr->world->fruitList.size()<<"," << distancelength(curr->charmove->_position, mob->movement->_position)
        << "," <<  mob->movement->isEating<< ","<<mob->movement->dancing<<"\n";
*/

        // draw world
        window.clear(sf::Color(255, 255, 255));
        for (int i = 0; i < curr->world->gridx; i++)
        {

            for (int j = 0; j < curr->world->gridx; j++)
            {
                //std::cout<<("%f %f\n",curr->world->tiles[i][j]->pos.x, curr->world->tiles[i][j]->pos.x);

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

        std::list<Kinematic *> drawme = std::list<Kinematic *>();
        drawme.emplace_back(curr->charmove);
        drawme.emplace_back(mob->movement);

        for (Kinematic *k : drawme)
        {
            FixedQueue<crumb, 150> temp;
            Kinematic *target = k;
            while (!target->crumbs.empty())
            {
                // if(chars.crumb_idx==15){
                //     window.close();

                //}
                //std::cout<<("PLEASE GIVE A CRUMB");
                crumb bready = target->crumbs.front();

                ////std::cout<<(" Where crumb at: %f, %f\n", temp.query().x, temp.query().y);
                bready.draw(&window);

                target->crumbs.pop();
                temp.push(bready);
            }
            while (!temp.empty())
            {
                target->crumbs.push(std::move(temp.front()));
                temp.pop();
            }
            if (target->dancing > 0)
            {
                if (target->sprite.getColor() == sf::Color::Black)
                {
                    target->sprite.setColor(sf::Color::Green);
                }
                if (target->sprite.getColor() == sf::Color::Green)
                {
                    target->sprite.setColor(sf::Color::Magenta);
                }
                else
                {
                    target->sprite.setColor(sf::Color::Red);
                }
            }
            if (!target->dead)
            {
                window.draw(target->sprite);
            }
        }

        if (curr->charmove->dead && mob->movement->dancing <= 2)
        {
            std::cout<<"remake"<<animateTimer.restart().asSeconds()<<"\n";
            int g1 = random() % 661;
            int g2 = random() % 661;

            curr = new character(new Kinematic(windowx, windowy, 1, "boid.png"), GameId++, test);
            curr->charmove->sprite.setPosition(curr->world->getTile(curr->world->tileList.at(g1)->coords)->coords);
            curr->charmove->_position = curr->charmove->sprite.getPosition();
            //printf("%ld\n", ships.size());
            if (ships.size() > 1)
            {

                ships.erase(ships.begin());
            }
            //printf("%ld\n", ships.size());

            window.draw(curr->charmove->sprite);

            mob = new Mob(lt, new Kinematic(windowx, windowy, 2, ships.front()), curr->world, temp, curr->charmove);
            mob->movement->sprite.setPosition(curr->world->getTile(curr->world->tileList.at(g2)->coords)->coords);
            mob->movement->_position = mob->movement->sprite.getPosition();
            mob->movement->sprite.setScale(.5, .5);
            window.draw(mob->movement->sprite);
            

            timer.restart();
            animateTimer.restart();
            generatefruittimer.restart();
        }

        if (!curr->world->fruitList.empty())
        {
            for (fruit *f : curr->world->fruitList)
            {
                if (mob->movement->isEating && mob->movement->targetFruit->id == f->id)
                {
                    window.draw(mob->movement->targetFruit->sprite);
                }
                else
                {
                    window.draw(f->sprite);
                }
            }
        }
        window.display();

        ////std::cout<<("curr: %f, %f\n", chars._position.y, chars._position.y);

        //std::cout<<("endloop\n\n");
    }
    if (verbose)
    {
        out.close();
        std::cout.rdbuf(coutbuf);
    }
    return 0;
}

void printTreeInMAIN(LearningTreeDecisionNode *n){
    LearningTreeNodeLeaf *m;

    std::cout<<"attName: " <<n->attr_name<<" val"<<n->attr_val<<" \n";
    if(n->trueBranch->type.compare("leaf")!=0 && n->trueBranch->type.compare("null")!=0){
        std::cout<<"True branch of "<<"attName: " <<n->attr_name<<" val"<<n->attr_val<<" true branch ";
        printTreeInMAIN((LearningTreeDecisionNode*)n->trueBranch);
    }else{
        m=(LearningTreeNodeLeaf *)n->trueBranch;
        std::cout<<"True Leaf child of "<< n->attr_name<< " "<<n->attr_val<<" " <<n->trueBranch->type<<" name "<<n->trueBranch->name<<" "<<m->targetName<<"\n";
    }
    
    if(n->falseBranch!=NULL && n->falseBranch->type.compare("leaf")!=0 && n->falseBranch->type.compare("null")!=0){
        std::cout<<"false branch of "<<"attName: "<<n->attr_name<<" val"<<n->attr_val<<"  ";
        printTreeInMAIN((LearningTreeDecisionNode*)n->falseBranch);
    }else if(n->falseBranch!=NULL){
        m=(LearningTreeNodeLeaf* )n->falseBranch;
        std::cout<<"False Leaf child of "<< n->attr_name<< " "<<n->attr_val<<" " <<n->falseBranch->type<<" name "<<n->falseBranch->name<<" "<<m->targetName<<"\n";
    }else{
        std::cout<<" false NULL\n";
    }
    
};