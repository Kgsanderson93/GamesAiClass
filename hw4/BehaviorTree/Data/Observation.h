#include "../behaviorTree/Behavior.h"

class LearningTreeNode {
	public:
		LearningTreeNode();
		void setAttrName(const std::string& attr_name);
		std::string getAttrName();
		void setType(const std::string& type);
		std::string getType();
		Behavior * behavior;
	protected:
		std::string attr_name;
		std::string type; // one of 'discrete', 'continuous' and 'leaf'
};

class DiscAttrLearningTreeNode: public LearningTreeNode {
	public:
		DiscAttrLearningTreeNode();

		// attr_val must be a possible value of attr_name
		LearningTreeNode*& operator[](const std::string& attr_val);
		std::vector<LearningTreeNode*> getChildPointers();
	private:
		std::unordered_map<std::string, LearningTreeNode*> child;
};

class ContAttrLearningTreeNode: public LearningTreeNode {
	public:
		ContAttrLearningTreeNode();

		LearningTreeNode*& operator[](const double& attr_val);

		// this must be called before using operator[]
		void setDividers(const std::vector<double> dividers);

		// return value 'r' is such that attr_val will belong to child[r]
		int getIndex(double attr_val);

		std::vector<LearningTreeNode*> getChildPointers();
	private:
		std::vector<double> dividers;
		std::vector<LearningTreeNode*> child;
};

class Instance {
	public:
		Instance();

		// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
		Instance(
			const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals);

		// Used to access value of a particular attribute
		std::string operator[](const std::string& attr_name);

	protected:
		std::unordered_map<std::string, std::string> els;
};

class Example: public Instance {
	public:
		Example();

		// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
		Example(
			const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals,
			const std::string& target_class);

		std::string getTargetClass() const;

	private:
		std::string target_class;
};

class LearningTree {
	public:
		// Used to populate 'pos_vals'
		void addAttrInfo(const std::string& attr_name,
			const std::vector<std::string>& attr_vals);

		// Used to populate 'target_values'
		void addTargetValues(std::set<std::string> target_values);

		// 'addAttrInfo' and 'addTargetValues' must be used before calling
		// this function
		void build(const std::vector<Example>& train_data);

		// 'build' must be called before calling this function
		void prune(const std::vector<Example>& validation_data);

		// 'build' must be called before calling this function
		double test(const std::vector<Example>& test_data);

		void print();

	private:
		// Returns the target value given to the instance 'inst' using the
		// already built decision tree
		std::string classify(const Instance& inst);

		// Used by public 'build'
		void build(std::vector<Example> train_data, LearningTreeNode*& p,
			std::vector<std::string> check_attr, int depth);

		// Returns information gain
		// 'attr_name' must be a discrete-valued attribute
		double discInfoGain(std::vector<Example> els, const std::string& attr_name);

		// Returns a pair (info gain, vector of dividers)
		// 'attr_name' must be a continous-valued attribute
		std::pair<double, std::vector<double>> contInfoGain(
			std::vector<Example> els, const std::string& attr_name);

		// Returns entropy given a map
		// (target value, num of occurrences of that target value)
		double calcEntropy(const std::map<std::string, int>& els);

		void print(LearningTreeNode *p);

		// key is attribute
		// value is vector of all possible values for that attribute
		// If the attribute is continuous then value will be empty vector
		std::unordered_map<std::string, std::vector<std::string> > pos_vals;

		LearningTreeNode *root;
		std::set<std::string> target_values;
};

class Reader {
	public:
		// file name and number of attributes
		static std::vector<std::vector<std::string> > readData(std::string fileloc);
		static std::set <std::string> readTargetVal(std::string fileloc, int n);
};

void LearningTreeBuilder(std::string attPath, std::string path, LearningTree *lt);

