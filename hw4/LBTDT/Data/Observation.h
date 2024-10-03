#pragma once
#include "../decisionTree/GameTreeNode.h"
#include "../behaviorTree/Behavior.h"

class LearningTreeNodeLeaf : public GameTreeAction
{
public:
	LearningTreeNodeLeaf();
	std::string targetName;
	// one of 'discrete', 'continuous' and 'leaf'
};
class LearningTreeDecisionNode : public GameTreeDecision
{
	public:
	std::string attr_name;
	std::string attr_val;

};

class DiscAttrLearningTreeNode : public LearningTreeDecisionNode
{
public:
	DiscAttrLearningTreeNode();
};

class ContAttrLearningTreeNode : public LearningTreeDecisionNode
{
public:
	ContAttrLearningTreeNode();
	// the dividing val turns this into a boolean. false ==below, true==above;
};

class Instance
{
public:
	Instance();

	// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
	Instance(
		const std::vector<std::string> &attr_names, const std::vector<std::string> &attr_vals);

	// Used to access value of a particular attribute
	std::string operator[](const std::string &attr_name);

protected:
	std::unordered_map<std::string, std::string> els;
};

class Example : public Instance
{
public:
	Example();

	// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
	Example(
		const std::vector<std::string> &attr_names, const std::vector<std::string> &attr_vals,
		const std::string &target_class);

	std::string getTargetClass() const;

private:
	std::string target_class;
};

class LearningTree
{
public:
	LearningTree(std::string attpath, std::string valpath);
	void getExamples(const std::string &fileloc, const std::vector<std::string> &attr_names);
	// Used to populate 'pos_vals'
	void addAttrInfo(const std::string &attr_name,
					 const std::vector<std::string> &attr_vals);

	// Used to populate 'target_values'
	void addTargetValues(std::set<std::string> target_values);

	// 'addAttrInfo' and 'addTargetValues' must be used before calling
	// this function
	void build();

	// 'build' must be called before calling this function
	void prune(const std::vector<Example> &validation_data);

	// 'build' must be called before calling this function
	double test(const std::vector<Example> &test_data);

	void print(DecisionTreeNode *n, float indent);
	void print();
	std::string RandomAction();


	// Returns the target value given to the instance 'inst' using the
	// already built decision tree
	//Behavior classify(const Instance &inst);

	// Used by public 'build'
	DecisionTreeNode *build(std::vector<Example> train_data,
							std::vector<std::string> check_attr, float depth);

	// Returns information gain
	// 'attr_name' must be a discrete-valued attribute
	std::pair<float, std::string>  discInfoGain(std::vector<Example> els, const std::string &attr_name);

	// Returns a pair (info gain, vector of dividers)
	// 'attr_name' must be a continous-valued attribute
	std::pair<float, float> contInfoGain(
		std::vector<Example> els, const std::string &attr_name);

	// Returns entropy given a map
	// (target value, num of occurrences of that target value)
	double calcEntropy(const std::map<std::string, float> &els);


	// key is attribute
	// value is vector of all possible values for that attribute
	// If the attribute is continuous then value will be empty vector
	std::unordered_map<std::string, std::vector<std::string>> pos_vals;
	LearningTreeDecisionNode *root;
	std::set<std::string> target_values;
	std::vector<DiscAttrLearningTreeNode *> discDeci;
	std::vector<ContAttrLearningTreeNode *> contDeci;
	std::vector<LearningTreeNodeLeaf *> leaves;
	std::vector<Example> data;
};

class Reader
{
public:
	// file name and number of attributes
	static std::vector<std::vector<std::string>> readData(std::string fileloc);
	static std::set<std::string> readTargetVal(std::string fileloc, float n);
};

void LearningTreeBuilder(std::string attPath, std::string path, LearningTree *lt);
