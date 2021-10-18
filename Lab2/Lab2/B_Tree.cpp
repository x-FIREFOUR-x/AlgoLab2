#include "B_Tree.h"
B_Tree::B_Tree(string file, int t1)
{
	t = t1;
	min_keys = t - 1;
	max_keys = 2 * t - 1;
	filename = file;
	read_BD();
}


string B_Tree::search(int key)
{
	string value = "";
	Node* cur_node = root;
	while (cur_node->data.size() != 0 && value == "" )
	{
		value = search_in_node(cur_node, key);
	}
	return value;
}
string B_Tree::search_in_node(Node*& cur_node, const int key)
{
	string value = "";
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
			cur_node = cur_node->ptr_sons[i + 1];
			break;
		}
	}
	return value;
}



void B_Tree::push(int key, string value)
{
	bool element_rise = true;
	Node* add_ptr = new Node;
	search_node(root, key, value, element_rise, add_ptr);
	write_BD();
}
void B_Tree::search_node(Node*& curent_node, int& key, string& value, bool& element_rise, Node*& add_ptr)
{
	int pos_in;
	if (curent_node->ptr_sons[0]->data.size() != 0)		// якщо не дійшли до листа заглиблюємося далі
	{
		pos_in = search_pos_insert(curent_node, key);	// запамятовуєм в якому місці заглиблюємося
		search_node(curent_node->ptr_sons[pos_in], key, value, element_rise, add_ptr);
	}
	
	if (element_rise)
	{
		if (curent_node->data.size() < max_keys)			// вставка без розбиття вузла
		{
			int pos = search_pos_insert(curent_node, key);
			insert_element(curent_node, pos, key, value, add_ptr);
			element_rise = false;
		}
		else											// вставка з розбиттям вузла і спливання елемента вгору
		{
			Node* n1 = new Node;
			Node* n2 = new Node;
			cell_node(curent_node, n1, n2);			// розбиваєм вузол на два n1, n2

			int pos = search_pos_insert(curent_node, key);	// місце для вставлення вузла у ще не розбитому вузлі
			if (pos <= curent_node->data.size() / 2)		// визначаєм куди вставим новий елемент в n1 чи n2
			{
				insert_element(n1, pos, key, value, add_ptr);
			}
			else
			{
				pos = pos - curent_node->data.size() - 1;
				insert_element(n2, pos, key, value, add_ptr);
			}
							// елемент що всплив потрібно вставити в вузол що є батьком від поточного    
			key = curent_node->data[curent_node->data.size() / 2].first;			// ключем елемента що вставляється є ключ елемента що всплив
			value = curent_node->data[curent_node->data.size() / 2].second;			// значення елемента що вставляється є значення елемента що всплив
			curent_node = n1;														// вказівник що вказував на поточний тепер вказує на першу половину вузла що був розбитий
			add_ptr = n2;															// добавлений вказівник підчас добавлення спливаючого елемента вказує на другу половину вузла що був розбитий
		}
	}
	
}

int B_Tree::search_pos_insert(Node* curent_node, int key)
{
	int pos = 0;
	for (int i = 0; i < curent_node->data.size(); i++)
	{
		if (key < curent_node->data[i].first)
		{
			pos = i;
			break;
		}
		if (i == curent_node->data.size() - 1 && key > curent_node->data[i].first)
		{
			pos = i + 1;
			break;
		}
	}
	return pos;
}
void B_Tree::insert_element(Node*& curent_node, int pos, int key, string value, Node*& add_ptr)
{
	auto it = curent_node->data.begin();
	it = it + pos;
	curent_node->data.insert(it, pair<int, string>(key, value));

	auto it2 = curent_node->ptr_sons.begin();
	it2 = it2 + pos + 1;
	//Node* node = new Node;
	curent_node->ptr_sons.insert(it2, add_ptr);
}
void B_Tree::cell_node(Node*& curent_node, Node*& n1, Node*& n2)
{
	for (int i = 0; i < curent_node->data.size() / 2; i++)
	{
		n1->data.push_back(pair<int, string>(curent_node->data[i].first, curent_node->data[i].second));
	}
	for (int i = 0; i < curent_node->ptr_sons.size() / 2; i++)
	{
		n1->ptr_sons.push_back(curent_node->ptr_sons[i]);
	}
	for (int i = curent_node->data.size() / 2 + 1; i < curent_node->data.size(); i++)
	{
		n2->data.push_back(pair<int, string>(curent_node->data[i].first, curent_node->data[i].second));
	}
	for (int i = curent_node->ptr_sons.size() / 2 + 1; i < curent_node->ptr_sons.size(); i++)
	{
		n2->ptr_sons.push_back(curent_node->ptr_sons[i]);
	}
}



void B_Tree::pop(int key)
{
	pair<int, string> swap_element;
	search_delete_key(root, key, "non", swap_element);
	if (root->data.size() == 0)
	{
		root = root->ptr_sons[0];
	}
}
void B_Tree::search_delete_key(Node*& cur_node, int& key, string side, pair<int, string> swap_element)
{
	if (cur_node != nullptr)
	{
		bool is_key_in_tree = false;
		int pos;
		int situation = 0;
		if (is_key_in_tree || side != "non")
		{
			if (is_key_in_tree)
				swap_element = cur_node->data[pos - 1];
			if (side == "right")
				pos = cur_node->data.size();
			if (side == "left")
				pos = 0;
			if(cur_node->ptr_sons[pos] == nullptr)
			
		}
	}
}


bool B_Tree::search_node_with_key(Node* cur_node, int key, int& pos)
{	
	bool is_search = false;
	for (int i = 0; i < cur_node->data.size(); i++)
	{
		if (key == cur_node->data[i].first)
		{
			pos = i;
			is_search = true;
			break;
		}
		else
		{
			if (key < cur_node->data[i].first)
			{
				pos = i;
				break;
			}
		}
		if (i == cur_node->data.size() - 1 && key > cur_node->data[i].first)
		{
			pos = i + 1;
			break;
		}
	}
	return is_search;
}
void B_Tree::delete_element(Node*& node_key, int key, int pos)
{
	auto it_d = node_key->data.begin();
	auto it_s = node_key->ptr_sons.begin();
	node_key->data.erase(it_d + pos);
	node_key->ptr_sons.erase(it_s + pos);
}


void B_Tree::write_BD()
{
	ofstream fout;
	fout.open(filename);

	string line = "";
	queue<Node*> que;
	que.push(root);

	while (!que.empty())
	{
		Node* cur_node = que.front();
		que.pop();
		for (int i = 0; i < cur_node->data.size(); i++)
		{
			line += to_string(cur_node->data[i].first) + ":" + cur_node->data[i].second + ";" ;
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


void B_Tree::read_BD()
{
	ifstream fin;
	fin.open(filename);
	//cout << fin.is_open() << endl;

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