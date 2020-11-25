#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>

using namespace std;
const int CHILDREN_NUM = 50;
struct  Node
{
	int lable = -1;
	int attr = -1;
	vector<int> attr_value;
	vector<int> index;
	Node* children[CHILDREN_NUM] = { NULL };
};

class DesicionTree
{
public:
	DesicionTree();
	~DesicionTree();
	void OutputTree();
	vector<int> Predict(vector<int> index);
	friend ostream& operator<<(ostream& out, map<int, string>& obj);
	friend ostream& operator<<(ostream& out, vector<map<string, int>>& obj);
	friend ostream& operator<<(ostream& out, vector<vector<int>>& obj);
	friend ostream& operator<<(ostream& out, vector<int>& obj);
private:
	Node* root;
	map<int, string> attrMap;
	vector< map<string, int>> attrValueMap;
	vector<vector<int>> Data;
	int length;
	void Init(string path = "D:\\作业\\大三上\\人工智能\\第一次作业\\西瓜集.txt");
	int ChooseAttr(vector<int> index, vector<int> attr);
	vector<vector<int>> Divide(vector<int> index, int attr);
	vector<int> Count(vector<int> index, int attr);
	double Ent(vector<int> pk);
	double Gain(vector<int> index, int attr);
	Node* BulidTree(vector<int> index, vector<int> attr);
	void Output(Node* node);
	bool IsValueSame(vector<int> index);
	bool IsTypeSame(vector<int> index);
	bool Isnum(string s){
		stringstream sin(s);
		double t;
		if (!(sin >> t))return false;
		else return true;
	}
};

