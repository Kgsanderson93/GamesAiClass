#include <string>
#include "Observation.h"

std::string missing_attr = "?";
float num_target_val = 7;
LearningTreeNodeLeaf::LearningTreeNodeLeaf()
{
	type = std::string("leaf");
}
DiscAttrLearningTreeNode::DiscAttrLearningTreeNode()
{
	type = std::string("discrete");
}

ContAttrLearningTreeNode::ContAttrLearningTreeNode()
{
	type = std::string("continuous");
}

Instance::Instance()
{
	;
}

LearningTree::LearningTree(std::string attPath, std::string path)
{

	//std::cout << "start \n";
	this->contDeci = std::vector<ContAttrLearningTreeNode *>();
	this->discDeci = std::vector<DiscAttrLearningTreeNode *>();
	this->leaves = std::vector<LearningTreeNodeLeaf *>();
	this->leaves.emplace_back(new LearningTreeNodeLeaf());
	this->leaves.at(0)->type.assign("null");
	this->leaves.at(0)->name.assign("defaultNull");
	this->leaves.at(0)->targetName.assign("MoveToPlayerAction");

	std::set<std::string> target_vals = std::set<std::string>();
	std::vector<std::string> attr_names = std::vector<std::string>();

	// addTargetValues
	target_vals.insert("MobPatrol");
	target_vals.insert("MoveToPlayerAction");
	target_vals.insert("PlayerHitAction");
	target_vals.insert("MobDanceAction"); // 1
	target_vals.insert("MoveToFruitAction");
	target_vals.insert("MobEat"); // 2
	target_vals.insert("SmashFruitAction");

	//std::cout << "start5\n";

	this->addTargetValues(target_vals);
	//std::cout << "start2\n";

	// addAttrInfo
	std::vector<std::vector<std::string>> dat = Reader::readData(attPath);
	for (std::vector<std::string> t : dat)
	{
		for (std::string s : t)
		{
			//std::cout << s << " " << s.size() << ",";
		}
		//std::cout << "\n";
	}
	//std::cout << "start2\n";

	for (float i = 0; i < dat.size(); i++)
	{

		attr_names.push_back(dat[i][0]);
		std::vector<std::string> temp;
		for (float j = 1; j < dat[i].size(); j++)
		{
			//std::cout << dat[i][j] << "\n";
			if (dat[i][j].compare("continuous") == 0)
			{
				break;
			}
			else
			{
				temp.push_back(dat[i][j]);
				//std::cout << dat[i][j] << "\n";
			}
		}
		this->addAttrInfo(dat[i][0], temp);
	}
	//std::cout << "start3\n";
	getExamples(path, attr_names);
	// build
	this->build();

	//std::cout << this->root->name << "name\n";
	//std::cout << this->contDeci.size()<<"cont\n";
	//std::cout << this->discDeci.size()<<"disc\n";
	//std::cout << this->leaves.size()<<"leaf\n";
}

Instance::Instance(
	const std::vector<std::string> &attr_names, const std::vector<std::string> &attr_vals)
{
	for (float i = 0; i < attr_names.size(); i++)
	{

		els[attr_names[i]] = attr_vals[i];
	}
}

std::string Instance::operator[](const std::string &attr_name)
{
	return els[attr_name];
}

Example::Example()
{
	;
}

Example::Example(
	const std::vector<std::string> &attr_names, const std::vector<std::string> &attr_vals,
	const std::string &target_class)

{
	//std::cout << attr_names.at(attr_names.size() - 1) << " " << attr_vals.at(attr_vals.size() - 1) << "\n";
	//std::cout << "Build an example" << attr_names.size() << " " << attr_vals.size() << " " << els.size() << "\n";

	for (float i = 0; i < attr_names.size(); i++)
	{
		els[attr_names[i]].assign(attr_vals[i]);
	}
	//std::cout << "Build an example2" << "\n";

	this->target_class.assign(target_class);
	//std::cout << "Build an example3" << "\n";
}

std::string Example::getTargetClass() const
{
	return target_class;
}

void LearningTree::addAttrInfo(
	const std::string &attr_name, const std::vector<std::string> &attr_vals)
{
	pos_vals[attr_name] = attr_vals;
}

void LearningTree::addTargetValues(std::set<std::string> target_values)
{
	this->target_values = target_values;
}

void LearningTree::build()
{
	//std::cout << this->data.size() << "\n";
	std::vector<std::string> all_attr;
	for (auto it = pos_vals.begin(); it != pos_vals.end(); it++)
	{

		all_attr.push_back(it->first);
	}
	//std::cout << all_attr.size() << "\n";
	this->root = (LearningTreeDecisionNode *)build(this->data, all_attr, 0);
	//std::cout << root->name << "\n";
}
std::string LearningTree::RandomAction()
{
	int i = random() % 7;
	std::set<std::string>::iterator it = target_values.begin();
	std::advance(it, i);
	return *it;
}
DecisionTreeNode *LearningTree::build(std::vector<Example> train_data,
									  std::vector<std::string> check_attr, float depth)
{

	float maj = 0;
	bool leaf = false;
	std::string target_class;
	float standard = 0.90f;
	std::cout << "Leaves :" << this->leaves.size() << " Cont dec" << this->contDeci.size() << " Dec decision" << this->discDeci.size() << "\n";
	std::cout << "New Build " << train_data.size() << " " << depth << "\n";
	if (train_data.empty())
	{
		//std::cout << "NULL leaf\n";
		this->leaves.emplace_back(new LearningTreeNodeLeaf());
		this->leaves.at(this->leaves.size() - 1)->name.assign("EmptySet");
		this->leaves.at(this->leaves.size() - 1)->type.assign("leaf");
		this->leaves.at(this->leaves.size() - 1)->targetName.assign("PlayerHitAction");
		return this->leaves.at(this->leaves.size() - 1);
	}

	target_class.assign("MoveToPlayerAction");
	//std::cout << target_class << "\n";

	//std::cout << train_data.size() << "\n";
	float count = 0;

	std::map<std::string, float> targets;
	int totalTrainData = 0;
	// we really don't need pur nodes. Just majorities.
	for (float i = 1; i < train_data.size(); i++)
	{
		////std::cout << "Target?" << train_data[i].getTargetClass() << "\n";

		targets[train_data[i].getTargetClass()]++;
		totalTrainData++;
	}
	for (auto e : targets)
	{
		////std::cout << "in count targets " << e.first << " " << e.second << "\n";
		if (e.second > count)
		{
			count = e.second;
			target_class.assign(e.first);
			if (e.first.compare("HitPlayerAction") == 0 && (float)e.second / (float)train_data.size() > .7)
			{
				std::cout<<"yeahahhh ok\n";
				this->leaves.emplace_back(new LearningTreeNodeLeaf());
				this->leaves.at(this->leaves.size() - 1)->name.assign("I cheated but we need to hit players eventually.");
				this->leaves.at(this->leaves.size() - 1)->type.assign("leaf");
				this->leaves.at(this->leaves.size() - 1)->targetName.assign("HitPlayerAction");
				return this->leaves.at(this->leaves.size() - 1);
			}
		}
	}
	if (totalTrainData != train_data.size())
	{
		//std::cout<<"well thats weird\n";
	}
	//std::cout << maj << " " << train_data.size()<<" "<<target_class<<"\n";
	maj = (float)count / (float)train_data.size();

	if (train_data.size() < 40 || depth > 5)
	{
		standard = .80;
	}
	if (train_data.size() < 20 || depth > 6)
	{
		standard = .70;
	}
	if (train_data.size() < 10 || depth > 7)
	{
		standard = .5;
	}
	if (train_data.size() < 5 || depth > 8)
	{
		standard = -.3;
	}

	//std::cout << "majority? " << maj << " standard " << standard << "\n";
	if (maj > standard || train_data.empty() || check_attr.empty())
	{

		//std::cout << "I BELIEVE\n";
		leaf = true;
	}

	if (leaf)
	{
		this->leaves.emplace_back(new LearningTreeNodeLeaf());
		this->leaves.at(this->leaves.size() - 1)->type.assign("leaf");
		if (train_data.size() > 0)
		{
			this->leaves.at(leaves.size() - 1)->name.assign("True Leaf 1");
			//std::cout<<"I will now print this value\n";
			//std::cout<<target_class;
			//std::cout<<"\n";
			//std::cout<<target_class.empty()<<"\n";

			if (!target_class.empty() || target_class.size() != 0 || target_values.find(target_class) != target_values.end())
			{
				this->leaves.at(leaves.size() - 1)->targetName.assign(target_class);
			}
			else
			{
				//std::cout<<"\n\n\nOH SHIT I GOT ONE!\n\n\n\n";
				this->leaves.at(leaves.size() - 1)->targetName.assign("MoveToPlayerAction");
			}
		}
		else
		{
			this->leaves.at(leaves.size() - 1)->name.assign("False Leaf");

			this->leaves.at(this->leaves.size() - 1)->targetName.assign("PlayerHitAction");
		}

		//std::cout << leaf << "leaf" << " " << target_class << train_data.size() << "\n";
		return this->leaves.at(leaves.size() - 1);
	}
	std::string div;
	double max_gain = 999;
	float max_index = 0;
	bool is_cont = false;
	float divider = 0;

	//std::cout << "into split check\n"<<check_attr.empty()<<"\n";
	if (check_attr.empty())
	{
		std::cout<<"OOPS its empty\n";
	}
	//std::cout << check_attr.size();
	for (float i = 0; i < check_attr.size(); i++)
	{
		//std::cout << "continuous " << check_attr[i];
		if (pos_vals[check_attr[i]].size() == 0)
		{

			// if continuous attribute
			std::pair<double, float> temp = contInfoGain(
				train_data, check_attr[i]);
			//std::cout << "continuous " << check_attr[i] << " " << temp.first << " " << temp.second << "\n";

			if (temp.first < max_gain)
			{
				max_gain = temp.first;
				max_index = i;
				is_cont = true;
				divider = (temp.second);
			}
		}
		else
		{
			std::pair<double, std::string> temp = discInfoGain(
				train_data, check_attr[i]);
			//std::cout << "discrete " << check_attr[i] << " " << temp.first << " " << temp.second << "\n";

			if (temp.first < max_gain)
			{
				max_gain = temp.first;
				max_index = i;
				is_cont = false;
				div.assign(temp.second);
			}
		}
	}
	std::cout << "Max Entropy..supposedly " << check_attr[max_index] << " " << max_gain << "\n";
	std::string attr_name = check_attr[max_index];
	//std::cout << attr_name << " " << check_attr.size() << "\n";
	check_attr.erase(check_attr.begin() + max_index);
	//std::cout << "ok erased" << check_attr.size() << " "<< is_cont << "\n";

	depth = depth + 1;
	if (is_cont)
	{
		const int size = contDeci.size();
		//std::cout << is_cont << "\n";
		//std::cout << divider << " " << attr_name << "\n";

		////std::cout << curr->type << "\n";
		std::vector<Example> sub_train_data;
		std::vector<Example> sub_train_data2;
		//std::cout << "got here\n";
		for (Example e : train_data)
		{
			if (atof(e[attr_name].c_str()) <= divider)
			{

				sub_train_data.emplace_back(e);
			}
			else
			{
				sub_train_data2.emplace_back(e);
			}
		}
		bool root = false;
		std::cout<<"root check "<<contDeci.size()<<" "<<discDeci.size()<<"\n";
		if (this->contDeci.size() == 0 && this->discDeci.size() == 0)
		{
			std::cout << "found root\n";

			root = true;
		}
		this->contDeci.emplace_back(new ContAttrLearningTreeNode());

		this->contDeci.at(size)->falseBranch = this->leaves.at(0);
		this->contDeci.at(size)->trueBranch = this->leaves.at(0);

		//std::cout << "here\n";

		this->contDeci.at(size)->attr_val.assign(std::to_string(divider));
		this->contDeci.at(size)->attr_name.assign(attr_name);
		this->contDeci.at(size)->name.assign("continuous");
		if (root)
		{
			std::cout << "found root\n";
			this->root = this->contDeci.at(size);
		}


		




		
		

		//std::cout << "New Train sizes " << sub_train_data2.size() << " " << sub_train_data.size() << "\n ";
		if (!sub_train_data2.empty())
		{
			this->contDeci.at(size)->falseBranch = build(sub_train_data2, check_attr, depth++);
		}
		else
		{
			//std::cout << "NULL leaf\n";
			this->leaves.emplace_back(new LearningTreeNodeLeaf());
			this->leaves.at(this->leaves.size() - 1)->type.assign("null");
			this->leaves.at(this->leaves.size() - 1)->name.assign("False Leaf");
			this->leaves.at(this->leaves.size() - 1)->targetName.assign("PlayerHitAction");

			this->contDeci.at(size)->falseBranch == this->leaves.at(this->leaves.size() - 1);
		}
		if (!sub_train_data.empty())
		{
			this->contDeci.at(size)->trueBranch = build(sub_train_data, check_attr, depth++);
		}
		else
		{
			//std::cout << "NULL leaf\n";
			this->leaves.emplace_back(new LearningTreeNodeLeaf());
			this->leaves.at(this->leaves.size() - 1)->type.assign("null");
			this->contDeci.at(size)->trueBranch == this->leaves.at(this->leaves.size() - 1);
			this->leaves.at(this->leaves.size() - 1)->name.assign("False Leaf");
			this->leaves.at(this->leaves.size() - 1)->targetName.assign("PlayerHitAction");
		}
		//std::cout << "Cont Branch " << this->contDeci.at(size)->attr_name << "\n";
		return this->contDeci.at(size);
	}
	else
	{
		const int size = this->discDeci.size();
		//std::cout << div << "\n";
		bool root = false;
		if (this->contDeci.size() == 0 && this->discDeci.size() == 0)
		{
			root = true;
		}
		this->discDeci.emplace_back(new DiscAttrLearningTreeNode());
		if (root)
		{
			std::cout << "found root\n";
			this->root = this->discDeci.at(discDeci.size() - 1);
		}
		this->discDeci.at(size)->falseBranch = this->leaves.at(0);
		this->discDeci.at(size)->trueBranch = this->leaves.at(0);
		//std::cout << div << "\n";
		this->discDeci.at(size)->name.assign("discrete");
		this->discDeci.at(size)->attr_val.assign(div);
		this->discDeci.at(size)->attr_name.assign(attr_name);
		std::vector<Example> sub_train_data;
		std::vector<Example> sub_train_data2;
		for (Example e : train_data)
		{
			if (e[attr_name].compare(div) != 0)
			{
				sub_train_data.emplace_back(e);
			}
			else
			{
				sub_train_data2.emplace_back(e);
			}
		}
		//std::cout << sub_train_data2.size() << " " << sub_train_data.size() << "\n ";

		if (!sub_train_data2.empty())
		{
			this->discDeci.at(size)->falseBranch = build(sub_train_data2, check_attr, depth);
		}
		else
		{
			//std::cout << "NULL leaf\n";
			this->leaves.emplace_back(new LearningTreeNodeLeaf());
			this->leaves.at(this->leaves.size() - 1)->type.assign("null");
			this->leaves.at(this->leaves.size() - 1)->name.assign("False Leaf");
			this->leaves.at(this->leaves.size() - 1)->targetName.assign("PlayerHitAction");

			this->discDeci.at(size)->falseBranch == this->leaves.at(this->leaves.size() - 1);
		}
		if (!sub_train_data.empty())
		{
			this->discDeci.at(size)->trueBranch = build(sub_train_data, check_attr, depth);
		}
		else
		{
			//std::cout << "NULL leaf\n";
			this->leaves.emplace_back(new LearningTreeNodeLeaf());
			this->leaves.at(this->leaves.size() - 1)->type.assign("null");
			this->leaves.at(this->leaves.size() - 1)->name.assign("False Leaf");
			this->leaves.at(this->leaves.size() - 1)->targetName.assign("PlayerHitAction");

			this->discDeci.at(size)->falseBranch == this->leaves.at(this->leaves.size() - 1);
		}
		//std::cout << "Disc Branch " << this->discDeci.at(size)->attr_name << "\n";

		return this->discDeci.at(size);
	}
	//std::cout << "LeafNode " << this->leaves.at(this->leaves.size() - 1)->targetName << "\n";

	this->leaves.emplace_back(new LearningTreeNodeLeaf());
	this->leaves.at(this->leaves.size() - 1)->type.assign("IDK leaf");
	this->leaves.at(this->leaves.size() - 1)->targetName.assign("PlayerHitAction");

	//std::cout << "IDK something bad\n";
	return this->leaves.at(this->leaves.size());
}

void LearningTree::print()
{
	//std::cout << "Root " << root->type << "\n";
	print(this->root->trueBranch, 1);
	print(this->root->falseBranch, 1);
}

void LearningTree::print(DecisionTreeNode *p, float indent)
{
	float i = indent;
	i++;
	//std::cout << p->name << "\n";
	if (p->type.compare("discrete") == 0)
	{
		DiscAttrLearningTreeNode *pp = static_cast<DiscAttrLearningTreeNode *>(p);
		DecisionTreeNode *t = pp->trueBranch;
		DecisionTreeNode *f = pp->falseBranch;
		//std::cout << indent << " " << pp->type << "\n";

		print(t, i);
		print(f, i);
	}
	else if (p->type.compare("continuous") == 0)
	{
		ContAttrLearningTreeNode *pp = static_cast<ContAttrLearningTreeNode *>(p);
		//std::cout << indent++ << " " << pp->type << "\n";
		DecisionTreeNode *t = pp->trueBranch;
		DecisionTreeNode *f = pp->falseBranch;

		print(t, i);
		print(f, i);
	}
	else
	{
		//std::cout << indent << " " << p->type << "\n";
	}
}

std::pair<float, float> LearningTree::contInfoGain(std::vector<Example> els, const std::string &attr_name)
{
	//std::cout << els.size() << " " << attr_name << "\n";
	std::map<std::string, float> premap;
	for (std::string s : this->target_values)
	{
		premap[s] = 0;
	}
	for (Example e : els)
	{
		premap[e.getTargetClass()]++;
		//std::cout << premap[e.getTargetClass()] << "\n";
	}
	float PreEnt = 0;
	for (auto it = premap.begin(); it != premap.end(); it++)
	{
		if ((float)it->second != 0)
		{
			//std::cout << "in prenet cal " << it->first << " " << it->second << "\n";
			float p = ((float)it->second / (float)els.size());
			float logp = std::log2(p);
			//std::cout << logp << " " << p << "\n";

			PreEnt += (p * logp);
			//std::cout << PreEnt << "\n";
		}
	}
	PreEnt *= -1;
	//std::cout << PreEnt << "\n";

	float min = 99;
	float max = 0;
	for (Example e : els)
	{
		if (atof(e[attr_name].c_str()) > max && atof(e[attr_name].c_str()) != 999999)
		{
			max = atof(e[attr_name].c_str());
		}
		if (atof(e[attr_name].c_str()) < min && atof(e[attr_name].c_str()) != -99)
		{
			min = atof(e[attr_name].c_str());
		}
	}
	float div = (max + min) / 2;
	//std::cout << "Max and min" << max << " " << min << " " << div << "\n";

	std::map<std::string, float> minmap;
	std::map<std::string, float> maxmap;

	for (std::string s : this->target_values)
	{
		minmap[s] = 0;
	}
	for (std::string s : this->target_values)
	{
		maxmap[s] = 0;
	}
	float mincount = 0;
	float maxcount = 0;
	for (Example e : els)
	{
		if (atof(e[attr_name].c_str()) > div)
		{
			maxmap[e.getTargetClass()]++;
			maxcount++;
		}
		else
		{
			minmap[e.getTargetClass()]++;
			mincount++;
		}
	}
	float minEnt = 0;
	for (auto it = minmap.begin(); it != minmap.end(); it++)
	{
		if ((float)it->second != 0)
		{
			float p = ((float)it->second / (float)mincount);
			float logp = std::log2(p);
			//std::cout << "Log p and p" << logp << " " << p << "\n";
			minEnt += (p * logp);
			//std::cout << "minEnd" << minEnt << "\n";
		}
	}
	minEnt *= -1;
	//std::cout << "minEnt" << minEnt << "\n";

	float maxEnt = 0;
	for (auto it = maxmap.begin(); it != maxmap.end(); it++)
	{
		//std::cout << "it->second " << (float)it->second << "\n";
		if ((float)it->second != 0)
		{
			float p = ((float)it->second / (float)maxcount);
			float logp = std::log2(p);
			//std::cout << "Log p and p" << logp << " " << p << "\n";

			maxEnt += (p * logp);
			//std::cout << "maxEnd" << maxEnt << "\n";
		}
	}
	maxEnt *= -1;
	//std::cout << "maxEnd" << maxEnt << "\n";

	float maxp = (float)maxcount / (float)els.size();
	float minp = (float)mincount / (float)els.size();

	//std::cout << minp << " " << maxp << "\n";

	float postent = maxp * maxEnt + minp * minEnt;
	//std::cout << PreEnt << " " << postent << " " << PreEnt - postent << "\n";
	return std::make_pair(PreEnt - postent, div);
}

std::pair<float, std::string> LearningTree::discInfoGain(std::vector<Example> els, const std::string &attr_name)
{
	float max = 0;
	std::string maxlabel;
	std::map<std::string, float> premap;
	for (Example e : els)
	{
		premap[e.getTargetClass()]++;
	}
	float PreEnt = 0;
	for (auto it = premap.begin(); it != premap.end(); it++)
	{
		if (it->second > max)
		{
			max = it->second;
			maxlabel = it->first;
		}
		float p = ((float)it->second / (float)els.size());
		PreEnt += (p * std::log2(p));
	}
	PreEnt *= -1;

	float maxEnt = 0;
	std::string bestlabel;

	for (std::string d : pos_vals[attr_name])
	{
		//std::cout << "Poss val " << d << "\n";
		std::map<std::string, float> label;
		for (std::string d : pos_vals[attr_name])
		{
			label[d] = 0;
		}
		float labelcount = 0;
		std::map<std::string, float> nonlabel;
		for (std::string d : pos_vals[attr_name])
		{
			nonlabel[d] = 0;
		}
		float nonlabelcount = 0;
		for (Example e : els)
		{
			//std::cout << "Label me!" << d << " do these match?? " << e[attr_name] << "\n";
			if (e[attr_name].compare(d) == 0)
			{
				//std::cout << e[attr_name] << "\n";
				label[d] += 1.0f;
				labelcount += 1.0f;
				//std::cout << "count so far matched " << labelcount << " on this val " << d << " " << label[d] << "\n";
			}
			else
			{
				nonlabel[d] += 1.0f;
				nonlabelcount += 1.0f;
				//std::cout << "count so far non" << nonlabelcount << " on this val " << d << " " << nonlabel[d] << "\n";
			}
		}
		float labelent = 0;
		for (auto it = label.begin(); it != label.end(); it++)
		{
			if (it->second != 0 && labelcount != 0)
			{
				//std::cout << "label Vals" << it->second << " " << labelcount << "\n";
				float p = it->second / labelcount;
				float logp = std::log2(p);
				//std::cout << "pval" << logp << " " << p << "\n";
				labelent += (p * logp);
			}
		}
		labelent *= -1;
		//std::cout << "Post label" << labelent << "\n";
		float nonlabelent = 0;
		for (auto it = nonlabel.begin(); it != nonlabel.end(); it++)
		{
			if (it->second != 0 && nonlabelcount != 0)
			{
				//std::cout << "nonlabel Vals" << it->second << " " << nonlabelcount << "\n";

				float p = ((float)it->second / (float)nonlabelcount);
				float logp = std::log2(p);
				//std::cout << "pval" << logp << " " << p << "\n";
				nonlabelent += (p * logp);
			}
		}
		nonlabelent *= -1;
		//std::cout << "nonlabel post " << nonlabelent << "\n";
		float potentalend = ((float)labelcount / (float)els.size()) * (float)(labelent) + ((float)nonlabelcount / (float)els.size()) * (float)(nonlabelent);
		//std::cout << "TELL ME MY POTENTIAL " << potentalend << " " << maxEnt << "\n";
		if (potentalend >= maxEnt)
		{
			//std::cout << d << "\n";
			maxEnt == potentalend;
			bestlabel.assign(d);
		}
		//std::cout << "Preent " << PreEnt << "\n";
		//std::cout << "MaxEnt " << maxEnt << "\n";
	}
	return std::make_pair(PreEnt - maxEnt, bestlabel);
}

double LearningTree::calcEntropy(const std::map<std::string, float> &els)
{
	double sum = 0.0, ans = 0.0;
	std::vector<double> temp;
	for (auto it = els.begin(); it != els.end(); it++)
	{
		sum += it->second;
		temp.push_back((double)it->second);
	}
	for (float i = 0; i < temp.size(); i++)
		ans += (temp[i] / sum) * (log2(temp[i] / sum));
	ans *= -1.00;
	return ans;
}

std::vector<std::vector<std::string>> Reader::readData(std::string fileloc)
{

	std::ifstream fin(fileloc, std::ios::in);

	std::vector<std::vector<std::string>> data;

	while (!fin.eof())
	{

		std::string s;
		std::vector<std::string> att;
		fin >> s;
		std::stringstream str(s);
		//std::cout << s << "\n";
		//std::cout << str.peek() << "\n";
		while (str)
		{

			std::string temp;
			std::getline(str, temp, ',');
			if (temp.size() != 0)
			{
				//std::cout << temp << " " << temp.size() << "\n";

				att.push_back(temp);
			}
		}

		data.push_back(att);
	}
	//std::cout << "start26\n";
	return data;
}

std::set<std::string> Reader::readTargetVal(std::string fileloc, float n)
{
	std::ifstream fin(fileloc, std::ios::in);
	std::set<std::string> att;
	while (!fin.eof())
	{
		std::string s;
		fin >> s;
		std::stringstream str(s);
		for (float i = 0; i < n; i++)
		{
			std::string temp;
			std::getline(str, temp, ',');
			if (i == n - 1)
				att.insert(temp);
		}
	}

	return att;
}

void LearningTree::getExamples(const std::string &fileloc, const std::vector<std::string> &attr_names)
{
	/**std::ofstream out("targets.txt");
	std::streambuf *coutbuf;
	  coutbuf =////std::cout.rdbuf(); // save old buf
		//std::cout.rdbuf(out.rdbuf());*/
	std::vector<std::vector<std::string>> dataRead = Reader::readData(fileloc);
	//std::cout<<target_values.size();
	for (std::string s : target_values)
	{
		//std::cout<<"\n";
		//std::cout<<s;
	}
	for (float i = 0; i < dataRead.size(); i++)
	{

		std::string target_value = dataRead[i][dataRead[i].size() - 1];
		if (target_values.find(target_value) != target_values.end())
		{

			dataRead[i].pop_back();
			//std::cout << target_value << " " << dataRead[i].at(0) << " " << i << " " << dataRead.size() << "\n";
			this->data.push_back(Example(attr_names, dataRead[i], target_value));
			bool found = false;
			for (std::string t : this->target_values)
			{
				if (target_value.empty() == true)
				{
					//std::cout<<" ME EMPTY"<<target_value<<"\n";
				}
				if (target_value.compare(t) == 0)
				{
					found = true;
				}
			}
			if (found == false)
			{
				//std::cout<<" ME"<<target_value<<"\n";
			}
		}
		else
		{
			for (std::string it : dataRead[i])
			{
				//std::cout<<it<<" ";
			}
		}

		// out.close();
		////std::cout.rdbuf(coutbuf);
	}
}
void LearningTreeBuilder(std::string attPath, std::string path, LearningTree *lt)
{

	//std::cout << "start\n";

	// lt->print();
}
