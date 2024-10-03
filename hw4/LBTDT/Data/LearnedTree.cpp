#include "LearnedTree.h"
#include <string>

Behavior *LearnedTree::switcher(std::string s)
{
	if (s.compare("GameId") == 0)
	{
		return new GameId();
	}
	else if (s.compare("Time") == 0)
	{
		return new TimeSelector();
	}
	else if (s.compare("Pposx") == 0)
	{
		return new Pposx();
	}
	else if (s.compare("Pposy") == 0)
	{
		return new Pposy();
	}
	else if (s.compare("Mposx") == 0)
	{
		return new Mposx();
	}
	else if (s.compare("Mosy") == 0)
	{
		return new Mosy();
	}
	else if (s.compare("Fruits") == 0)
	{
		return new Fruits();
	}
	else if (s.compare("DmtoP") == 0)
	{
		return new DmtoP();
	}
	else if (s.compare("MDistF") == 0)
	{
		return new MDistF();
	}
	else if (s.compare("PDistF") == 0)
	{
		return new PDistF();
	}
	else if (s.compare("Peat") == 0)
	{
		return new Peat();
	}
	else if (s.compare("Meat") == 0)
	{
		return new Meat();
	}
	else if (s.compare("PDance") == 0)
	{
		return new PDance();
	}
	else if (s.compare("MDance") == 0)
	{
		return new MDance();
	}
	else if (s.compare("Pvelx") == 0)
	{
		return new Pvelx();
	}
	else if (s.compare("Pvely") == 0)
	{
		return new Pvely();
	}
	else if (s.compare("Mvelx") == 0)
	{
		return new Mvelx();
	}
	else if (s.compare("Mvely") == 0)
	{
		return new Mvely();
	}
	else if (s.compare("pathlenP") == 0)
	{
		return new pathlenP();
	}
	else if (s.compare("pathlenFm") == 0)
	{
		return new pathlenFm();
	}
	else if (s.compare("pathlenFp") == 0)
	{
		return new pathlenFp();
	}
	else if (s.compare("pgid") == 0)
	{
		return new pgid();
	}
	else if (s.compare("mgid") == 0)
	{
		return new mgid();
	}
	else if (s.compare("fgidid") == 0)
	{
		return new fgidid();
	}
	else if (s.compare("fg2id") == 0)
	{
		return new fg2id();
	}
	else if (s.compare("MobPatrol") == 0)
	{
		return new MobPatrol();
	}
	else if (s.compare("SmashFruitAction") == 0)
	{
		return new SmashFruitAction();
	}
	else if (s.compare("MoveToFruitAction") == 0)
	{
		return new MoveToFruitAction();
	}
	else if (s.compare("MobDanceAction") == 0)
	{
		return new MobDanceAction();
	}
	else if (s.compare("PlayerHitAction") == 0)

	{
		countHit++;
		return new PlayerHitAction();
	}
	else if (s.compare("MoveToPlayerAction") == 0)
	{
		return new MoveToPlayerAction();
	}
	else if (s.compare("MobEat") == 0)
	{
		return new MobEat();
	}

	//std::cout<<s<<"\n";
	//std::cout<<"OH NO\n\n";
	return NULL;
}
Behavior *LearnedTree::addBaby(LearningTreeDecisionNode *q)
{

	if (q->type.compare("continuous") == 0)
	{
		//std::cout<<"cont\n";
		ContAttrLearningTreeNode *n=(ContAttrLearningTreeNode *)q;
		conLearnedSelector *np = (conLearnedSelector *)(switcher(n->attr_name));
		
		//std::cout<<"attName: " <<n->name<<" val"<<n->attr_val<<" \n";

		np->div = atof(n->attr_val.c_str());
		np->strName.assign(n->attr_name);
		np->behaviorTree=this;

		//std::cout<<" AS A BEHAVIOR attName: " <<np->strName<<" val"<<np->div<<" \n";

		if (n->falseBranch != NULL && n->falseBranch->type.compare("leaf") != 0 && n->falseBranch->type.compare("null") != 0)
		{
			//std::cout<<"False branch of "<<"attName: " <<np->strName<<" val"<<np->div<<" true branch ";

			np->falsebranch = addBaby((LearningTreeDecisionNode *)n->falseBranch);

		}
		else if (n->falseBranch != NULL)
		{

			LearningTreeNodeLeaf *m = (LearningTreeNodeLeaf *)n->falseBranch;
			np->falsebranch=switcher(m->targetName);
			ActionBehavior* fb=(ActionBehavior*)np->falsebranch;
			fb->behaviorTree=this;
			np->falsebranch=(Behavior*)fb;
			//std::cout << "False Leaf child of " << np->strName << " " <<np->div<< " THIS IS THE VALUE I NEED\n" <<m->targetName<<"\n " <<  np->falsebranch->strName << " name " <<  n->falseBranch->name<< "\n";
		}

		if (n->trueBranch->type.compare("leaf") != 0 && n->trueBranch->type.compare("null") != 0)
		{
			//std::cout<<"True branch of "<<"attName: " <<np->strName<<" val"<<np->div<<" true branch ";
			np->truebranch = addBaby((LearningTreeDecisionNode *)n->trueBranch);
		}
		else
		{
			LearningTreeNodeLeaf *m = (LearningTreeNodeLeaf *)n->trueBranch;
			np->truebranch=switcher(m->targetName);
			ActionBehavior* fb=(ActionBehavior*)np->truebranch;
			fb->behaviorTree=this;
			np->truebranch=(Behavior*)fb;
			//std::cout << "true Leaf child of " << np->strName << " " <<np->div<< " THIS IS THE VALUE I NEED\n"<<m->targetName<<"\n " <<np->truebranch->strName << " name " <<  n->trueBranch->name <<  "\n";
		}
		return np;

		

			
		
	}
	else if (q->type.compare("discrete") == 0)
	{
		//std::cout<<"disc\n";
	
		DiscAttrLearningTreeNode *n=(DiscAttrLearningTreeNode *)q;
		discLearnedSelector *np = (discLearnedSelector *)(switcher(n->attr_name));
		
		//std::cout<<"attName: " <<n->name<<" val"<<n->attr_val<<" \n";

		np->div.assign(n->attr_val);
		np->strName.assign(n->attr_name);
		//std::cout<<" AS A BEHAVIOR attName: " <<np->strName<<" val"<<np->div<<" \n";
		if (n->falseBranch != NULL && n->falseBranch->type.compare("leaf") != 0 && n->falseBranch->type.compare("null") != 0)
		{
			np->falsebranch = addBaby((LearningTreeDecisionNode *)n->falseBranch);
		}
		else if (n->falseBranch != NULL)
		{
			LearningTreeNodeLeaf *m = (LearningTreeNodeLeaf *)n->falseBranch;
			np->truebranch=switcher(m->targetName);
			ActionBehavior* fb=(ActionBehavior*)np->falsebranch;
			fb->behaviorTree=this;
			np->truebranch=(Behavior*)fb;
			//std::cout << "False Leaf child of " << np->strName << " " <<np->div<< " THIS IS THE VALUE I NEED\n" <<m->targetName<<"\n " <<  np->falsebranch->strName << " name " <<  n->falseBranch->name<< "\n";
		}

		if (n->trueBranch->type.compare("leaf") != 0 && n->trueBranch->type.compare("null") != 0)
		{
			//std::cout << "True branch of " << "attName: " << n->attr_name << " val" << n->attr_val << " true branch ";
			np->truebranch = addBaby((LearningTreeDecisionNode *)n->trueBranch);
		}
		else
		{
			LearningTreeNodeLeaf *m = (LearningTreeNodeLeaf *)n->trueBranch;
			np->truebranch=switcher(m->targetName);
			ActionBehavior* fb=(ActionBehavior*)np->truebranch;
			fb->behaviorTree=this;
			np->truebranch=(Behavior*)fb;
			//std::cout << "True Leaf child of " << np->strName << " " <<np->div<< " THIS IS THE VALUE I NEED\n" <<m->targetName<<"\n " <<  np->truebranch->strName << " name " <<  n->trueBranch->name<< "\n";
		}
		return np;
	}
	//std::cout<<"OH NO BUT HIGHER!!\n";
	return NULL;
}
LearnedTree::LearnedTree(LearningTree *tree, GameWorld *game, Kinematic *mob, Kinematic *goal, Kinematic *player)
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
	this->countHit=0;
	//std::cout<<" I BEG I PLEAD I DESIRE!\n";
	this->root = addBaby(tree->root);
	this->root->strName.assign("LearnedTree");
	//std::cout<<"DID WE MAKE IT??\n";

}