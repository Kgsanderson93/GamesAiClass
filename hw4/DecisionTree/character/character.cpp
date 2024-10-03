#include "character.h"

character::character(Kinematic *charmove)
{
    this->charmove = charmove;
    this->path = new Path();
    this->world = new GameWorld();
    this->tree = new GameTree(charmove, world);

    arrive= Arrive();
    align=Align ();
    eatFruit=EatFruit();
    dance=Dance ();
}

void character::newBehavior(float time)
{

    this->tree->updateTree(charmove, world, time);
    std::cout << "in new behavior\n";
    std::cout << "check last action : " << tree->lastAction->name << "\n";

    GameTreeAction *d = (GameTreeAction *)this->tree->root->makeDecision();
    std::cout << "traversed tree found : " << d->name << "\n";

    if (d->name.compare("Wander") == 0)
    {
        std::cout << "in wander last action" << this->tree->lastAction->name << "\n";
        if (this->tree->lastAction->name.compare("Wander") == 0)
        {
            if (this->path->returnedpath.size() == 0) // if path is already done get a new one to wander on
            {
                std::cout << "in wander last action path size 0\n";

                // gameTile *g = this->world->getTileNearby(this->charmove->_position);

                gameTile *g = this->world->getTile(d->goal->_position);
                std::cout << g->coords.x << " " << g->coords.y << "\n";
                this->aquirePath(g);
                std::cout << path->returnedpath.size() << "\n";
                this->tree->lastAction->goal->_position = path->returnedpath.at(0)->coords;
                followPath();
                return;
            }
            else
            {
                std::cout << "in wander last action path size !0\n";

                followPath();
                return;
            }
        }
        else
        {
            tree->lastAction = d;
            std::cout << "in wander last action  not wander\n";

            gameTile *g = this->world->getTile(d->goal->_position);
            this->aquirePath(g);
            this->tree->lastAction->goal->_position = path->returnedpath.at(0)->coords;

            followPath();
            return;
        }
    }
    else if (d->name.compare("Seek Fruit") == 0)
    {
        gameTile *t = world->getTile(d->goal->_position);
        std::cout << "dist between pos and tile retrieved? " << distancelength(d->goal->_position, t->coords) << "\n";
        if (world->getTile(this->charmove->_position)->getID()== t->getID())
        {
            printf("I MAY HAVE A PROBLEM\n");
            std::cout << "dist to fruit: " << distancelength(charmove->_position, world->goals.top().second->coords) << "\n";
            std::cout << "dist to fruit: " << distancelength(charmove->_position, tree->lastAction->goal->_position) << "\n";
            charmove->updateData = arrive.calculateAcceleration(charmove, d->goal);
            std::cout << "moving: " << charmove->updateData.linear.x << " " << charmove->updateData.linear.y << " manual steering to fruit\n";

            charmove->updateData.angular = align.calculateAcceleration(charmove, d->goal).angular;
            return;
        }
        this->aquirePath(t);
        this->tree->lastAction->goal->_position = path->returnedpath.at(0)->coords;

        followPath();
        return;
    }
    else if (d->name.compare("Seek") == 0)
    {
        std::cout << "LastAction: " << this->tree->lastAction->name << "\n";
        if (this->tree->lastAction->name.compare("Seek") == 0)
        {

            std::cout << "LastAction: " << this->tree->lastAction->name << "Same goal\n";

            followPath();
            return;
        }
        else
        {
            tree->lastAction = d;

            std::cout << "LastAction: " << this->tree->lastAction->name << " but new goal\n";
            gameTile *t = world->getTile(d->goal->_position);
            std::cout << "dist between pos and tile retrieved? " << distancelength(d->goal->_position, t->coords) << "\n";
            this->aquirePath(t);
            this->tree->lastAction->goal->_position = path->returnedpath.at(0)->coords;

            followPath();
            return;
        }
    }
    else if (d->name.compare("Dance") == 0)
    {
        tree->lastAction = tree->actions.at(0);
        std::cout << "dance update tree found : " << tree->lastAction->name << "\n";

        this->charmove->updateData = dance.calculateAcceleration(this->charmove, this->charmove);
        return;
    }
    else if (d->name.compare("Eat Fruit") == 0)
    {

        tree->lastAction = tree->actions.at(3);

        std::cout << "eat fruit update tree found : " << tree->lastAction->name << "\n";

        this->charmove->updateData = eatFruit.calculateAcceleration(this->charmove, this->charmove);
        this->charmove->targetFruit = world->goals.top().second;
        return;
    }
    // this->tree->lastAction = d; // make sure old action is wander so we can track it.
}
void character::aquirePath(gameTile *g)
{

    this->path->makePath(this->charmove, g, world);
}

void character::followPath()
{
    tree->lastAction->goal->_position = path->returnedpath.at(path->idx)->coords;

    std::cout << "Follow path: " << path->returnedpath.empty() << " " << path->idx << " " << path->returnedpath.size() << " following path\n";
    std::cout << "curr pos: " << charmove->_position.x << " " << charmove->_position.y << "Goal pos: " << tree->lastAction->goal->_position.x << " " << tree->lastAction->goal->_position.y << "Dis bet: " << distancelength(charmove->_position, tree->lastAction->goal->_position) << "\n";
    charmove->_satisfied = inRadiusSat(distancelength(charmove->_position, tree->lastAction->goal->_position));
    std::cout << "curr pos sprite: " << charmove->sprite.getPosition().x << " " << charmove->sprite.getPosition().y << "Goal pos: " << path->returnedpath.at(path->idx)->coords.x << " " << path->returnedpath.at(path->idx)->coords.y << "Dis bet: " << distancelength(charmove->_position, charmove->sprite.getPosition()) << "\n";
    // charmove->_satisfied = inRadiusSat(distancelength(charmove->_position, tree->lastAction->goal->_position));
    std::cout << "Satisfied? " << charmove->_satisfied << "\n";
    if (!path->returnedpath.empty() && path->idx < path->returnedpath.size() && charmove->_satisfied)
    {
        std::cout << "LastAction: " << this->tree->lastAction->name << " following path\n";

        tree->lastAction->goal->_position = path->returnedpath.at(path->idx++)->coords;
        tree->lastAction->goal->_position.x += 10.0f;
        tree->lastAction->goal->_position.y += 10.0f;
        std::cout << "curr pos: " << charmove->_position.x << " " << charmove->_position.y << "Goal pos: " << tree->lastAction->goal->_position.x << " " << tree->lastAction->goal->_position.y << "Dis bet: " << distancelength(charmove->_position, tree->lastAction->goal->_position) << "\n";
    }
    charmove->updateData = arrive.calculateAcceleration(charmove, tree->lastAction->goal);
    std::cout << "moving: " << charmove->updateData.linear.x << " " << charmove->updateData.linear.y << " following path\n";

    charmove->updateData.angular = align.calculateAcceleration(charmove, tree->lastAction->goal).angular;
}

void character::updateChar(float time)
{
    if (path->returnedpath.size() == path->idx)
    {
        path->returnedpath.clear();
    }
    newBehavior(time);
    charmove->update(time);
}