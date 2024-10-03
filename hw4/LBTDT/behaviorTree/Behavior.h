#pragma once
#include "../character/kinematic.h"
#include "../World/gameWorld.h"
#include "../behavior/steeringbehavior.h"
#include "../utilities/util.h"
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
	BehaviorTree();
	BehaviorTree(GameWorld *game, Kinematic *mob, Kinematic *goal, Kinematic *player);
	void tick(float time);
	std::vector<Behavior *> m_Behaviors;
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
class Invert : public Decorator
{
public:
	Invert(Behavior *child) : Decorator(child) {}
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
public:
	virtual ~Selector()
	{
	}

	virtual void onInitialize();
	void onTerminate(Status);
	virtual Status update();
	Behaviors::iterator m_Current;
	BehaviorTree *behaviorTree;
};
class NonDeterministicSelector : public Composite
{

protected:
	virtual ~NonDeterministicSelector()
	{
	}

	virtual void onInitialize();
	virtual Status update();
	void onTerminate(Status);
	Behaviors::iterator m_Current;
};

class ActionBehavior : public Behavior
{
	public:
	BehaviorTree *behaviorTree;
	Status update();
	Path *path;
	void onInitialize();
	void onTerminate(Status);
};
class MoveToPlayerAction : public ActionBehavior
{
public:
	Status update();
	//Path *path;
	void onInitialize();
	void onTerminate(Status);
};
class PlayerDeadCondition : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class PlayerVisCondition : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class PlayerHitAction : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class MobDanceAction : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class FruitVisCondition : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class MoveToFruitAction : public ActionBehavior
{
public:
	Status update();
	//Path *path;
	void onInitialize();
	void onTerminate(Status);
};
class SmashFruitAction : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class MobEat : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;

	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class MobWait : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;
	int count;
	Status update();
	void onInitialize();
	void onTerminate(Status);
};
class MobPatrol : public ActionBehavior
{
public:
	//BehaviorTree *behaviorTree;
	std::list<gameTile *> corners;
	gameTile *last;
	//Path *path;
	int indx;
	Status update();
	void onInitialize();
	void onTerminate(Status);
};

class LearnedSelector : public Selector
{
public:
	Behavior *truebranch;
	Behavior *falsebranch;
	std::string attname;
	std::string type;

	Status Update();
};
class discLearnedSelector : public LearnedSelector
{
public:
	std::string div;
	Status update();
	void onInitialize();
};
class conLearnedSelector : public LearnedSelector
{
public:
	float div;
	Status update();
	void onInitialize();
};
class GameId : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class TimeSelector : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Pposx : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Pposy : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Mposx : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Mosy : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class pgid : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class DmtoP : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class MDistF : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class PDistF : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};

class Pvelx : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Pvely : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Mvelx : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Mvely : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class pathlenP : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class pathlenFm : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class pathlenFp : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class mgid : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class fgidid : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class fg2id : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class MDance : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class PDance : public conLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Meat : public discLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Peat : public discLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
class Fruits : public discLearnedSelector
{
public:
	void onInitialize();

	Status update();
};
