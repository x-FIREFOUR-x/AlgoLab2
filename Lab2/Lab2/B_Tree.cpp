#include "B_Tree.h"
B_Tree::B_Tree(string file, int t1)
{
	t = t1;
	min_keys = t - 1;
	max_keys = 2 * t - 1;
	filename = file;
	read_BD();
}



void B_Tree::console_write()
{
	TLR(root);
}

void B_Tree::TLR(Node* p)
{
	if (p)
	{
		for (int i = 0; i < p->data.size(); i++)
		{
			cout << p->data[i].first << ":";
			cout << p->data[i].second << " ";
		}
		cout << endl;
		for (int i = 0; i < p->ptr_sons.size(); i++)
		{
			TLR(p->ptr_sons[i]);
		}
	
	}
}

string B_Tree::search(int key)
{
	string value = "";
	Node* cur_node = root;
	while (cur_node->data.size() != 0 && value == "" )
	{
		for (int i = 0; i < cur_node->data.size(); i++)
		{
			if (key == cur_node->data[i].first)
			{
				value = cur_node->data[i].second;
				break;
			}
			else
			{
				if (key < cur_node->data[i].first)
				{
					cur_node = cur_node->ptr_sons[i];
					break;
				}
			}
			if (i == cur_node->data.size() - 1 && key > cur_node->data[i].first)
			{
				cur_node = cur_node->ptr_sons[i+1];
				break;
			}
		}
	}

	return value;
}

void B_Tree::read_BD()
{
	ifstream fin;
	fin.open(filename);
	cout << fin.is_open() << endl;

	string line;
	fin >> line;
	queue<Node*> que;
	Node* node = new Node();
	root = node;
	que.push(node);

	int key;
	string value;
	
	while (line !="")
	{
		Node* cur_node = que.front();
		que.pop();
		int cursor = 0;
		while (cursor < line.size())
		{
			key = parsing_key(line, cursor);
			value = parsing_value(line, cursor);
			cur_node->data.push_back(pair<int, string>(key, value));
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
	
}
int B_Tree::parsing_key(string& line, int& cursor)
{
	int i = line.find(":", cursor);
	int key = stoi(line.substr(cursor, i - cursor));
	cursor = i + 1;
	return key;
}
string B_Tree::parsing_value(string& line, int& cursor)
{
	int i = line.find(";", cursor);
	string value = line.substr(cursor, i - cursor);
	cursor = i + 1;
	return value;
}

