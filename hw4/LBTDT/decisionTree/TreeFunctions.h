#pragma once
#include "../character/kinematic.h"
#include "../World/gameWorld.h"


class TreeDecision{
    public: virtual bool makeDecision(Kinematic *cur, GameWorld *game)=0;
};

class FruitExist:public TreeDecision{
    public: bool makeDecision(Kinematic *cur, GameWorld *game);
};
class InCorner:public TreeDecision{
    public: bool makeDecision(Kinematic *cur, GameWorld *game);
};
class FruitVis: public TreeDecision{
    public: bool makeDecision(Kinematic *cur, GameWorld *game);
};
class DoneEating: public TreeDecision{
    public: bool makeDecision(Kinematic *cur, GameWorld *game);
};
class CanReachFruit:public TreeDecision{
    public: bool makeDecision(Kinematic *cur, GameWorld *game);
};

class ActionComplete:public TreeDecision{
    public: bool makeDecision(Kinematic *cur, GameWorld *game);
};

