#pragma once
#include "../character/kinematic.h"
#include "../World/gameWorld.h"
#include "../behavior/steeringbehavior.h"
#include "../utilities/util.h"
#include "asserter.h"
#include "../PathFinding/Path.h"




enum Status
/**
 * Return values of and valid states for behaviors.
 */
{
	BH_INVALID,
	BH_SUCCESS,
	BH_FAILURE,
	BH_RUNNING,
	BH_ABORTED,
};
typedef std::function<void(Status)> BehaviorObserver;

class Behavior
/**
 * Base class for actions, conditions and composites.
 */
{
public:
	std::string strName;
	virtual Status update() = 0;

	virtual void onInitialize() {}
	virtual void onTerminate(Status) {}

	Behavior()
		: m_eStatus(BH_INVALID)
	{
	}

	virtual ~Behavior()
	{
	}

	Status tick();
	void reset();
	void abort();
	bool isTerminated() const;
	bool isRunning() const;
	Status getStatus() const;

private:
	Status m_eStatus;
};

class BehaviorTree
{
public:
	sf::Clock t; // this is more a containor than anything else.
	float time;
	Arrive arrive;
	Align align;
	Dance dance;
	EatFruit eatFruit;
	Kinematic *goal;
	Kinematic *player;
	Kinematic *mob;
	GameWorld *game;
	Behavior *last;
	Behavior *root;
	BehaviorTree(GameWorld *game, Kinematic *mob, Kinematic *goal, Kinematic *player);
	void tick(float time);
	std::list<Behavior *> m_Behaviors;
};
class Decorator : public Behavior
{
protected:
	Behavior *m_pChild;

public:
	Decorator(Behavior *child) : m_pChild(child) {}
};
class Repeat : public Decorator
{
public:
	Repeat(Behavior *child)
		: Decorator(child)
	{
	}

	void setCount(int count);
	void onInitialize();
	void onTerminate(Status);
	Status update();

protected:
	int m_iLimit;
	int m_iCounter;
};
class Invert: public Decorator{
	public:
	Invert(Behavior * child):Decorator(child){}
	void onInitialize();
	void onTerminate(Status);
	Status update();
};
class Composite : public Behavior
{
public:
	void addChild(Behavior *child) { m_Children.push_back(child); }
	void removeChild(Behavior *);
	void clearChildren();

protected:
	typedef std::vector<Behavior *> Behaviors;
	Behaviors m_Children;
};
class Sequence : public Composite
{
protected:
	virtual ~Sequence()
	{
	}

	virtual void onInitialize();
	void onTerminate(Status);
	virtual Status update();
	Behaviors::iterator m_CurrentChild;
};
class Selector : public Composite
{
protected:
	virtual ~Selector()
	{
	}

	virtual void onInitialize();
	void onTerminate(Status);
	virtual Status update();
	Behaviors::iterator m_Current;
};
class NonDeterministicSelector :public Composite{

	protected:
	virtual ~NonDeterministicSelector()
	{
	}

	virtual void onInitialize();
	virtual Status update();
	void onTerminate(Status);
	Behaviors::iterator m_Current;
};



class MoveToPlayerAction : public Behavior
{
public:
	BehaviorTree *behaviorTree;
	Status update();
	Path *path;
	void onInitialize();
	void onTerminate(Status);

};
class PlayerDeadCondition : public Behavior
{
public:
	BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class PlayerVisCondition : public Behavior
{
public:
	BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class PlayerHitAction : public Behavior
{
public:
	BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class MobDanceAction : public Behavior
{
public:
	BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class FruitVisCondition : public Behavior
{
public:
	BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class MoveToFruitAction : public Behavior
{
public:
	BehaviorTree *behaviorTree;
	Status update();
	Path *path;
	void onInitialize();
	void onTerminate(Status);
};
class SmashFruitAction : public Behavior
{
public:
	BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class MobEat : public Behavior
{
public:
	BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class MobWait : public Behavior
{
public:
	BehaviorTree *behaviorTree;
	int count;
	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class MobPatrol : public Behavior
{
public:
	BehaviorTree *behaviorTree;
	std::list<gameTile *> corners;
	gameTile *last;
	Path *path;
	int indx;
	Status update();
	void onInitialize();
	void onTerminate(Status);
};
