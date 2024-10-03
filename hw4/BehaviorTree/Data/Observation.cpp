
#include "Observation.h"

std::string missing_attr = "?";
int num_target_val = 7;

LearningTreeNode::LearningTreeNode()
{
	;
}

void LearningTreeNode::setAttrName(const std::string &attr_name)
{
	this->attr_name = attr_name;
}

std::string LearningTreeNode::getAttrName()
{
	return attr_name;
}

void LearningTreeNode::setType(const std::string &type)
{
	this->type = type;
}

std::string LearningTreeNode::getType()
{
	return type;
}

DiscAttrLearningTreeNode::DiscAttrLearningTreeNode()
{
	type = "discrete";
}

LearningTreeNode *&DiscAttrLearningTreeNode::operator[](const std::string &attr_val)
{
	return child[attr_val];
}

std::vector<LearningTreeNode *> DiscAttrLearningTreeNode::getChildPointers()
{
	std::vector<LearningTreeNode *> ret;
	for (auto it = child.begin(); it != child.end(); it++)
	{
		ret.push_back(it->second);
	}
	return ret;
}

ContAttrLearningTreeNode::ContAttrLearningTreeNode()
{
	type = "continuous";
}

int ContAttrLearningTreeNode::getIndex(double attr_val)
{
	auto it = std::upper_bound(dividers.begin(), dividers.end(), attr_val);
	return it - dividers.begin();
}

LearningTreeNode *&ContAttrLearningTreeNode::operator[](const double &attr_val)
{
	return child[getIndex(attr_val)];
}

void ContAttrLearningTreeNode::setDividers(const std::vector<double> dividers)
{
	this->dividers = dividers;
	sort((this->dividers).begin(), (this->dividers).end());
	child.resize((this->dividers).size() + 1);
}

std::vector<LearningTreeNode *> ContAttrLearningTreeNode::getChildPointers()
{
	return child;
}

Instance::Instance()
{
	;
}

Instance::Instance(
	const std::vector<std::string> &attr_names, const std::vector<std::string> &attr_vals)
{
	for (int i = 0; i < attr_names.size(); i++)
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
	// //std::cout<<attr_names.at(attr_names.size()-1)<<" "<<attr_vals.at(attr_vals.size()-1)<<"\n";
	// //std::cout << "Build an example" << attr_names.size() << " " << attr_vals.size() << "\n";

	for (int i = 0; i < attr_names.size(); i++)
	{
		els[attr_names[i]] = attr_vals[i];
	}
	// //std::cout << "Build an example2" << "\n";

	this->target_class = target_class;
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

void LearningTree::build(const std::vector<Example> &train_data)
{

	std::vector<std::string> all_attr;
	for (auto it = pos_vals.begin(); it != pos_vals.end(); it++)
	{

		all_attr.push_back(it->first);
	}
	build(train_data, root, all_attr, 0);
}

void LearningTree::build(std::vector<Example> train_data,
						 LearningTreeNode *&p, std::vector<std::string> check_attr, int depth)
{
	
	
	std::cout << "in Build " << depth << " " << train_data.empty() << " " << check_attr.size() << "\n";
	
	// check if there is any training data. if not assign a target class randomly
	if (train_data.empty() || train_data.size() < 25 || depth > 6)
	{
		p = new LearningTreeNode;
		p->setAttrName("random");
		p->setType("leaf");
		std::cout << "made a node \n";

		return;
	}

	// check if all examples have same target class
	bool leaf = true;
	std::string target_class = train_data[0].getTargetClass();
	std::cout << target_class << "\n";

	std::cout << train_data.size() << "\n";

	for (int i = 1; i < train_data.size(); i++)
	{
		// std::cout << train_data[i].getTargetClass() << "\n";

		if (train_data[i].getTargetClass() != target_class)
		{
			// std::cout << train_data[i].getTargetClass() << "\n";
			leaf = false;
			break;
		}
	}
	std::cout << "out of clas check\n";

	if (leaf)
	{
		p = new LearningTreeNode;
		p->setAttrName(target_class);
		p->setType("leaf");
	}
	else
	{
		double max_gain = -1;
		int max_index = 0;
		std::vector<double> dividers;
		bool is_cont;
		std::cout << "into split check\n";

		// find which attribute should be at the node
		for (int i = 0; i < check_attr.size(); i++)
		{
			if (pos_vals[check_attr[i]].size() == 0)
			{
				// if continuous attribute
				std::pair<double, std::vector<double>> temp = contInfoGain(
					train_data, check_attr[i]);
				std::cout << "continuous " << check_attr[i] << " " << temp.first << "\n";

				double cand_gain = temp.first;
				if (cand_gain > max_gain)
				{
					max_gain = cand_gain;
					max_index = i;
					is_cont = true;
					dividers = temp.second;
				}
			}
			else
			{

				double cand_gain = discInfoGain(train_data, check_attr[i]);
				std::cout << "discrete " << check_attr[i] << " " << cand_gain << "\n";

				if (cand_gain > max_gain)
				{
					max_gain = cand_gain;
					max_index = i;
					is_cont = false;
				}
			}
		}
		std::cout << "Max Entropy..supposedly" << check_attr[max_index] << "\n";
		// now, the attribute to be placed has been found
		std::string attr_name = check_attr[max_index];
		std::cout << attr_name << "\n";
		for (double d : dividers)
		{
			// std::cout<<d<<",";
		}
		// std::cout<<"\n";
		check_attr.erase(check_attr.begin() + max_index);

		if (is_cont)
		{
			p = new ContAttrLearningTreeNode;
			p->setType("continuous");
			p->setAttrName(attr_name);

			ContAttrLearningTreeNode *pp = static_cast<ContAttrLearningTreeNode *>(p);
			pp->setDividers(dividers);

			// std::cout<<" iterating through each child\n";
			for (int i = 0; i <= dividers.size(); i++)
			{
				// std::cout<<" check which examples will be in this child\n";
				std::vector<Example> sub_train_data;
				for (int j = 0; j < train_data.size(); j++)
				{
					if (pp->getIndex(atof(train_data[j][attr_name].c_str())) == i)
					{
						sub_train_data.push_back(train_data[j]);
					}
				}
				std::cout << sub_train_data.size() << "\n";
				if (!sub_train_data.empty())
				{
					build(sub_train_data,
						  (*pp)[atof(sub_train_data[0][attr_name].c_str())], check_attr, depth++);
				}
			}
		}
		else
		{
			p = new DiscAttrLearningTreeNode;
			p->setType("discrete");
			p->setAttrName(attr_name);

			DiscAttrLearningTreeNode *pp = static_cast<DiscAttrLearningTreeNode *>(p);
			// iterating through each possible value for the selected attribute
			for (int i = 0; i < pos_vals[attr_name].size(); i++)
			{
				(*pp)[pos_vals[attr_name][i]] = NULL;
				std::vector<Example> sub_train_data;
				for (int j = 0; j < train_data.size(); j++)
				{
					if (train_data[j][attr_name] == pos_vals[attr_name][i])
					{
						sub_train_data.push_back(train_data[j]);
					}
				}
				build(sub_train_data, (*pp)[pos_vals[attr_name][i]], check_attr, depth++);
			}
		}
	}
}

void LearningTree::print()
{
	print(root);
}

void LearningTree::print(LearningTreeNode *p)
{
	// std::cout << (p->getAttrName());
	if (p->getType() == "discrete")
	{
		DiscAttrLearningTreeNode *pp = static_cast<DiscAttrLearningTreeNode *>(p);
		std::vector<LearningTreeNode *> child_pointers;
		child_pointers = pp->getChildPointers();
		for (int i = 0; i < child_pointers.size(); i++)
		{
			print(child_pointers[i]);
		}
	}
	else if (p->getType() == "continuous")
	{
		ContAttrLearningTreeNode *pp = static_cast<ContAttrLearningTreeNode *>(p);
		std::vector<LearningTreeNode *> child_pointers;
		child_pointers = pp->getChildPointers();
		for (int i = 0; i < child_pointers.size(); i++)
		{
			print(child_pointers[i]);
		}
	}
}

std::pair<double, std::vector<double>> LearningTree::contInfoGain(std::vector<Example> els, const std::string &attr_name)
{

	std::set<std::pair<double, std::string>> cont_val_set;
	for (int i = 0; i < els.size(); i++)
		cont_val_set.insert(make_pair(atof(els[i][attr_name].c_str()), els[i].getTargetClass()));

	std::vector<std::pair<double, std::string>> cont_val_list;
	for (auto it = cont_val_set.begin(); it != cont_val_set.end(); it++)
	{
		cont_val_list.push_back(*it);
	}

	double pos, info_gain = -1;
	double sum = els.size();

	std::map<std::string, int> calc_entropy_map;
	for (int i = 0; i < els.size(); i++)
		calc_entropy_map[els[i].getTargetClass()]++;

	double entropy_1 = calcEntropy(calc_entropy_map);

	for (int i = 1; i < cont_val_list.size(); i++)
	{

		if (cont_val_list[i].second == cont_val_list[i - 1].second)
			continue;
		else
		{
			double numerator = 0;
			calc_entropy_map.clear();
			for (int j = 0; j <= i - 1; j++)
				calc_entropy_map[cont_val_list[j].second]++, numerator++;

			double entropy_2 = (numerator / sum) * calcEntropy(calc_entropy_map);
			calc_entropy_map.clear();
			numerator = 0;

			for (int j = i; j < cont_val_list.size(); j++)
				calc_entropy_map[cont_val_list[j].second]++, numerator++;

			entropy_2 += (numerator / sum) * calcEntropy(calc_entropy_map);
			if (entropy_1 - entropy_2 > info_gain)
			{
				info_gain = entropy_1 - entropy_2;
				pos = (cont_val_list[i].first + cont_val_list[i - 1].first) / 2;
			}
		}
	}
	std::vector<double> temp;
	temp.push_back(pos);
	return make_pair(info_gain, temp);
}

double LearningTree::discInfoGain(std::vector<Example> els, const std::string &attr_name)
{

	// std::cout << attr_name << "\n";
	std::set<std::string> target_val_set = target_values;
	/*for(std::string s: target_val_set){
		//std::cout<<s<<" "<<s.size()<<"\n";
	}*/
	std::map<std::string, int> num_attr[num_target_val];

	std::vector<std::string> temp_tar_val;
	for (auto it = target_val_set.begin(); it != target_val_set.end(); it++)
	{
		temp_tar_val.push_back(*it);
		// //std::cout<<*it<<"\n";
	}

	for (int i = 0; i < els.size(); i++)
	{
		Example temp = els[i];
		// //std::cout<<attr_name<<" "<<temp[attr_name]<<"\n";
		if (temp[attr_name] != missing_attr)
		{
			for (auto it = temp_tar_val.begin(); it != temp_tar_val.end(); it++)
			{
				if (*it == temp.getTargetClass())
				{
					int pos = it - temp_tar_val.begin();

					num_attr[pos][temp[attr_name]]++;
					// std::cout << pos << " " << temp[attr_name] << " " << num_attr[pos][temp[attr_name]] << "\n";
					break;
				}
			}
		}
	}
	// std::cout << "fin count\n";
	std::map<std::string, int>::iterator it;

	/**for(int i = 0; i<7; i++){
		for(auto it= num_attr[i].begin(); it!=num_attr[i].end();i++){
			//std::cout<<it->first<<":"<<it->second<<"\n";
		}
	}*/

	for (int i = 0; i < els.size(); i++)
	{
		Example temp = els[i];

		int pos;

		if (temp[attr_name] != missing_attr)
		{
			// //std::cout<<"should have no missing vars\n";
			for (auto it = temp_tar_val.begin(); it != temp_tar_val.end(); it++)
			{

				if (*it == temp.getTargetClass())
				{
					pos = it - temp_tar_val.begin();
					break;
				}
			}
		}

		int m = -1;
		for (auto it = num_attr[pos].begin(); it != num_attr[pos].end(); it++)
		{
			if (m <= it->second)
			{
				m = it->second;
				temp[attr_name] = it->first;
			}
		}
		els[i] = temp;
	}
	// std::cout << "ordering\n";
	double sum = els.size();

	std::map<std::string, int> calc_entropy_map;
	for (int i = 0; i < els.size(); i++)
		calc_entropy_map[els[i].getTargetClass()]++;
	// std::cout << "am I the problem?\n";
	double entropy_1 = calcEntropy(calc_entropy_map);

	// std::cout << entropy_1 << "\n";
	double entropy_2 = 0.0;

	for (int i = 0; i < pos_vals[attr_name].size(); i++)
	{

		double numerator = 0;
		calc_entropy_map.clear();

		std::string attr = pos_vals[attr_name][i];
		for (std::set<std::string>::iterator it = target_val_set.begin(); it != target_val_set.end(); it++)
		{

			for (int j = 0; j < els.size(); j++)
			{

				if (attr == els[j][attr_name])
				{
					calc_entropy_map[*it]++;
					numerator++;
				}
			}
		}

		double temp_entropy = calcEntropy(calc_entropy_map);
		// std::cout << "temp" << temp_entropy << " numerator " << numerator << " sum" << sum << "\n";

		entropy_2 += (numerator / sum) * temp_entropy;
	}
	// std::cout << "finishedEntropy" << entropy_1 - entropy_2 << "\n";
	return (entropy_1 - entropy_2) * -1;
}

double LearningTree::calcEntropy(const std::map<std::string, int> &els)
{
	double sum = 0.0, ans = 0.0;
	std::vector<double> temp;
	for (auto it = els.begin(); it != els.end(); it++)
	{
		sum += it->second;
		temp.push_back((double)it->second);
	}
	for (int i = 0; i < temp.size(); i++)
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
		// std::cout << s << "\n";
		// std::cout << str.peek() << "\n";
		while (str)
		{

			std::string temp;
			std::getline(str, temp, ',');
			if (temp.size() != 0)
			{
				// std::cout << temp << " " << temp.size() << "\n";

				att.push_back(temp);
			}
		}

		data.push_back(att);
	}
	// std::cout << "start26\n";
	return data;
}

std::set<std::string> Reader::readTargetVal(std::string fileloc, int n)
{
	std::ifstream fin(fileloc, std::ios::in);
	std::set<std::string> att;
	while (!fin.eof())
	{
		std::string s;
		fin >> s;
		std::stringstream str(s);
		for (int i = 0; i < n; i++)
		{
			std::string temp;
			std::getline(str, temp, ',');
			if (i == n - 1)
				att.insert(temp);
		}
	}

	return att;
}

std::vector<Example> getExamples(const std::string &fileloc, const std::vector<std::string> &attr_names)
{
	std::vector<Example> ret;
	std::vector<std::vector<std::string>> data = Reader::readData(fileloc);
	for (int i = 0; i < data.size(); i++)
	{
		std::string target_value = data[i][data[i].size() - 1];
		data[i].pop_back();
		// //std::cout << target_value << " " << data[i].at(0) << "\n";
		ret.push_back(Example(attr_names, data[i], target_value));
	}
	return ret;
}

void LearningTreeBuilder(std::string attPath, std::string path, LearningTree *lt)
{
	// std::cout << "start\n";
	std::set<std::string> target_vals = std::set<std::string>();
	std::vector<std::string> attr_names = std::vector<std::string>();
	lt=new LearningTree();
	

	// addTargetValues
	target_vals.insert("MobPatrol");
	target_vals.insert("MoveToPlayerAction");
	target_vals.insert("PlayerHitAction");
	target_vals.insert("MobDanceAction");
	target_vals.insert("MoveToFruitAction");
	target_vals.insert("MobEat");
	target_vals.insert("SmashFruitAction");

	// std::cout << "start5\n";

	lt->addTargetValues(target_vals);
	// std::cout << "start2\n";

	// addAttrInfo
	std::vector<std::vector<std::string>> dat = Reader::readData(attPath);
	for (std::vector<std::string> t : dat)
	{
		for (std::string s : t)
		{
			// std::cout << s << " " << s.size() << ",";
		}
		// std::cout << "\n";
	}
	// std::cout << "start2\n";

	for (int i = 0; i < dat.size(); i++)
	{

		attr_names.push_back(dat[i][0]);
		std::vector<std::string> temp;
		for (int j = 1; j < dat[i].size(); j++)
		{
			// std::cout << dat[i][j] << "\n";
			if (dat[i][j].compare("continuous") == 0)
			{
				break;
			}
			else
			{
				temp.push_back(dat[i][j]);
				// std::cout << dat[i][j] << "\n";
			}
		}
		lt->addAttrInfo(dat[i][0], temp);
	}
	// std::cout << "start3\n";

	// build
	lt->build(getExamples(path, attr_names));
	// std::cout << "start4\n";

	lt->print();
}