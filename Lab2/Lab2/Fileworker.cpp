#include "Fileworker.h"
void Fileworker::write_BD(string filename, Node* root, int t)
{
	ofstream fout;
	fout.open(filename);

	string line = to_string(t);
	fout << line << "\n";

	line = "";
	queue<Node*> que;
	que.push(root);

	while (!que.empty() && que.front()->data.size() != 0)
	{
		Node* cur_node = que.front();
		que.pop();
		for (int i = 0; i < cur_node->data.size(); i++)
		{
			line += to_string(cur_node->data[i].first) + ":" + cur_node->data[i].second + ";";
		}
		line += "\n";
		for (int i = 0; i < cur_node->ptr_sons.size(); i++)
		{
			que.push(cur_node->ptr_sons[i]);
		}

		fout << line;
		line = "";
	}
}


Node* Fileworker::read_BD(string filename, int& t, int& min_keys, int& max_keys, int& size)
{
	ifstream fin;
	fin.open(filename);
	//cout << fin.is_open() << endl;

	string line;
	fin >> line;
	t = stoi(line);
	min_keys = t - 1;
	max_keys = t * 2 - 1;

	line = "";
	fin >> line;
	queue<Node*> que;
	Node* node = new Node();
	Node* root = node;
	que.push(node);

	int key;
	string value;

	while (line != "")
	{
		Node* cur_node = que.front();
		que.pop();
		int cursor = 0;
		while (cursor < line.size())
		{
			key = parsing_key(line, cursor);
			value = parsing_value(line, cursor);
			cur_node->data.push_back(pair<int, string>(key, value));
			size++;
		}
		for (int i = 0; i < cur_node->data.size() + 1; i++)
		{
			node = new Node();
			cur_node->ptr_sons.push_back(node);
			que.push(cur_node->ptr_sons[i]);
		}
		line = "";
		fin >> line;
	}

	return root;
}
int Fileworker::parsing_key(string& line, int& cursor)
{
	int i = line.find(":", cursor);
	int key = stoi(line.substr(cursor, i - cursor));
	cursor = i + 1;
	return key;
}
string Fileworker::parsing_value(string& line, int& cursor)
{
	int i = line.find(";", cursor);
	string value = line.substr(cursor, i - cursor);
	cursor = i + 1;
	return value;
}

