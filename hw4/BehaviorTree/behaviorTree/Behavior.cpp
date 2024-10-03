
#include "Behavior.h"
#define SIGHT 15
void printStats(BehaviorTree *behaviorTree, std::string name, Status status)
{
    // std::cout<<name<<"\n";
    int i = 0;
    // printf("%d\n", i++); // 0
    int pathlenP = 0;
    int pathlenFm = 0;
    int pathlenFp = 0;
    gameTile *mg = behaviorTree->game->getTile(behaviorTree->mob->_position);
    // printf("%d\n", i++); // 1

    gameTile *pg = behaviorTree->game->getTile(behaviorTree->player->_position);
    // printf("%d\n", i++); // 2

    gameTile *fg;
    int fgid;
    gameTile *fg2;
    int fg2id;

    Path k = Path();
    if (mg->id != pg->id)
    {
        // std::cout<<mg->id<<" "<<pg->id<<"\n";
        k.makePath(behaviorTree->mob, pg, behaviorTree->game);
        // printf("%d\n", i++); // 3

        pathlenP = k.returnedpath.size();
        // printf("%d\n", i++); // 4
    }
    float distofruitM;
    float distofruitP;
    // std::cout<<behaviorTree->game->fruitList.empty()<<"\n";
    if (!behaviorTree->game->fruitList.empty() && behaviorTree->game->fruitList.size() > 0)
    {
        /**std::cout<<behaviorTree->game->fruitList.size()<<"\n";
        for(fruit *f: behaviorTree->game->fruitList){
            std::cout<<"fruit "<<f->coords.x<<","<<f->coords.y<<" "<<f->location->id<<"\n";
        }*/
        behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
        // printf("%d\n", i++);

        fg = behaviorTree->game->getTile(behaviorTree->game->goals.top().second->coords);
        // printf("%d\n", i++);

        if (fg->id != mg->id)
        {
            k.makePath(behaviorTree->mob, fg, behaviorTree->game);
            // printf("%d\n", i++);

            pathlenFm = k.returnedpath.size();
            // printf("%d\n", i++);
        }
        distofruitM = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position);
        // printf("%d\n", i++);

        behaviorTree->game->updategoals(behaviorTree->player->_position, 0);
        // printf("%d\n", i++);

        fg2 = behaviorTree->game->getTile(behaviorTree->game->goals.top().second->coords);
        // printf("%d\n", i++);

        if (fg2->id != mg->id)
        {
            k.makePath(behaviorTree->mob, fg2, behaviorTree->game);
            // printf("%d\n", i++);

            pathlenFp = k.returnedpath.size();
            // printf("%d\n", i++);
        }
        distofruitP = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->player->_position);
        // printf("%d\n", i++);
        fgid = fg->id;
        fg2id = fg2->id;
    }
    else
    {
        fgid = -99;
        fg2id = -99;
        distofruitM = 999999;
        distofruitP = 999999;
    }
    if (behaviorTree->game->test == false)
    {

        std::cout << behaviorTree->game->GameId << "," << behaviorTree->time << "," << behaviorTree->player->isEating << "," << behaviorTree->player->beenhit << "," << behaviorTree->player->dead << "," << behaviorTree->player->dancing
                  << "," << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "," << behaviorTree->player->_velocity.x << ", " << behaviorTree->player->_velocity.y << "," << behaviorTree->player->_rotation << "," << behaviorTree->mob->_position.x << ","
                  << behaviorTree->mob->_position.y << "," << behaviorTree->mob->_velocity.x << "," << behaviorTree->mob->_velocity.y << "," << behaviorTree->mob->_rotation << "," << behaviorTree->game->fruitList.size() << "," << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position)
                  << "," << behaviorTree->mob->isEating << "," << behaviorTree->mob->dancing << "," << pathlenP << "," << pathlenFm << "," << pathlenFp << "," << fgid << "," << fg2id << "," << mg->id << "," << pg->id << "," << distofruitM << "," << distofruitP << "," << status << "," << name <<","<<length(behaviorTree->mob->_velocity)<<","<<length(behaviorTree->player->_velocity)<< "\n";
    }
}

BehaviorTree::BehaviorTree(GameWorld *game, Kinematic *mob, Kinematic *goal, Kinematic *player)
{
    this->t.restart();
    this->game = game;
    this->mob = mob;
    this->player = player;
    this->goal = goal;
    this->arrive = Arrive();
    this->align = Align();
    this->dance = Dance();
    this->eatFruit = EatFruit();

    PlayerVisCondition *pVis = new PlayerVisCondition();
    pVis->behaviorTree = this;
    pVis->strName = std::string("PlayerVisCondition");

    PlayerDeadCondition *pDead = new PlayerDeadCondition();
    pDead->behaviorTree = this;
    pDead->strName = std::string("PlayerDeadCondition");

    Invert *invert = new Invert(pDead);
    invert->strName = std::string("inverter");

    PlayerHitAction *pHit = new PlayerHitAction();
    pHit->behaviorTree = this;
    pHit->strName = std::string("PlayerHitAction");

    MobDanceAction *mDance = new MobDanceAction();
    mDance->behaviorTree = this;
    mDance->strName = std::string("MobDanceAction");

    MoveToPlayerAction *mToP = new MoveToPlayerAction();
    mToP->behaviorTree = this;
    mToP->strName = std::string("MoveToPlayerAction");

    FruitVisCondition *fVis = new FruitVisCondition();
    fVis->behaviorTree = this;
    fVis->strName = std::string("FruitVisCondition");

    MoveToFruitAction *mToF = new MoveToFruitAction();
    mToF->behaviorTree = this;
    mToF->strName = std::string("MoveToFruitAction");

    SmashFruitAction *sMA = new SmashFruitAction();
    sMA->behaviorTree = this;
    sMA->strName = std::string("SmashFruitAction");

    MobEat *mEat = new MobEat();
    mEat->behaviorTree = this;
    mEat->strName = std::string("MobEat");

    MobWait *mWait = new MobWait();
    mWait->behaviorTree = this;
    mWait->strName = std::string("MobWait");

    MobPatrol *mPatrol = new MobPatrol();
    mPatrol->behaviorTree = this;
    mPatrol->strName = std::string("MobPatrol");

    Sequence *hitSeq = new Sequence();
    hitSeq->strName = std::string("HitPlayerSeq");
    hitSeq->addChild(invert);
    hitSeq->addChild(pHit);

    Repeat *hitRep = new Repeat(hitSeq);
    hitRep->strName = std::string("Repeater for Hit");
    hitRep->setCount(2);

    Sequence *wholeHit = new Sequence();
    wholeHit->addChild(pVis);
    wholeHit->addChild(mToP);
    wholeHit->addChild(hitRep);
    wholeHit->addChild(mDance);
    wholeHit->strName = std::string("Entire Hit Seq");

    NonDeterministicSelector *nonSel = new NonDeterministicSelector();
    nonSel->addChild(sMA);
    nonSel->addChild(mEat);
    nonSel->strName = std::string("NonDeterministicSelector");

    Sequence *fruitAct = new Sequence();
    fruitAct->addChild(fVis);
    fruitAct->addChild(mToF);
    fruitAct->addChild(nonSel);
    fruitAct->strName = std::string("fruitAct");

    Selector *fruitSelector = new Selector();
    fruitSelector->addChild(fruitAct);
    fruitSelector->addChild(mPatrol);
    fruitSelector->strName = std::string("fruitSelector");

    Selector *roottobe = new Selector();
    roottobe->addChild(wholeHit);
    roottobe->addChild(fruitSelector);

    roottobe->strName = std::string("root");
    root = roottobe;
}
void Invert::onTerminate(Status s)
{
}
void Invert::onInitialize()
{
}
Status Invert::update()
{
    m_pChild->tick();
    if (m_pChild->getStatus() == BH_FAILURE)
    {
        return BH_SUCCESS;
    }
    if (m_pChild->getStatus() == BH_SUCCESS)
    {
        return BH_FAILURE;
    }
    else
        return m_pChild->getStatus();
}
void NonDeterministicSelector::onTerminate(Status s)
{
}
void NonDeterministicSelector::onInitialize()
{
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(m_Children), std::end(m_Children), rng);
    m_Current = m_Children.begin();
}
void BehaviorTree::tick(float time)
{
    this->time = time;
    root->tick();
}

Status Behavior::tick()
{
    // std::cout << this->strName << "\n";
    if (m_eStatus != BH_RUNNING)
    {
        onInitialize();
    }

    m_eStatus = update();

    if (m_eStatus != BH_RUNNING)
    {
        onTerminate(m_eStatus);
    }

    // std::cout << this->strName << " " << m_eStatus << "\n";
    return m_eStatus;
}

void Behavior::reset()
{
    m_eStatus = BH_INVALID;
}

void Behavior::abort()
{
    onTerminate(BH_ABORTED);
    m_eStatus = BH_ABORTED;
}

bool Behavior::isTerminated() const
{
    return m_eStatus == BH_SUCCESS || m_eStatus == BH_FAILURE;
}

bool Behavior::isRunning() const
{
    return m_eStatus == BH_RUNNING;
}

Status Behavior::getStatus() const
{
    return m_eStatus;
}
void Repeat::setCount(int count)
{
    m_iLimit = count;
}
void Repeat::onTerminate(Status s)
{
    //
    onInitialize();
}

void Repeat::onInitialize()
{
    m_iCounter = 0;
}

Status Repeat::update()
{
    for (;;)
    {
        m_pChild->tick();
        if (m_pChild->getStatus() == BH_RUNNING)
            break;
        if (m_pChild->getStatus() == BH_FAILURE)
            return BH_FAILURE;
        if (++m_iCounter == m_iLimit)
            return BH_SUCCESS;
        m_pChild->reset();
    }
    return BH_INVALID;
}

void Sequence::onInitialize()
{
    m_CurrentChild = m_Children.begin();
}
void Sequence::onTerminate(Status s)
{
    // onInitialize();
}

Status Sequence::update()
{
    // Keep going until a child behavior says it's running.
    for (;;)
    {
        Status s = (*m_CurrentChild)->tick();

        // If the child fails, or keeps running, do the same.
        if (s != BH_SUCCESS)
        {
            return s;
        }

        // Hit the end of the array, job done!
        if (++m_CurrentChild == m_Children.end())
        {
            return BH_SUCCESS;
        }
    }
}

void Selector::onInitialize()
{

    m_Current = m_Children.begin();
}
void Selector::onTerminate(Status s)
{
    // onInitialize();
}
Status Selector::update()
{
    // Keep going until a child behavior says its running.
    for (;;)
    {
        Status s = (*m_Current)->tick();

        // If the child succeeds, or keeps running, do the same.
        if (s != BH_FAILURE)
        {
            return s;
        }

        // Hit the end of the array, it didn't end well...
        if (++m_Current == m_Children.end())
        {
            return BH_FAILURE;
        }
    }
}

void MoveToPlayerAction::onInitialize()
{
    reset();
    path = new Path();
}
void MoveToPlayerAction::onTerminate(Status s)
{
    // onInitialize();
}

void PlayerDeadCondition::onInitialize()
{
    reset();
}
void PlayerDeadCondition::onTerminate(Status s)
{
    // onInitialize();
}
Status PlayerDeadCondition::update()
{
    if (behaviorTree->player->dead)
    {
        Status status = BH_SUCCESS;
        printStats(behaviorTree, this->strName, status);
        return status;
    }
    else
    {
        Status status = BH_FAILURE;

        // std::cout << behaviorTree->t.getElapsedTime().asSeconds() << "," << this->strName << "," << status << ",(" << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "),(" << behaviorTree->mob->_position.x << "," << behaviorTree->player->_position.y << ")," << behaviorTree->game->fruitList.size() << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << "," << behaviorTree->game->fruitList.size() << "," << distancelength(behaviorTree->mob->_position, behaviorTree->player->_position) << "\n";
        return status;
    }
}
Status PlayerVisCondition::update()
{
    Path *path = new Path;
    gameTile *goal = behaviorTree->game->getTile(behaviorTree->player->_position);
    gameTile *start = behaviorTree->game->getTile(behaviorTree->mob->_position);
    // std::cout << "playerVis: player:" << behaviorTree->player->_position.x << ", " << behaviorTree->player->_position.y << "mob:" << behaviorTree->mob->_position.x << ", " << behaviorTree->mob->_position.y << "\n";
    path->makePath(behaviorTree->mob, goal, behaviorTree->game);
    prettyprintRef(path->returnedpath);
    if (start->getID() == goal->getID())
    {
        Status status = BH_SUCCESS;

        float distofruitM;
        float distofruitP;
        if (behaviorTree->game->fruitList.size() > 0)
        {
            behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
            distofruitM = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position);
            behaviorTree->game->updategoals(behaviorTree->player->_position, 0);
            distofruitP = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->player->_position);
        }
        else
        {
            distofruitM = 999999;
            distofruitP = 999999;
        }
        // std::cout << this->strName << "," << behaviorTree->game->GameId << "," << status << "," << behaviorTree->t.getElapsedTime().asSeconds() << "," << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "," << behaviorTree->mob->_position.x << "," << behaviorTree->mob->_position.y << "," << behaviorTree->game->fruitList.size() << "," << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << "," << distofruitM << "," << distofruitP << "," << behaviorTree->player->_satisfied << "," << behaviorTree->mob->_satisfied << "," << behaviorTree->player->isEating << "," << behaviorTree->mob->isEating << "," << behaviorTree->player->dancing << "," << behaviorTree->mob->dancing << "\n";

        return status;
    }
    if (!path->returnedpath.empty() && path->returnedpath.size() < SIGHT)
    {
        Status status = BH_SUCCESS;
        float distofruitM;
        float distofruitP;
        if (behaviorTree->game->fruitList.size() > 0)
        {
            behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
            distofruitM = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position);
            behaviorTree->game->updategoals(behaviorTree->player->_position, 0);
            distofruitP = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->player->_position);
        }
        else
        {
            distofruitM = 999999;
            distofruitP = 999999;
        }

        return status;
    }
    else
    {
        Status status = BH_FAILURE;
        // std::cout << behaviorTree->t.getElapsedTime().asSeconds() << "," << this->strName << "," << status << ",(" << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "),(" << behaviorTree->mob->_position.x << "," << behaviorTree->player->_position.y << ")," << behaviorTree->game->fruitList.size() << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << "," << behaviorTree->game->fruitList.size() << "," << distancelength(behaviorTree->mob->_position, behaviorTree->player->_position) << "\n";

        return status;
    }
}

void PlayerVisCondition::onTerminate(Status s)
{
    // onInitialize();
}
void PlayerHitAction::onTerminate(Status s)
{
    onInitialize();
}
Status PlayerHitAction::update()
{
    // std::cout << "Trying to hit has he been hit yet?" << behaviorTree->player->beenhit << "\n";
    // printf("hit\n");
    sf::Vector2f goal = behaviorTree->player->_position;

    sf::Vector2f start = behaviorTree->mob->_position;
    float d = distancelength(start, goal);
    // std::cout << "Distance" << d << "\n";
    if (inRadiusDecel(d))
    {
        if (behaviorTree->player->beenhit)
        {

            behaviorTree->player->dead = true;
            behaviorTree->player->isVisible = false;
            Status status = BH_SUCCESS;

            printStats(behaviorTree, this->strName, status);
            return status;
        }
        else
        {
            behaviorTree->player->beenhit = true;
            Status status = BH_SUCCESS;
            for (int i = 0; i < 10 < i; i++)
            {
                printStats(behaviorTree, this->strName, status);
            }
            return status;
        }
    }
    else
    {
        Status status = BH_FAILURE;
        printStats(behaviorTree, this->strName, status);
        return status;
    }
}

void MobDanceAction::onTerminate(Status s)
{
    // onInitialize();
}
Status MobDanceAction::update()
{
    // printf("in dance %d %d %f\n", behaviorTree->player->dead, this->getStatus(), behaviorTree->time);
    if (behaviorTree->player->dead)
    {
        Status status;
        if (this->getStatus() == BH_INVALID && behaviorTree->mob->dancing == 0)
        {
            // printf(" start dance\n");
            Dance d = Dance();
            behaviorTree->mob->updateData = d.calculateAcceleration(behaviorTree->mob, behaviorTree->mob);
            behaviorTree->mob->update(behaviorTree->time);
            behaviorTree->mob->dancing = 1.0f;

            status = BH_RUNNING;
            printStats(behaviorTree, this->strName, status);
            return status;
        }
        else if (this->getStatus() == BH_RUNNING)
        {
            // printf("update dance \n");
            if (behaviorTree->mob->dancing > 0)
            {
                Dance d = Dance();
                behaviorTree->mob->updateData = d.calculateAcceleration(behaviorTree->mob, behaviorTree->mob);
                behaviorTree->mob->update(behaviorTree->time);
                status = BH_RUNNING;
                printStats(behaviorTree, this->strName, status);
                return status;
            }
            else
            {
                status = BH_SUCCESS;
                printStats(behaviorTree, this->strName, status);
                return status;
            }
        }
    }
    Status status = BH_FAILURE;
    printStats(behaviorTree, this->strName, status);

    return status;
}
void FruitVisCondition::onTerminate(Status s)
{
    // onInitialize();
}
Status FruitVisCondition::update()
{
    if (behaviorTree->game->fruitList.empty())
    {
        Status status = BH_FAILURE;
        // std::cout << behaviorTree->t.getElapsedTime().asSeconds() << "," << this->strName << "," << status << ",(" << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "),(" << behaviorTree->mob->_position.x << "," << behaviorTree->player->_position.y << ")," << behaviorTree->game->fruitList.size() << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << "," << behaviorTree->game->fruitList.size() << "," << distancelength(behaviorTree->mob->_position, behaviorTree->player->_position) << "\n";

        return status;
    }
    behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
    Path *p = new Path();
    while (!behaviorTree->game->goals.empty())
    {
        // std::cout << ("In Fruit Vis Task\n");
        gameTile *goal = behaviorTree->game->goals.top().second->location;
        gameTile *start = behaviorTree->game->getTile(behaviorTree->mob->_position);
        if (start->getID() == goal->getID())
        {
            Status status = BH_SUCCESS;
            float distofruitM;
            float distofruitP;
            if (behaviorTree->game->fruitList.size() > 0)
            {
                behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
                distofruitM = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position);
                behaviorTree->game->updategoals(behaviorTree->player->_position, 0);
                distofruitP = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->player->_position);
            }
            else
            {
                distofruitM = 999999;
                distofruitP = 999999;
            }
            // std::cout << this->strName << "," << behaviorTree->game->GameId << "," << status << "," << behaviorTree->t.getElapsedTime().asSeconds() << "," << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "," << behaviorTree->mob->_position.x << "," << behaviorTree->mob->_position.y << "," << behaviorTree->game->fruitList.size() << "," << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << "," << distofruitM << "," << distofruitP << "," << behaviorTree->player->_satisfied << "," << behaviorTree->mob->_satisfied << "," << behaviorTree->player->isEating << "," << behaviorTree->mob->isEating << "," << behaviorTree->player->dancing << "," << behaviorTree->mob->dancing << "\n";

            return status;
        }
        p->makePath(behaviorTree->mob, goal, behaviorTree->game);
        if (!p->returnedpath.empty() && p->returnedpath.size() < SIGHT)
        {
            Status status = BH_SUCCESS;
            float distofruitM;
            float distofruitP;
            if (behaviorTree->game->fruitList.size() > 0)
            {
                behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
                distofruitM = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position);
                behaviorTree->game->updategoals(behaviorTree->player->_position, 0);
                distofruitP = distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->player->_position);
            }
            else
            {
                distofruitM = 999999;
                distofruitP = 999999;
            }
            // std::cout << this->strName << "," << behaviorTree->game->GameId << "," << status << "," << behaviorTree->t.getElapsedTime().asSeconds() << "," << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "," << behaviorTree->mob->_position.x << "," << behaviorTree->mob->_position.y << "," << behaviorTree->game->fruitList.size() << "," << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << "," << distofruitM << "," << distofruitP << "," << behaviorTree->player->_satisfied << "," << behaviorTree->mob->_satisfied << "," << behaviorTree->player->isEating << "," << behaviorTree->mob->isEating << "," << behaviorTree->player->dancing << "," << behaviorTree->mob->dancing << "\n";

            return status;
        }
        behaviorTree->game->goals.pop();
    }
    Status status = BH_FAILURE;
    // std::cout << behaviorTree->t.getElapsedTime().asSeconds() << "," << this->strName << "," << status << ",(" << behaviorTree->player->_position.x << "," << behaviorTree->player->_position.y << "),(" << behaviorTree->mob->_position.x << "," << behaviorTree->player->_position.y << ")," << behaviorTree->game->fruitList.size() << "\n";

    return status;
}
void MoveToFruitAction::onInitialize()
{
    path = new Path();
    reset();
}
void MoveToFruitAction::onTerminate(Status s)
{
    // onInitialize();
}

Status MoveToFruitAction::update()
{
    // std::cout << "Move to fruit action \n";
    Path p2 = Path();
    gameTile *goal = behaviorTree->game->getTile(behaviorTree->player->_position);
    gameTile *start = behaviorTree->game->getTile(behaviorTree->mob->_position);
    p2.makePath(behaviorTree->mob, goal, behaviorTree->game);

    if (!p2.returnedpath.empty() && p2.returnedpath.size() < SIGHT)
    {
        Status status = BH_FAILURE; // found the player please do that instead
        printStats(behaviorTree, this->strName, status);

        return status;
    }
    // std::cout << "Move to fruit action dist:" << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << " \n";
    behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
    if (behaviorTree->game->goals.size() == 0)
    {
        Status status = BH_FAILURE; // somehow there is no fruit
        printStats(behaviorTree, this->strName, status);

        return status;
    }
    behaviorTree->mob->targetFruit = behaviorTree->game->goals.top().second;

    goal = behaviorTree->game->getTile(behaviorTree->mob->targetFruit->coords);
    start = behaviorTree->game->getTile(behaviorTree->mob->_position);
    behaviorTree->goal->_position = behaviorTree->mob->targetFruit->coords;

    if (start->getID() == goal->getID())
    {

        behaviorTree->mob->updateData = behaviorTree->arrive.calculateAcceleration(behaviorTree->mob, behaviorTree->goal);
        behaviorTree->mob->updateData.angular = behaviorTree->align.calculateAcceleration(behaviorTree->mob, behaviorTree->goal).angular;
        behaviorTree->mob->update(behaviorTree->time);
        Status status;

        status = BH_SUCCESS;

        printStats(behaviorTree, this->strName, status);
        return status;
    }

    path->makePath(behaviorTree->mob, goal, behaviorTree->game);
    behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));
    // std::cout << "Move to fruit action path len: " << path->returnedpath.size() << " index " << path->idx << " Is satisfied at that index? " << behaviorTree->mob->_satisfied << " \n";

    if (!path->returnedpath.empty() && path->returnedpath.size() < SIGHT)
    {

        behaviorTree->goal->_position = path->returnedpath.at(path->idx)->coords;
        behaviorTree->goal->_position.x += 10.0f;
        behaviorTree->goal->_position.y += 10.0f;
        behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));
        if (behaviorTree->mob->_satisfied)
        {
            if (path->returnedpath.size() > path->idx + 1 && behaviorTree->mob->_satisfied)
            {

                path->idx++;
                behaviorTree->goal->_position = path->returnedpath.at(path->idx)->coords;
                behaviorTree->goal->_position.x += 10.0f;
                behaviorTree->goal->_position.y += 10.0f;
                behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));
            }
        }

        behaviorTree->mob->updateData = behaviorTree->arrive.calculateAcceleration(behaviorTree->mob, behaviorTree->goal);
        behaviorTree->mob->updateData.angular = behaviorTree->align.calculateAcceleration(behaviorTree->mob, behaviorTree->goal).angular;
        behaviorTree->mob->update(behaviorTree->time);
        Status status = BH_RUNNING;

        if (inRadiusSat(distancelength(behaviorTree->mob->_position, behaviorTree->mob->targetFruit->coords)))
        {
            status = BH_SUCCESS;
        }
        printStats(behaviorTree, this->strName, status);
        return status;
    }
    else
    {
        Status status = BH_FAILURE;
        printStats(behaviorTree, this->strName, status);
        return status;
    }
}
void SmashFruitAction::onTerminate(Status s)
{
    // onInitialize();
}
Status SmashFruitAction::update()
{
    behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
    if (inRadiusSat(distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position)))
    {
        behaviorTree->game->goals.top().second->smash();
        behaviorTree->game->goals.pop();
        Status status = BH_SUCCESS;
        for (int i = 0; i < 10; i++)
        {
            printStats(behaviorTree, this->strName, status);
        }
        return status;
    }
    Status status = BH_FAILURE;
    printStats(behaviorTree, this->strName, status);
    return status;
}

void MobEat::onTerminate(Status s)
{
    // onInitialize();
}
Status MobEat::update()
{
    behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
    if (this->getStatus() == BH_INVALID || this->getStatus() == BH_FAILURE)
    {
        if (inRadiusSat(distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position)))
        {
            EatFruit e = EatFruit();
            behaviorTree->mob->targetFruit = behaviorTree->game->goals.top().second;
            behaviorTree->mob->updateData = e.calculateAcceleration(behaviorTree->mob, behaviorTree->mob);
            behaviorTree->mob->update(behaviorTree->time);
            Status status = BH_RUNNING;
            for (int i = 0; i < 10; i++)
            {
                printStats(behaviorTree, this->strName, status);
            }
            return status;
        }
        else
        {
            Status status = BH_FAILURE;
            printStats(behaviorTree, this->strName, status);
            return status;
        }
    }
    else
    {
        if (behaviorTree->mob->isEating && behaviorTree->mob->targetFruit->frame.left < 900)
        {

            behaviorTree->mob->update(behaviorTree->time);
            Status status = BH_RUNNING;
            for (int i = 0; i < 10; i++)
            {
                printStats(behaviorTree, this->strName, status);
            }
            return status;
        }
        else if (behaviorTree->mob->isEating)
        {
            behaviorTree->mob->isEating = false;
            int i = 0;
            int offset;

            behaviorTree->game->fruitList.remove(behaviorTree->mob->targetFruit);
            behaviorTree->mob->doneEating = true;
            // std::cout << "done eating\n";
            Status status = BH_SUCCESS;
            for (int i = 0; i < 10; i++)
            {
                printStats(behaviorTree, this->strName, status);
            }
            return status;
        }
    }
    Status status = BH_FAILURE;
    printStats(behaviorTree, this->strName, status);
    return status;
}
void MobWait::onTerminate(Status s)
{
    // onInitialize();
}
void MobWait::onInitialize()
{
    count = 10;
}
Status MobWait::update()
{
    behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
    if (this->getStatus() == BH_RUNNING)
    {
        if (inRadiusDecel(distancelength(behaviorTree->player->_position, behaviorTree->mob->_position)))
        {
            Status status = BH_SUCCESS;
            printStats(behaviorTree, this->strName, status);

            return status;
        }
    }
    if (inRadiusSat(distancelength(behaviorTree->game->goals.top().second->coords, behaviorTree->mob->_position)))
    {
        if (this->count > 0)
        {
            this->count -= behaviorTree->time;
            behaviorTree->mob->_velocity = sf::Vector2f(0, 0);
            behaviorTree->mob->_rotation = 0;
            Status status = BH_RUNNING;
            printStats(behaviorTree, this->strName, status);

            return status;
        }
        else
        {
            Status status = BH_FAILURE;
            printStats(behaviorTree, this->strName, status);
            return status;
        }
    }
    else
    {
        Status status = BH_INVALID;
        printStats(behaviorTree, this->strName, status);
        return status;
    }
}
typedef std::pair<float, gameTile *> piRef2;
void MobPatrol::onTerminate(Status s)
{
    // onInitialize();
}

void MobPatrol::onInitialize()
{
    path = new Path();
    corners = std::list<gameTile *>();
    corners.emplace_back(behaviorTree->game->getTile(sf::Vector2f(0, 0)));
    corners.emplace_back(behaviorTree->game->getTile(sf::Vector2f(630, 630)));
    corners.emplace_back(behaviorTree->game->getTile(sf::Vector2f(0, 630)));
    corners.emplace_back(behaviorTree->game->getTile(sf::Vector2f(630, 0)));
    last = behaviorTree->game->getTile(behaviorTree->mob->_position);
    gameTile *current = last;
    float d = 999.0f;
    for (gameTile *g : corners)
    {
        if (distancelength(current->coords, g->coords) < d)
        {
            last = g;
            d = distancelength(current->coords, g->coords);
        }
    }
    corners.remove(last);
    corners.emplace_back(last); // move the "last visited tile to the back"
}

Status MobPatrol::update()
{
    behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
    // std::cout << "Patrol dis to player: " << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << "\n";
    gameTile *goal2 = behaviorTree->game->getTile(behaviorTree->player->_position);

    Path *pplayer = new Path();
    pplayer->makePath(behaviorTree->mob, goal2, behaviorTree->game);

    // std::cout << "made a path yall " << pplayer->returnedpath.size() << "\n";
    if ((!pplayer->returnedpath.empty() && pplayer->returnedpath.size() < SIGHT))
    {
        Status status = BH_FAILURE;
        printStats(behaviorTree, this->strName, status);
        return status;
    }
    if (!behaviorTree->game->fruitList.empty())
    {
        goal2 = behaviorTree->game->getTile(behaviorTree->game->goals.top().second->coords);
        pplayer->makePath(behaviorTree->mob, goal2, behaviorTree->game);

        if ((!pplayer->returnedpath.empty() && pplayer->returnedpath.size() < SIGHT))
        {
            Status status = BH_FAILURE;
            printStats(behaviorTree, this->strName, status);
            return status;
        }
    }
    // std::cout << "past success 1\n";
    //   behaviorTree->game->updategoals(behaviorTree->mob->_position, 0);
    if (this->getStatus() == BH_RUNNING)
    {
        // std::cout << "PathSize " << path->returnedpath.size() << "\n";

        if (!path->returnedpath.empty() && path->idx < path->returnedpath.size())
        {
            behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));

            if (behaviorTree->mob->_satisfied)
            {
                if (path->returnedpath.size() > path->idx + 1)
                {
                    behaviorTree->goal->_position = path->returnedpath.at(path->idx++)->coords;
                }
                else
                {
                    Status status = BH_SUCCESS;
                    printStats(behaviorTree, this->strName, status);
                    return status;
                }
            }

            else
            {
                behaviorTree->goal->_position = path->returnedpath.at(path->idx)->coords;
            }
            // std::cout << "dis to goal " << distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords) << "\n";
            behaviorTree->mob->updateData = behaviorTree->arrive.calculateAcceleration(behaviorTree->mob, behaviorTree->goal);
            behaviorTree->mob->updateData.angular = behaviorTree->align.calculateAcceleration(behaviorTree->mob, behaviorTree->goal).angular;
            behaviorTree->mob->update(behaviorTree->time);
            // std::cout << "\ndis to goal " << distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords) << "\n";

            // std::cout << "\nWe should be updating\n";
            Status status = BH_RUNNING;
            printStats(behaviorTree, this->strName, status);
            return status;
        }

        else
        {
            Status status = BH_FAILURE;
            printStats(behaviorTree, this->strName, status);
            return status;
        }
    }
    // std::cout << ("Patrol? \n");
    gameTile *goal = corners.front();
    gameTile *start = behaviorTree->game->getTile(behaviorTree->mob->_position);
    while (start->getID() == goal->getID())
    {
        corners.remove(goal);
        corners.emplace_back(goal);
        goal = corners.front();
    }
    path->makePath(behaviorTree->mob, goal, behaviorTree->game);
    if (!path->returnedpath.empty())
    {
        behaviorTree->goal->_position = path->returnedpath.at(path->idx++)->coords;

        behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));

        if (behaviorTree->mob->_satisfied)
        {
            behaviorTree->goal->_position = path->returnedpath.at(path->idx++)->coords;
        }
        // std::cout << "dis to goal" << distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords) << "\n";

        behaviorTree->mob->updateData = behaviorTree->arrive.calculateAcceleration(behaviorTree->mob, behaviorTree->goal);
        behaviorTree->mob->updateData.angular = behaviorTree->align.calculateAcceleration(behaviorTree->mob, behaviorTree->goal).angular;
        behaviorTree->mob->update(behaviorTree->time);
        // std::cout << "dis to goal" << distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords) << "\n";

        // std::cout << "Still should have updated\n";
        Status status = BH_RUNNING;
        printStats(behaviorTree, this->strName, status);
        return status;
    }

    Status status = BH_FAILURE;
 printStats(behaviorTree, this->strName, status);
    return status;
}

Status MoveToPlayerAction::update()
{
    // std::cout << "Move to player action \n";
    if (inRadiusSat(distancelength(behaviorTree->player->_position, behaviorTree->mob->_position)))
    {
        Status status = BH_SUCCESS;

        printStats(behaviorTree, this->strName, status);
        return status;
    }
    // std::cout << "Move to player action dist:" << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << " \n";

    gameTile *goal = behaviorTree->game->getTile(behaviorTree->player->_position);
    gameTile *start = behaviorTree->game->getTile(behaviorTree->mob->_position);
    if (start->getID() == goal->getID())
    {
        behaviorTree->mob->updateData = behaviorTree->arrive.calculateAcceleration(behaviorTree->mob, behaviorTree->player);
        behaviorTree->mob->updateData.angular = behaviorTree->align.calculateAcceleration(behaviorTree->mob, behaviorTree->player).angular;
        behaviorTree->mob->update(behaviorTree->time);
        Status status = BH_SUCCESS;
        printStats(behaviorTree, this->strName, status);
        return status;
    }
    path->makePath(behaviorTree->mob, goal, behaviorTree->game);
    behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));
    // std::cout << "Move to player action path len: " << path->returnedpath.size() << " index " << path->idx << " Is satisfied at that index? " << behaviorTree->mob->_satisfied << " \n";

    if (!path->returnedpath.empty() && path->returnedpath.size() < SIGHT)
    {
        // std::cout << "move 1\n";
        behaviorTree->goal->_position = path->returnedpath.at(path->idx)->coords;
        behaviorTree->goal->_position.x += 10.0f;
        behaviorTree->goal->_position.y += 10.0f;
        behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));
        // std::cout << "move 2" << behaviorTree->mob->_satisfied << "\n";

        if (behaviorTree->mob->_satisfied)
        {
            // std::cout << "move 3 " << path->returnedpath.size() << " " << path->idx << "\n";

            while (path->returnedpath.size() > path->idx + 1 && behaviorTree->mob->_satisfied)
            {
                // std::cout << "move 3 " << path->returnedpath.size() << " " << path->idx << "\n";

                path->idx += 1;
                behaviorTree->goal->_position = path->returnedpath.at(path->idx)->coords;
                behaviorTree->goal->_position.x += 10.0f;
                behaviorTree->goal->_position.y += 10.0f;
                behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));
                // std::cout << " Is satisfied at that index? " << behaviorTree->mob->_satisfied << " \n";
            }
            behaviorTree->mob->_satisfied = inRadiusSat(distancelength(behaviorTree->mob->_position, path->returnedpath.at(path->idx)->coords));

            if (path->returnedpath.size() == path->idx + 1 && behaviorTree->mob->_satisfied)
            {
                behaviorTree->mob->updateData = behaviorTree->arrive.calculateAcceleration(behaviorTree->mob, behaviorTree->player);
                behaviorTree->mob->updateData.angular = behaviorTree->align.calculateAcceleration(behaviorTree->mob, behaviorTree->player).angular;
                behaviorTree->mob->update(behaviorTree->time);
                Status status = BH_SUCCESS;
                printStats(behaviorTree, this->strName, status);
                return status;
            }
        }
        // std::cout << "Move to player action dist:" << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << " \n";

        behaviorTree->mob->updateData = behaviorTree->arrive.calculateAcceleration(behaviorTree->mob, behaviorTree->goal);
        behaviorTree->mob->updateData.angular = behaviorTree->align.calculateAcceleration(behaviorTree->mob, behaviorTree->goal).angular;
        behaviorTree->mob->update(behaviorTree->time);
        Status status = BH_SUCCESS;
        // std::cout << "Move to player action dist:" << distancelength(behaviorTree->player->_position, behaviorTree->mob->_position) << " \n";
        printStats(behaviorTree, this->strName, status);

        return status;
    }
    else
    {
        Status status = BH_FAILURE;
 printStats(behaviorTree, this->strName, status);
        return status;
    }
}

void SmashFruitAction::onInitialize()
{
    reset();
}
void FruitVisCondition::onInitialize()
{
    reset();
}
void MobDanceAction::onInitialize()
{
    reset();
}
void PlayerHitAction::onInitialize()
{
    reset();
}
void PlayerVisCondition::onInitialize()
{
    reset();
}
void MobEat::onInitialize()
{
    reset();
}
Status NonDeterministicSelector::update()
{

    // Keep going until a child behavior says it's running.
    for (;;)
    {

        Status s = (*m_Current)->tick();

        // If the child succeeds, or keeps running, do the same.
        if (s != BH_FAILURE)
        {
            return s;
        }

        // Hit the end of the array, it didn't end well...
        if (++m_Current == m_Children.end())
        {
            return BH_FAILURE;
        }
    }
}