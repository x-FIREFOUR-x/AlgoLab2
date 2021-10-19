#include "B_Tree.h"
B_Tree::B_Tree(string file, int t1)
{
	t = t1;
	min_keys = t-1;
	max_keys = t*2 -1;
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
	pop(key, root, nullptr,  swap_element, -1);
	/*if (root->data.size() == 0)
	{
		root = root->ptr_sons[0];
	}*/
	write_BD();
}
void B_Tree::pop(int& key, Node* cur_node, Node* father_ptr,  pair<int, string> swap_element, int pos_deep)
{
	int pos = 0;
	bool there_node = search_node_with_key(cur_node, key, pos);			//позиція елемента, якщо він є в цьому вузлі ,або вказівника на поглиблення далі
	if (there_node)
	{
		if (cur_node->ptr_sons[0]->data.size() == 0)			// чи елемент знаходиться в листі
		{
			if (cur_node->data.size() > min_keys || father_ptr == nullptr)		// видалення коли розмір не мінімальний
			{
				delete_element(cur_node, key, pos);
			}
			else if (pos_deep > 0 && father_ptr->ptr_sons[pos_deep-1]->data.size() > min_keys)		// видаляєм з листа коли його розмір мін а лівого сусіда не мін
			{
				int pos_last_elem = father_ptr->ptr_sons[pos_deep - 1]->data.size() - 1;
				swap_element = father_ptr->ptr_sons[pos_deep - 1]->data[pos_last_elem];					// запам'ятовуєм останній елемент лівого сусіда		
				delete_element(father_ptr->ptr_sons[pos_deep - 1], swap_element.first, pos_last_elem);	// видаляєм цей останній елемент з лівого сусіда

				pair<int, string> buf_element = swap_element;
				swap_element = father_ptr->data[pos_deep - 1];			// запамятовуєм елемент в батьківській ноді між вказівниками на вузол і його лівого сусіда
				father_ptr->data[pos_deep - 1] = buf_element;			// замість цього елемента кладем останній елемент з лівого сусіда

				for (int i = pos; i > 0; i--)							// затираєм елемент який потрібно видалити сусідами з права
				{
					father_ptr->ptr_sons[pos_deep]->data[i] = father_ptr->ptr_sons[pos_deep]->data[i-1];
				}
				father_ptr->ptr_sons[pos_deep]->data[0] = swap_element;			// і кладем в вузол на позицію першого елемента  елемент з батька

			}
			else if (pos_deep < father_ptr->ptr_sons.size() - 1 && father_ptr->ptr_sons[pos_deep+1]->data.size() > min_keys)	// видаляєм з листа коли його розмір мін а правого сусіда не мін
			{
				swap_element = father_ptr->ptr_sons[pos_deep + 1]->data[0];						//запам'ятовуємо перший елемент з правого сусіда
				delete_element(father_ptr->ptr_sons[pos_deep + 1], swap_element.first, 0);		// видаляєм цей перший елемент з правого сусіда

				pair<int, string> buf_element = swap_element;
				swap_element = father_ptr->data[pos_deep];			// запамятовуєм елемент в батьківській ноді між вказівниками на вузол і його правого сусіда
				father_ptr->data[pos_deep] = buf_element;			// замість цього елемента кладем перший елемент з правого сусіда

				int pos_last_elem = father_ptr->ptr_sons[pos_deep]->data.size() - 1;
				for (int i = pos; i < pos_last_elem; i++)					// затираєм елемент який потрібно видалити сусідами з права 
				{
					father_ptr->ptr_sons[pos_deep]->data[i] = father_ptr->ptr_sons[pos_deep]->data[i + 1];
				}
				
				father_ptr->ptr_sons[pos_deep]->data[pos_last_elem] = swap_element;		// і кладем в вузол на позицію останнього елемента елемент з батька
			}
			else		// коли в листі і в правому і лівому сусіді min
			{
				if (pos_deep != 0)			// злиття з лівим сусідом
				{
					swap_element = father_ptr->data[pos_deep - 1];								// спуск елемента з батька в кінець лівого сусіда
					father_ptr->ptr_sons[pos_deep - 1]->data.push_back(swap_element);

					auto it_end_right_node = father_ptr->ptr_sons[pos_deep - 1]->data.end();	// злиття вузла в його лівого сусіда
					auto it_begin_node = father_ptr->ptr_sons[pos_deep]->data.begin();
					auto it_end_node = father_ptr->ptr_sons[pos_deep]->data.end();
					father_ptr->ptr_sons[pos_deep - 1]->data.insert(it_end_right_node, it_begin_node, it_end_node);

		// !!!!змінити на рекурсивний виклик
					father_ptr->ptr_sons[pos_deep]->~Node();							// видалення спущеного вузла в батьку
					auto it_pos_date = father_ptr->data.begin() + pos_deep -1;
					father_ptr->data.erase(it_pos_date);
					auto it_pos_ptr = father_ptr->ptr_sons.begin() + pos_deep;
					father_ptr->ptr_sons.erase(it_pos_ptr);

					while (father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.size() <= father_ptr->ptr_sons[pos_deep - 1]->data.size())
					{
						father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.push_back(new Node);
					}

					search_node_with_key(father_ptr->ptr_sons[pos_deep - 1], key, pos);		// видалення в новому вже вузлі потрібного елемента(старий лівий сусід)
					delete_element(father_ptr->ptr_sons[pos_deep - 1], key, pos);

					
				}
				else			// злиття з правим сусідом
				{
					swap_element = father_ptr->data[pos_deep];										// спуск елемента з батьківського в початок правого сусіда вузла
 					vector<pair<int, string>> buf;
					buf.push_back(swap_element);
					auto it_begin_right_node = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
					auto it_begin_elem = buf.begin();
					auto it_end_elem = buf.end();
					father_ptr->ptr_sons[pos_deep + 1]->data.insert(it_begin_right_node, it_begin_elem, it_end_elem);

					it_begin_right_node = father_ptr->ptr_sons[pos_deep + 1]->data.begin();			// злиття вузла в його правого сусіда
					auto it_begin_node = father_ptr->ptr_sons[pos_deep]->data.begin();
					auto it_end_node = father_ptr->ptr_sons[pos_deep]->data.end();
					father_ptr->ptr_sons[pos_deep + 1]->data.insert(it_begin_right_node, it_begin_node, it_end_node);

		// !!!!змінити на рекурсивний виклик
					father_ptr->ptr_sons[pos_deep]->~Node();							// видалення спущеного елемента в батьківському вузлі
					auto it_pos_date = father_ptr->data.begin() + pos_deep ;
					father_ptr->data.erase(it_pos_date);
					auto it_pos_ptr = father_ptr->ptr_sons.begin() + pos_deep;
					father_ptr->ptr_sons.erase(it_pos_ptr);

					while (father_ptr->ptr_sons[pos_deep]->ptr_sons.size() <= father_ptr->ptr_sons[pos_deep]->data.size())
					{
						father_ptr->ptr_sons[pos_deep]->ptr_sons.push_back(new Node);
					}

					search_node_with_key(father_ptr->ptr_sons[pos_deep], key, pos);			// видалення в новому вже вузлі потрібного елемента (старий правий сусід)
					delete_element(father_ptr->ptr_sons[pos_deep], key, pos);

				}
				
			}
		}
		else   // елемент знаходиться не в листовому вузлі
		{
			if (father_ptr == nullptr && cur_node->data.size() == 1)
			{
				if (cur_node->ptr_sons[0]->data.size())
				{
					pair<int, string> element = cur_node->data[0];
					//Node* f_ptr = nullptr;
					Node* ptr_leet = search_swap_root_left(cur_node, 0, element);

					if (ptr_leet->data.size() > min_keys)
					{
						cur_node->data[0] = element;
						auto it_d = ptr_leet->data.begin() + ptr_leet->data.size() - 1;
						auto it_s = ptr_leet->ptr_sons.begin() + ptr_leet->ptr_sons.size() - 1;
						ptr_leet->data.erase(it_d);
						ptr_leet->ptr_sons.erase(it_s);
					}
					else
					{
						ptr_leet = search_swap_root_left(cur_node, 1, element);
						if (ptr_leet->data.size() > min_keys)
						{
							auto it_d = ptr_leet->data.begin();
							auto it_s = ptr_leet->ptr_sons.begin();
							ptr_leet->data.erase(it_d);
							ptr_leet->ptr_sons.erase(it_s);
						}
					}
						
				}
				else
				{
					root->~Node();
					root = new Node;
				}
				 
			}
			else
			{
				if (cur_node->ptr_sons[pos]->data.size() > min_keys)    // якщо лівий син має більше мін ключів
				{
					int size_left_son = cur_node->ptr_sons[pos]->data.size();
					swap_element = cur_node->ptr_sons[pos]->data[size_left_son - 1];
					cur_node->data[pos] = swap_element;			// на місце елемента що потрібно видалити кладем останній елемент лівого сина

					pop(swap_element.first, cur_node->ptr_sons[pos], cur_node, swap_element, pos);	// викликаєм видалення цього сплившого елемента в лівому сині 
				}
				else if (cur_node->ptr_sons[pos + 1]->data.size() > min_keys)  // якщо правий син має більше мін ключів
				{
					swap_element = cur_node->ptr_sons[pos + 1]->data[0];
					cur_node->data[pos] = swap_element;						// на місце елемента що потрібно видалити кладем перший елемент правого сина

					pop(swap_element.first, cur_node->ptr_sons[pos + 1], cur_node, swap_element, pos + 1);		// викликаєм видалення цього сплившого елемента в правому сині
				}
				else		// якщо правий і лівий син мають мінімальну кількість ключів
				{
					if (cur_node->data.size() > min_keys || (father_ptr == nullptr && cur_node->data.size() > 1))		// якщо вузол має не мінімальну кількість ключів
					{
						swap_element = cur_node->data[pos];
						cur_node->ptr_sons[pos]->data.push_back(swap_element);		// спускаєм потрібний елемент в кінець лівого сина

						auto it_end_left_son = cur_node->ptr_sons[pos]->data.end();
						auto it_begin_right_son = cur_node->ptr_sons[pos + 1]->data.begin();
						auto it_end_right_son = cur_node->ptr_sons[pos + 1]->data.end();
						cur_node->ptr_sons[pos]->data.insert(it_end_left_son, it_begin_right_son, it_end_right_son);   // доєднуєм до лівого сина правого сина

						auto it_end_left_son_ptr = cur_node->ptr_sons[pos]->ptr_sons.end();
						auto it_begin_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.begin();
						auto it_end_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.end();
						cur_node->ptr_sons[pos]->ptr_sons.insert(it_end_left_son_ptr, it_begin_right_son_ptr, it_end_right_son_ptr);	// доєднуєм до вказівників лівого сина вказівники правого сина

						cur_node->ptr_sons[pos + 1]->~Node();					// удаляєм вузол (правий син)
						auto it_date = cur_node->data.begin() + pos;
						auto it_ptr = cur_node->ptr_sons.begin() + pos + 1;
						cur_node->data.erase(it_date);							// уидаляєм цей елемент в вузлі
						cur_node->ptr_sons.erase(it_ptr);						// удаляєм вказівник на правого сина з вузла

						pop(swap_element.first, cur_node->ptr_sons[pos], cur_node, swap_element, pos);		// викликаєм функцію видалення цього ключа для дівого сина

					}
					else    // якщо вузол має мінімальну кількість ключів
					{
						swap_element = cur_node->data[pos];
						cur_node->ptr_sons[pos]->data.push_back(swap_element);		// спускаєм потрібний елемент в кінець лівого сина

						auto it_end_left_son = cur_node->ptr_sons[pos]->data.end();
						auto it_begin_right_son = cur_node->ptr_sons[pos + 1]->data.begin();
						auto it_end_right_son = cur_node->ptr_sons[pos + 1]->data.end();
						cur_node->ptr_sons[pos]->data.insert(it_end_left_son, it_begin_right_son, it_end_right_son);   // доєднуєм до лівого сина правого сина

						auto it_end_left_son_ptr = cur_node->ptr_sons[pos]->ptr_sons.end();
						auto it_begin_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.begin();
						auto it_end_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.end();
						cur_node->ptr_sons[pos]->ptr_sons.insert(it_end_left_son_ptr, it_begin_right_son_ptr, it_end_right_son_ptr);   // доєднуєм до вказівників лівого сина вказівники правого сина

						cur_node->ptr_sons[pos + 1]->~Node();					// удаляєм вузол (правий син)
						auto it_date = cur_node->data.begin() + pos;
						auto it_ptr = cur_node->ptr_sons.begin() + pos + 1;
						cur_node->data.erase(it_date);							// уидаляєм цей елемент в вузлі
						cur_node->ptr_sons.erase(it_ptr);						// удаляєм вказівник на правого сина з вузла

						pop(swap_element.first, cur_node->ptr_sons[pos], cur_node, swap_element, pos);		// викликаєм функцію видалення цього ключа для дівого сина
					}


				}
			}
				
			
		}
	}
	else
	{
		pop(key, cur_node->ptr_sons[pos], cur_node, swap_element, pos);
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
	int ptr_pos;
	if (pos == node_key->data.size() - 1)
		ptr_pos = pos + 1;
	else
		ptr_pos = pos;

	auto it_d = node_key->data.begin();
	node_key->data.erase(it_d + pos);

	
	auto it_s = node_key->ptr_sons.begin();
	node_key->ptr_sons.erase(it_s + ptr_pos);
}
Node* B_Tree::search_swap_root_left(Node* cur_node, int pos, pair<int, string>& element)
{
	Node* ptr_leet;
	if (cur_node->ptr_sons[pos]->data.size() != 0)
	{
		
		int pos_next =  cur_node->ptr_sons[pos]->ptr_sons.size() - 1;
		ptr_leet = search_swap_root_left(cur_node->ptr_sons[pos], pos_next, element);
	}
	else
	{
		pair<int, string> buf = element;
		element = cur_node->data[cur_node->data.size() - 1];
		cur_node->data[cur_node->data.size() - 1] = buf;
		return cur_node;
	}
	return ptr_leet;
}
Node* search_swap_root_right(Node* cur_node, int pos, pair<int, string>& element)
{
	Node* ptr_leet;
	if (cur_node->ptr_sons[pos]->data.size() != 0)
	{

		int pos_next = 0;
		ptr_leet = search_swap_root_right(cur_node->ptr_sons[pos], pos_next, element);
	}
	else
	{
		pair<int, string> buf = element;
		element = cur_node->data[0];
		cur_node->data[0] = buf;
		return cur_node;
	}
	return ptr_leet;
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