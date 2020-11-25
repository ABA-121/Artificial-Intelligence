#include "DesicionTree.h"

/*
* 从path读取数据集，初试化length，attrMap，Data，attrValueMap
*/
ostream& operator<<(ostream& out, vector<vector<int>>& obj)
{
	int i = 0;
	while (i < obj.size())
	{
		int j = 0;
		while (j < obj[i].size())
		{
			cout << obj[i][j] << " ";
			j++;
		}
		cout << endl;
		i++;
	}
	return out;
}
ostream& operator<<(ostream& out, vector<map<string, int>>& obj)
{
	int i = 0;
	while (i < obj.size())
	{
		map<string, int >::iterator j = obj[i].begin();
		while (j != obj[i].end())
		{
			cout << j->first << " " << j->second << endl;
			j++;
		}
		cout << endl << endl;
		i++;
	}
	return out;
}
ostream& operator<<(ostream& out, map<int, string>& obj)
{
	map<int, string >::iterator j = obj.begin();
	while (j != obj.end())
	{
		cout << j->first << " " << j->second << endl;
		j++;
	}
	return out;
}
ostream& operator<<(ostream& out, vector<int>& obj)
{
	int j = 0;
	while (j < obj.size())
	{
		cout << obj[j] << endl;
		j++;
	}
	return out;
}
void DesicionTree::Init(string path) {
	ifstream fin;
	string line,word;
	fin.open(path);
	int n = 0;
	while (getline(fin, line)) {
		istringstream sin(line);
		map<string, int> row;
		vector<int> vRow;
		int m = 0;
		map<string, int>::iterator it;
		while (getline(sin,word,','))
		{
			if (n == 0) {
				attrMap.insert(map<int, string>::value_type(m, word));
				attrValueMap.push_back(row);
			}
			else
			{
				attrValueMap[m].insert((map<string, int>::value_type(word, attrValueMap[m].size())));
				it = attrValueMap[m].find(word);
				vRow.push_back(it->second);
			}
			m++;
		}
		if (n !=0)
		{
			Data.push_back(vRow);
			vRow.clear();
		}
		n++;
	}
	length = attrMap.size();
}
vector<vector<int>> DesicionTree::Divide(vector<int> index, int attr) {
	int i = 0;
	vector<vector<int>> divide;
	vector<int> subSet;
	while (i++ < attrValueMap[attr].size()) divide.push_back(subSet);
	i = 0;
	while (i < index.size())
	{
		divide[Data[index[i]][attr]].push_back(index[i]);
		i++;
	}
	return divide;
}
vector<int> DesicionTree::Count(vector<int> index, int attr) {
	int i = 0;
	vector<int> pk;
	while (i++ < attrValueMap[attr].size()) pk.push_back(0);
	i = 0;
	while (i < index.size())
	{
		pk[Data[index[i]][attr]]++;
		i++;
	}
	return pk;
}
double DesicionTree::Ent(vector<int> pk) {
	
	int i = 0;
	double ent = 0;
	double num = 0;
	while (i <pk.size())
	{
		if(pk[i] > 0)num += pk[i];
		i++;
	}
	i = 0;
	while (i < pk.size())
	{
		if (pk[i] != 0) {
			double scale = pk[i] / num;
			ent -= scale * log(scale) / log(2);
		}
		i++;
	}
	return ent;
}
double DesicionTree::Gain(vector<int> index, int attr) {
	double entD = Ent(Count(index,length - 1));
	vector<vector<int>> divide = Divide(index, attr);
	int i = 0;
	double entDv = 0;
	double d = index.size();
	while (i<divide.size())
	{
		entDv += Ent(Count(divide[i], length - 1))* divide[i].size()/d;
		i++;
	}
	return entD - entDv;
}
/*
* 返回选择属性的索引值，当属性集为空时返回-1；
*/
int DesicionTree::ChooseAttr(vector<int> index, vector<int> attr) {
	int i = 0;
	if (attr.size()== 0) return -1;
	double bestGain =0.0;
	double nowGain = 0.0;
	int bestAttrIndex = 0;
	while (i < attr.size())
	{
		nowGain = Gain(index, attr[i]);
		if (bestGain < nowGain)
		{
			bestGain = nowGain;
			bestAttrIndex = i;
		}
		i++;
	}
	return bestAttrIndex;
}
bool DesicionTree::IsValueSame(vector<int> index) {
	for (int i = 1; i < index.size(); i++) {
		for (int j = 0; j < length - 1; j++)
		{
			if (Data[index[0]][j] != Data[index[i]][j])
				return false;
		}
	}
	return true;
}
bool DesicionTree::IsTypeSame(vector<int> index) {
	for (int i = 1; i < index.size(); i++) {
		if (Data[index[0]][length - 1] != Data[index[i]][length - 1])
			return false;
	}
	return true;
}
Node* DesicionTree::BulidTree(vector<int> index, vector<int> attr) {
	Node* node = new Node;
	node->attr_value = attr;
	node->index = index;
	if (IsTypeSame(index)) {
		node->lable = Data[index[0]][length - 1];
		return node;
	}
	if ((attr.size() == 0) || IsValueSame(index)) {
		vector<int> count = Count(index, length - 1);
		int num = 0;
		for (int i = 0; i < count.size(); i++)
		{
			if (num < count[i]) {
				num = count[i];
				node->lable = i;
			}
		}
		return node;
	}
	int attrIndex = ChooseAttr(index, attr); 
	node->attr = attr[attrIndex];
	int childrenNum = attrValueMap[node->attr].size();
	vector<vector<int>> divide = Divide(index, node->attr);
	while (childrenNum-- > 0)
	{
		if (divide[childrenNum].size() != 0)
		{
			vector<int> childAttr = attr;
			
			childAttr.erase(childAttr.begin() + attrIndex);
			node->children[childrenNum] = BulidTree(divide[childrenNum], childAttr);
		}
	}
	return node;
}
void DesicionTree::Output(Node* node)
{
	cout << endl << "DesicionTree:" << endl;
	if (node != NULL)
	{
		cout << node->attr << endl;
		if (node->attr >= 0)
		{
			int attrLenth = attrValueMap[node->attr].size();
			while (attrLenth--)
			{
				if (node->children[attrLenth] != NULL)
					Output(node->children[attrLenth]);
			}
		}
	}
}
void DesicionTree::OutputTree() {
	Output(root);
}
DesicionTree::DesicionTree() {
	Init();
	vector<int> index, attr;
	int i = 1;
	while (i < Data.size()-1)
	{
		if (i < attrMap.size()-1&& i!=7&&i!=8) attr.push_back(i);
		index.push_back(i++);
	}
	root = BulidTree(index, attr);
}
DesicionTree::~DesicionTree() { 
	Node* temp;
	while (root->children != NULL)
	{
		break;
	}
}
vector<int> DesicionTree::Predict(vector<int> index) {
	vector<int> unknown, result;
	Node* node = NULL;
	for (int i = 0; i < index.size(); i++)
	{
		unknown = Data[index[i]];
		node = root;
		while (node->lable == -1)
		{
			node = node->children[unknown[node->attr]];	
		}
		result.push_back(node->lable);
	}
	cout << result;
	return result;
}
