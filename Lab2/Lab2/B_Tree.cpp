#include "B_Tree.h"
B_Tree::B_Tree(string file)
{
	
	filename = file;
	Fileworker f;
	root = f.read_BD(filename, t, min_keys, max_keys);
}


string B_Tree::search(int key)
{
	string value = "";
	Node* cur_node = root;
	
	value = binary_search(cur_node, key);
	
	return value;
}
Node* B_Tree::get_root()
{
	return root;
}
string B_Tree::binary_search(Node*& cur_node, const int key)
{
	
	string value = "";
	int start = 0; 
	int end = cur_node->data.size() - 1;
	int middle;
	while (end != start)
	{
		middle = ((end - start) / 2) + start;
		if(key < cur_node->data[middle].first)
		{
			end = middle;
		}
		else
		{
			if (key > cur_node->data[middle].first)
			{
				start = middle + 1;
			}
			else
			{
				start = middle;
				end = middle;
			}
		}
	}
	if (key == cur_node->data[start].first)
	{
		value = cur_node->data[start].second;
		return value;
	}
	else
	{
		if (cur_node->ptr_sons[0]->data.size() != 0)
		{
			if (key > cur_node->data[start].first)
			{
				value = binary_search(cur_node->ptr_sons[start + 1], key);
			}
			else
			{
				value = binary_search(cur_node->ptr_sons[start], key);
			}
		}
		else
		{
			value == "";
			return value;
		}
		
	}
	
	return value;
}



void B_Tree::push(int key, string value, bool& element_add_success)
{
	bool element_rise = true;
	Node* add_ptr = new Node;
	search_node(root, key, value, element_rise, add_ptr, element_add_success);

	Fileworker f;
	f.write_BD(filename, root, t);	
}
void B_Tree::search_node(Node*& curent_node, int& key, string& value, bool& element_rise, Node*& add_ptr, bool& element_add_success)
{
	bool is_this_element = false;
	int pos_in;
	if (curent_node->ptr_sons[0]->data.size() != 0)		// якщо не дійшли до листа заглиблюємося далі
	{
		pos_in = search_pos_insert(curent_node, key, is_this_element);	// запамятовуєм в якому місці заглиблюємося
		if (!is_this_element)
		{
			search_node(curent_node->ptr_sons[pos_in], key, value, element_rise, add_ptr, element_add_success);
		}
		else
		{
			element_add_success = false;
			element_rise = false;
		}
		
	}
	
	if (element_rise)
	{
		int pos = search_pos_insert(curent_node, key, is_this_element);
		if (!is_this_element)
		{
			if (curent_node->data.size() < max_keys)			// вставка без розбиття вузла
			{

				insert_element(curent_node, pos, key, value, add_ptr);
				element_rise = false;
			}
			else											// вставка з розбиттям вузла і спливання елемента вгору
			{
				Node* n1 = new Node;
				Node* n2 = new Node;
				cell_node(curent_node, n1, n2);			// розбиваєм вузол на два n1, n2


				if (pos <= curent_node->data.size() / 2 )		// визначаєм куди вставим новий елемент в n1 чи n2
				{
					insert_element(n1, pos, key, value, add_ptr);
				}
				else
				{
					pos = pos - (curent_node->data.size() / 2) - 1;
					insert_element(n2, pos, key, value, add_ptr);
				}
				// елемент що всплив потрібно вставити в вузол що є батьком від поточного    
				key = curent_node->data[curent_node->data.size() / 2].first;			// ключем елемента що вставляється є ключ елемента що всплив
				value = curent_node->data[curent_node->data.size() / 2].second;			// значення елемента що вставляється є значення елемента що всплив
				curent_node = n1;														// вказівник що вказував на поточний тепер вказує на першу половину вузла що був розбитий
				add_ptr = n2;															// добавлений вказівник підчас добавлення спливаючого елемента вказує на другу половину вузла що був розбитий
			}
		}
		else
		{
			element_add_success = false;
			element_rise = false;
		}
	
	}
	
}

int B_Tree::search_pos_insert(Node* cur_node, int key, bool& is_this_key)
{
	int pos = 0;
	int start = 0;
	int end = cur_node->data.size() - 1;
	int middle;
	while (end != start)
	{
		middle = ((end - start) / 2) + start;
		if (key < cur_node->data[middle].first)
		{
			end = middle;
		}
		else
		{
			if (key > cur_node->data[middle].first)
			{
				start = middle + 1;
			}
			else
			{
				start = middle;
				end = middle;
			}
		}
	}

	if (key == cur_node->data[start].first)				
	{
		is_this_key = true;
		pos = start;
	}
	else
	{
		if (key > cur_node->data[start].first)
		{
				pos = start + 1;
		}
		else
		{
				pos = start;
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
	for (int i = curent_node->ptr_sons.size() / 2; i < curent_node->ptr_sons.size(); i++)
	{
		n2->ptr_sons.push_back(curent_node->ptr_sons[i]);
	}
}



bool B_Tree::pop(int key)
{
	pair<int, string> swap_element;
	int side = 0;
	bool lift_del = false;
	bool descent = true;
	bool is_key = true;
	pop(key, is_key, root, nullptr,  swap_element, -1, lift_del, side, descent);
	
	Fileworker f;
	f.write_BD(filename, root, t);

	if (is_key)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void B_Tree::pop(int& key, bool& is_key, Node* cur_node, Node* father_ptr,  pair<int, string> swap_element, int pos_deep, bool& lift_del, int& side, bool& descent)
{
	
	int pos = 0;
	bool there_node = search_node_with_key(cur_node, key, pos);			//позиція елемента, якщо він є в цьому вузлі ,або вказівника на поглиблення далі
	if (descent)
	{
		if (there_node)
		{
					//чи елемент знаходиться в листі
			if (cur_node->ptr_sons[0]->data.size() == 0)			
			{
						//  видалення коли розмір не мінімальний
				if (cur_node->data.size() > min_keys || father_ptr == nullptr)  
				{
					delete_element(cur_node, key, pos);
				}
						// видаляєм з листа коли його розмір мін а лівого сусіда не мін
				else if (pos_deep > 0 && father_ptr->ptr_sons[pos_deep - 1]->data.size() > min_keys) 
				{
					int pos_last_elem = father_ptr->ptr_sons[pos_deep - 1]->data.size() - 1;
					swap_element = father_ptr->ptr_sons[pos_deep - 1]->data[pos_last_elem];					// запам'ятовуєм останній елемент лівого сусіда		
					delete_element(father_ptr->ptr_sons[pos_deep - 1], swap_element.first, pos_last_elem);	// видаляєм цей останній елемент з лівого сусіда

					pair<int, string> buf_element = swap_element;
					swap_element = father_ptr->data[pos_deep - 1];			// запамятовуєм елемент в батьківській ноді між вказівниками на вузол і його лівого сусіда
					father_ptr->data[pos_deep - 1] = buf_element;			// замість цього елемента кладем останній елемент з лівого сусіда

					for (int i = pos; i > 0; i--)							// затираєм елемент який потрібно видалити сусідами з права
					{
						father_ptr->ptr_sons[pos_deep]->data[i] = father_ptr->ptr_sons[pos_deep]->data[i - 1];
					}
					father_ptr->ptr_sons[pos_deep]->data[0] = swap_element;			// і кладем в вузол на позицію першого елемента  елемент з батька

				}
						// видаляєм з листа коли його розмір мін а правого сусіда не мін
				else if (pos_deep < father_ptr->ptr_sons.size() - 1 && father_ptr->ptr_sons[pos_deep + 1]->data.size() > min_keys)	
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
						// коли в листі і в правому і лівому сусіді min
				else		
				{
							// злиття з лівим сусідом
					if (pos_deep != 0)			
					{
						swap_element = father_ptr->data[pos_deep - 1];								// спуск елемента з батька в кінець лівого сусіда
						father_ptr->ptr_sons[pos_deep - 1]->data.push_back(swap_element);

						auto it_end_right_node = father_ptr->ptr_sons[pos_deep - 1]->data.end();	// злиття вузла в його лівого сусіда
						auto it_begin_node = father_ptr->ptr_sons[pos_deep]->data.begin();
						auto it_end_node = father_ptr->ptr_sons[pos_deep]->data.end();
						father_ptr->ptr_sons[pos_deep - 1]->data.insert(it_end_right_node, it_begin_node, it_end_node);

						auto it_end_right_ptr = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.end();	// злиття вказівників вузла в його лівого сусіда
						auto it_begin_ptr = father_ptr->ptr_sons[pos_deep]->ptr_sons.begin();
						auto it_end_ptr = father_ptr->ptr_sons[pos_deep]->ptr_sons.end();
						father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.insert(it_end_right_ptr, it_begin_ptr, it_end_ptr);

						search_node_with_key(father_ptr->ptr_sons[pos_deep - 1], key, pos);		// видалення в новому вже вузлі потрібного елемента(старий лівий сусід)
						delete_element(father_ptr->ptr_sons[pos_deep - 1], key, pos);

						key = swap_element.first; // ключ елемента якого потрібно видалити в батьківському вузлі так як він уже в сині
						side = 1; // видалити потрібно правий вказівник
						lift_del = true;

					}
							// злиття з правим сусідом
					else			
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

						auto it_begin_right_pos = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();			// злиття вказівників вузла в його правого сусіда
						auto it_begin_pos = father_ptr->ptr_sons[pos_deep]->ptr_sons.begin();
						auto it_end_pos = father_ptr->ptr_sons[pos_deep]->ptr_sons.end();
						father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.insert(it_begin_right_pos, it_begin_pos, it_end_pos);


						search_node_with_key(father_ptr->ptr_sons[pos_deep+1], key, pos);			// видалення в новому вже вузлі потрібного елемента (старий правий сусід)
						delete_element(father_ptr->ptr_sons[pos_deep+1], key, pos);

						key = swap_element.first; // ключ елемента якого потрібно видалити в батьківському вузлі так як він уже в сині
						side = -1; // видалити потрібно лівий вказівник
						lift_del = true;


					}

				}
			}
					// елемент знаходиться не в листовому вузлі
			else   
			{
						// видалення елемента з кореня коли його розмір мінімальний
				if (father_ptr == nullptr && cur_node->data.size() == 1)			
				{
							// якщо елемент в корені не єдиний в дереві
					if (cur_node->ptr_sons[0]->data.size() > 0)			
					{
						pair<int, string> element;
						//Node* f_ptr = nullptr;
						Node* ptr_leet = search_elem_left_subroot(cur_node, 0, element);		// находим самий правий вузол лівого піддерева 

								// якщо правий лист лівого піддерева більше мін
						if (ptr_leet->data.size() > min_keys)					
						{
							cur_node->data[0] = element;						// заміняєм видаляємий елемент з кореня на останній елемент з листа
							
							ptr_leet->data.pop_back();
							ptr_leet->ptr_sons.pop_back();
						}
						else   
						{
							ptr_leet = search_elem_right_subroot(cur_node, 1, element);		// находим самий лівий вузол правого піддерева
								// якщо лівий лист правого піддерева більше мін
							if (ptr_leet->data.size() > min_keys)				// якщо лівий лист правого піддерева більше мін
							{
								cur_node->data[0] = element;				// заміняєм видаляємий елемент з кореня на перший елемент з листа

								auto it_d = ptr_leet->data.begin();			// видаляєм цей елемент з листа
								auto it_s = ptr_leet->ptr_sons.begin();
								ptr_leet->data.erase(it_d);
								ptr_leet->ptr_sons.erase(it_s);
							}
								// якщо правий лист лівого піддерева мін і лівий лист правого піддерева мін
							else				
							{
								pair<int, string> root_elem = cur_node->data[pos];
								int key_leet = element.first;
								pop(key_leet, is_key, root, nullptr, swap_element, 0, lift_del, side, descent);     // виклик видалення елемента в листі який ми помістим в корінь

								lift_del = false;		// збивання флажків які могли активуватися в процедурі видалення елемента з листа
								descent = true;
								side = 0;

								Node* node_root_elem = root;
								node_root_elem = search_root_element(node_root_elem, root_elem.first, pos);	// пошук вузла і позиція елемента що був у корені
								node_root_elem->data[pos] = element;						// заміна елемента в вузлі який ми мали видалити на той що видалили в листі
							}
						}

					}
							// якщо елемент єдиний в дереві
					else					
					{
						root->~Node();
						root = new Node;
					}

				}
						// не корінь
				else			
				{		
							// якщо лівий син має більше мін ключів
					if (cur_node->ptr_sons[pos]->data.size() > min_keys)   
					{
						int size_left_son = cur_node->ptr_sons[pos]->data.size();
						swap_element = cur_node->ptr_sons[pos]->data[size_left_son - 1];
						cur_node->data[pos] = swap_element;			// на місце елемента що потрібно видалити кладем останній елемент лівого сина

						pop(swap_element.first, is_key, cur_node->ptr_sons[pos], cur_node, swap_element, pos, lift_del, side, descent);	// викликаєм видалення цього сплившого елемента в лівому сині 
					}
							// якщо правий син має більше мін ключів
					else if (cur_node->ptr_sons[pos + 1]->data.size() > min_keys) 
					{
						swap_element = cur_node->ptr_sons[pos + 1]->data[0];
						cur_node->data[pos] = swap_element;						// на місце елемента що потрібно видалити кладем перший елемент правого сина

						pop(swap_element.first, is_key, cur_node->ptr_sons[pos + 1], cur_node, swap_element, pos + 1, lift_del, side, descent);		// викликаєм видалення цього сплившого елемента в правому сині
					}
							// якщо правий і лівий син мають мінімальну кількість ключів
					else		
					{
								// якщо вузол має не мінімальну кількість ключів
						if (cur_node->data.size() > min_keys || (father_ptr == nullptr && cur_node->data.size() > 1))		
						{
							swap_element = cur_node->data[pos];
							cur_node->ptr_sons[pos]->data.push_back(swap_element);		// спускаєм потрібний елемент в кінець лівого сина

							to_left_son_add_right_son(cur_node, pos);   // до лівого сина доєднуєм правого

							cur_node->ptr_sons[pos + 1]->~Node();					// удаляєм вузол (правий син)
							auto it_date = cur_node->data.begin() + pos;
							auto it_ptr = cur_node->ptr_sons.begin() + pos + 1;
							cur_node->data.erase(it_date);							// уидаляєм цей елемент в вузлі
							cur_node->ptr_sons.erase(it_ptr);						// удаляєм вказівник на правого сина з вузла

							pop(swap_element.first, is_key, cur_node->ptr_sons[pos], cur_node, swap_element, pos, lift_del, side, descent);		// викликаєм функцію видалення цього ключа для лівого сина

						}
								// якщо вузол має мінімальну кількість ключів (100, 50) (сини теж)
						else   
						{
							
							if (pos_deep > 0 && father_ptr->ptr_sons[pos_deep-1]->data.size()> min_keys)   // якщо лівий сусід не мін
							{
								to_left_son_add_right_son(cur_node, pos);   // до лівого сина доєднуєм правого
							
								for (int i = pos; i > 0; i--)							// зміщуєм елементи вправо знищуючи видаляємий елемент
									cur_node->data[i] = cur_node->data[i - 1];
								cur_node->ptr_sons[pos + 1]->~Node();
								for (int i = pos + 1; i > 0; i--)						// зміщуєм вказівники вправо знищуючи вказівник на правого сина 
									cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
				
								cur_node->data[0] = father_ptr->data[pos_deep - 1];					// на місце першого елемента  поточного вузла(пустий) кладем елемент в батьківському вузлі між вказівниками на вузол і його лівого сусіда

								int size_left_neightbor_d = father_ptr->ptr_sons[pos_deep - 1]->data.size();
								father_ptr->data[pos_deep - 1] = father_ptr->ptr_sons[pos_deep - 1]->data[size_left_neightbor_d - 1];			// на місце елемента в батьку кладем останній елемент лівого сусіда

								int size_left_neightbor_s = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.size();					
								cur_node->ptr_sons[0] = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons[size_left_neightbor_s - 1];		// на місце першого вказівника поточного елемента кладем останній лівого сусіда
								
								father_ptr->ptr_sons[pos_deep - 1]->data.pop_back();				// зменшуєм розмір лівого сусіда (видаляєм останній елемент і вказівник)
								father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.pop_back();
							}
								// якщо правий сусід не мін
							else if (pos_deep < father_ptr->ptr_sons.size()-1 && father_ptr->ptr_sons[pos_deep + 1]->data.size()> min_keys) 
							{
								to_left_son_add_right_son(cur_node, pos);   // до лівого сина доєднуєм правого

								for (int i = pos; i < cur_node->data.size() - 1; i++)					// зміщуєм елементи вліво знищуючи видаляємий елемент
									cur_node->data[i] = cur_node->data[i + 1];
								cur_node->ptr_sons[pos + 1]->~Node();
								for (int i = pos + 1; i < cur_node->ptr_sons.size() - 1; i++)			// зміщуєм вказівники вліво знищуючи вказівник на правого сина 
									cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];

								cur_node->data[cur_node->data.size() - 1] = father_ptr->data[pos_deep];					// на місце останнього елемента  елемента кладем елемент в батьківському вузлі між вказівниками на поточного вузла і правого сусіда
								
								father_ptr->data[pos_deep] = father_ptr->ptr_sons[pos_deep + 1]->data[0];				// на місце елемента в батьку кладем перший елемент в правому сусіду

								cur_node->ptr_sons[cur_node->ptr_sons.size() - 1] = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons[0];		// на місце останнього вказівника поточного вузла кладем перший вказівник правого сусіда

								auto it_right_neightbor_d = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
								auto it_right_neightbor_s = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
								father_ptr->ptr_sons[pos_deep + 1]->data.erase(it_right_neightbor_d);						// видаляєм перший елемент і вказівник правого сусіда
								father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.erase(it_right_neightbor_s);

							}	
								// якщо обидва сусіди мін
							else		
							{
									// зливаєм з лівим сусідом (приєднуєм лівого сусіда)
								if (pos_deep > 0)			
								{
									to_left_son_add_right_son(cur_node, pos);		// до лівого сина доєднуєм правого
									
									for (int i = pos; i > 0; i--)							// зміщуєм елементи вправо знищуючи видаляємий елемент
										cur_node->data[i] = cur_node->data[i - 1];
									cur_node->ptr_sons[pos + 1]->~Node();
									for (int i = pos + 1; i > 0; i--)						// зміщуєм вказівники вправо знищуючи вказівник на правого сина 
										cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];

									swap_element = father_ptr->data[pos_deep - 1];
									cur_node->data[0] = swap_element;					// спускаєм елемент з батька в вузол на перше місце

									auto it_begin_node_ptr = cur_node->ptr_sons.begin();	
									cur_node->ptr_sons.erase(it_begin_node_ptr);			// видаляєм перший вказівник вузла (він пустий)

									auto it_begin_node_data = cur_node->data.begin();
									auto it_begin_left_data = father_ptr->ptr_sons[pos_deep - 1]->data.begin();
									auto it_end_left_data = father_ptr->ptr_sons[pos_deep - 1]->data.end();
									cur_node->data.insert(it_begin_node_data, it_begin_left_data, it_end_left_data);   // приєднуєм елементи лівого сусіда до вузла

									it_begin_node_ptr = cur_node->ptr_sons.begin();
									auto it_begin_left_ptr = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.begin();
									auto it_end_left_ptr = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.end();
									cur_node->ptr_sons.insert(it_begin_node_ptr, it_begin_left_ptr, it_end_left_ptr);   // приєднуєм вказівники лівого сусіда до вузла

									key = swap_element.first; // ключ елемента якого потрібно видалити в батьківському вузлі так як він уже в поточному вузлі 
									side = -1; // видалити потрібно лівий вказівник
									lift_del = true;
								}
									// зливаєм з правим сусідом (приєднуєм правого сусіда)
								else			
								{
									to_left_son_add_right_son(cur_node, pos);		 // до лівого сина доєднуєм правого

									for (int i = pos; i < cur_node->data.size() - 1; i++)					// зміщуєм елементи вліво знищуючи видаляємий елемент
										cur_node->data[i] = cur_node->data[i + 1];
									cur_node->ptr_sons[pos + 1]->~Node();
									for (int i = pos + 1; i < cur_node->ptr_sons.size() - 1; i++)			// зміщуєм вказівники вліво знищуючи вказівник на правого сина 
										cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];

									swap_element = father_ptr->data[pos_deep];
									cur_node->data[cur_node->data.size()-1] = swap_element;					// спускаєм елемент з батька в вузол на останнє місце

									cur_node->ptr_sons.pop_back();		// видаляєм останній вказівник вузла (він пустий)

									auto it_end_node_data = cur_node->data.end();
									auto it_begin_left_data = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
									auto it_end_left_data = father_ptr->ptr_sons[pos_deep + 1]->data.end();
									cur_node->data.insert(it_end_node_data, it_begin_left_data, it_end_left_data);   // приєднуєм елементи правого сусіда до вузла

									auto it_end_node_ptr = cur_node->ptr_sons.end();
									auto it_begin_left_ptr = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
									auto it_end_left_ptr = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.end();
									cur_node->ptr_sons.insert(it_end_node_ptr, it_begin_left_ptr, it_end_left_ptr);   // приєднуєм вказівники правого сусіда до вузла

									key = swap_element.first; // ключ елемента якого потрібно видалити в батьківському вузлі так як він уже в поточному вузлі 
									side = 1; // видалити потрібно правий вказівник
									lift_del = true;
								}
								
							}
							
							
						}


					}
				}


			}
		}
		else
		{
			if (cur_node->ptr_sons[pos]->data.size() != 0)				// чи далі є вузли
			{
				pop(key, is_key, cur_node->ptr_sons[pos], cur_node, swap_element, pos, lift_del, side, descent);
			}
			else
			{
				is_key = false;
			}
			
		}
	}
	if (descent == false)
	{
		if (lift_del && side != 0)		// видалення спливаючого елемента
		{
			lift_deletion(key, is_key, cur_node, father_ptr, swap_element, pos_deep, lift_del, side, descent);
		}
	}
	

	if (lift_del)
	{
		descent = false;
	}
	
}

void B_Tree::lift_deletion(int& key, bool& is_key, Node* cur_node, Node* father_ptr, pair<int, string> swap_element, int pos_deep, bool& lift_del, int& side, bool& descent)
{
	int pos;
		// якщо поточний вузол це корінь
	if (father_ptr == nullptr)		
	{
		search_node_with_key(cur_node, key, pos);	// позиція елемента який потрібно видалити

			// в корені один елемент
		if (cur_node->data.size() == 1)     
		{
			Node* buf = root;
			if (side == 1)
			{
				cur_node->ptr_sons[1]->~Node();
				root = cur_node->ptr_sons[0];				// правий покажчик потрібно видалити а одже лівий покажчик тепер вказує на корінь
			}
			else
			{
				cur_node->ptr_sons[0]->~Node();
				root = cur_node->ptr_sons[1];					// лівий покажчик потрібно видалити а одже правий покажчик тепер вказує на корінь
			}
			buf->~Node();
		}
			// в корені декілька елементів
		else   
		{
			int ptrdel_r_l;	// число що визначає чи лівий чи правий поінтер затирається (цей поінтер вказує на елемент що в глибшому рівні рекурсії був злитий в сусіда)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i > 0; i--)							// зсуваєм елементи вправо заміщаючи той який нада видалити
			{
				cur_node->data[i] = cur_node->data[i - 1];
			}
			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i > 0; i--)				// зсуваєм вказівники вправо заміщаючи той який нада видалити (side)
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
			}
			auto it_begin_data = cur_node->data.begin();
			auto it_begin_ptr = cur_node->ptr_sons.begin();

			cur_node->data.erase(it_begin_data);			// видаляєм непотрібний перший елемент
			cur_node->ptr_sons.erase(it_begin_ptr);			// видаляєм не потрібний перший покажчик
		}
	}
		// якщо поточний вузол це не корінь
	else      
	{
		search_node_with_key(cur_node, key, pos);	// позиція елемента який потрібно видалити

			// в цьому вузлі не мін елементів
		if (cur_node->data.size() > min_keys)		
		{
			int ptrdel_r_l;	// число що визначає чи лівий чи правий поінтер затирається (цей поінтер вказує на елемент що в глибшому рівні рекурсії був злитий в сусіда)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i > 0; i--)					// зсуваєм вправо елементи ноди затираючи елемент який нада видалити
			{
				cur_node->data[i] = cur_node->data[i - 1];
			}

			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i > 0; i--)				// зсуваєм вправо вказівники ноди затираючи вказівник який нада видалити
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
			}

			// видаляєм перший вказівник і значення (зменшуєм розмір)
			auto it_begin_data = cur_node->data.begin();
			cur_node->data.erase(it_begin_data);
			auto it_begin_ptr = cur_node->ptr_sons.begin();
			cur_node->ptr_sons.erase(it_begin_ptr);

			lift_del = false;	// більше не потрібно нічого видаляти
			side = 0;
		}
			// лівий сусід не мін, батько мін
		else if (pos_deep > 0 && father_ptr->ptr_sons[pos_deep - 1]->data.size() > min_keys)		
		{
			swap_element = father_ptr->data[pos_deep - 1];  // елемент який ми спускаєм з батька на заміну видаляємого з вузла

			int ptrdel_r_l;	// число що визначає чи лівий чи правий поінтер затирається (цей поінтер вказує на елемент що в глибшому рівні рекурсії був злитий в сусіда)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i > 0; i--)					// зсовуєм елементи вправо заміщаючи видаляємий
			{
				cur_node->data[i] = cur_node->data[i - 1];
			}
			cur_node->data[0] = swap_element;				// кладем на місце першого елемента вузла (з якого видаляєм) елемент з батька між цим вузлом і лівим сусідом

			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i > 0; i--)			// зсовуєм вказівники вправо починаючи з правого чи лівого поінтер (side)
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
			}
			int in_last_p_left = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.size() - 1;	// індекс останнього поінтера лівого сусіда
			cur_node->ptr_sons[0] = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons[in_last_p_left];	// на місце першого поінтера в вузлі кладемо останній з лівого сусіда

			int in_last_d_left = father_ptr->ptr_sons[pos_deep - 1]->data.size() - 1;		// індекс останнього елемента лівого сусіда

			father_ptr->data[pos_deep - 1] = father_ptr->ptr_sons[pos_deep - 1]->data[in_last_d_left]; // на місце елемента в батьківському вузлі між вузлом і лівим сусідом кладем останній елемент лівого сусіда

			father_ptr->ptr_sons[pos_deep - 1]->data.pop_back();			// видаляєм останній елемент і поінтер лівого сусіда
			father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.pop_back();

			lift_del = false;	// більше не потрібно нічого видаляти
			side = 0;
		}
			// правий сусід не мін, батько мін
		else if (pos_deep < father_ptr->ptr_sons.size() - 1 && father_ptr->ptr_sons[pos_deep + 1]->data.size() > min_keys)  
		{
			swap_element = father_ptr->data[pos_deep];  // елемент який ми спускаєм з батька на заміну видаляємого елемента з вузла

			int ptrdel_r_l;	// число що визначає чи лівий чи правий поінтер затирається (цей поінтер вказує на елемент що в глибшому рівні рекурсії був злитий в сусіда)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i < cur_node->data.size() - 1; i++)					// зсовуєм елементи вліво заміщаючи видаляємий
			{
				cur_node->data[i] = cur_node->data[i + 1];
			}
			cur_node->data[cur_node->data.size() - 1] = swap_element;				// кладем на місце постаннього елемента вузла (з якого видаляєм) елемент з батька між цим вузлом і правим сусідом

			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i < cur_node->ptr_sons.size() - 1; i++)			// зсовуєм вказівники вліво починаючи з правого чи лівого поінтер (side)
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];
			}
			cur_node->ptr_sons[cur_node->ptr_sons.size() - 1] = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons[0];	// на місце першого поінтера в вузлі кладемо останній з лівого сусіда

			father_ptr->data[pos_deep] = father_ptr->ptr_sons[pos_deep + 1]->data[0]; // на місце елемента в батьківському вузлі між вузлом і правим сусідом кладем перший елемент правого сусіда


			auto it_begin_data_right = father_ptr->ptr_sons[pos_deep + 1]->data.begin();			// видаляєм перший елемент і поінтер правого сусіда
			auto it_begin_ptr_right = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
			father_ptr->ptr_sons[pos_deep + 1]->data.erase(it_begin_data_right);
			father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.erase(it_begin_ptr_right);

			lift_del = false;	// більше не потрібно нічого видаляти
			side = 0;
		}
			// лівий і правий сусід мін батько мін
		else        
		{
			if (pos_deep > 0)	//зливаєм з лівим сусідом (лівого доєднуєм)
			{
				int ptrdel_r_l;	// число що визначає чи лівий чи правий поінтер затирається (цей поінтер вказує на елемент що в глибшому рівні рекурсії був злитий в сусіда)
				if (side == 1)
					ptrdel_r_l = 1;
				else if (side == -1)
					ptrdel_r_l = 0;

				swap_element = father_ptr->data[pos_deep - 1];			// берем елемент(між вузлом і лівим сусідом) з батька що спускається в вузол
				for (int i = pos; i > 0; i--)							// зсовуєм елементи вправо заміщуючи видалений 
				{
					cur_node->data[i] = cur_node->data[i - 1];
				}
				cur_node->data[0] = swap_element;	// на перший елемент ставимо упущений елемент з батька

				cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
				for (int i = pos + ptrdel_r_l; i < 0; i--)			// зсовуєм вказівники вправо заміщуючи видалений  (side лівий чи правий)
				{
					cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
				}
				auto it_begin_ptr = cur_node->ptr_sons.begin();
				cur_node->ptr_sons.erase(it_begin_ptr);					// вирізаєм не потрібний перший вказівник

				auto it_begin_data_node = cur_node->data.begin();
				auto it_begin_data_left = father_ptr->ptr_sons[pos_deep - 1]->data.begin();
				auto it_end_data_left = father_ptr->ptr_sons[pos_deep - 1]->data.end();
				cur_node->data.insert(it_begin_data_node, it_begin_data_left, it_end_data_left); // доєднуєм елементи лівого сусіда до вузла

				it_begin_ptr = cur_node->ptr_sons.begin();
				auto it_begin_ptr_left = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.begin();
				auto it_end_ptr_left = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.end();
				cur_node->ptr_sons.insert(it_begin_ptr, it_begin_ptr_left, it_end_ptr_left);		// доєднуєм вказівники лівого сусіда до вузла

				lift_del = true;				// рекурсивний підйом з видалення продовжується
				side = -1;						// лівий вказівник вузла батька відносно видаляємого ключа далі теж видалити
				key = swap_element.first;		// ключ що переміщений з батька в вузол потрібно видалити в батьку
			}
				//зливаєм з правим сусідом (правого доєднуєм)
			else		
			{
				int ptrdel_r_l;	// число що визначає чи лівий чи правий поінтер затирається (цей поінтер вказує на елемент що в глибшому рівні рекурсії був злитий в сусіда)
				if (side == 1)
					ptrdel_r_l = 1;
				else if (side == -1)
					ptrdel_r_l = 0;

				swap_element = father_ptr->data[pos_deep];			// берем елемент(між вузлом і правим сусідом) з батька що спускається в вузол
				for (int i = pos; i > cur_node->data.size() - 1; i++)							// зсовуєм елементи вліво заміщуючи видалений 
				{
					cur_node->data[i] = cur_node->data[i + 1];
				}
				cur_node->data[cur_node->data.size() - 1] = swap_element;	// на останній елемент ставимо упущений елемент з батька

				cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
				for (int i = pos + ptrdel_r_l; i > cur_node->ptr_sons.size() - 1; i++)			// зсовуєм вказівники вліво заміщуючи видалений  (side лівий чи правий)
				{
					cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];
				}
				//auto it_end_ptr = cur_node->ptr_sons.begin() + cur_node->ptr_sons.size() - 1;
				cur_node->ptr_sons.pop_back();				// вирізаєм не потрібний останній вказівник

				auto it_end_data_node = cur_node->data.end();
				auto it_begin_data_left = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
				auto it_end_data_left = father_ptr->ptr_sons[pos_deep + 1]->data.end();
				cur_node->data.insert(it_end_data_node, it_begin_data_left, it_end_data_left);		// доєднуєм елементи правого сусіда до вузла

				auto it_end_ptr = cur_node->ptr_sons.end();
				auto it_begin_ptr_left = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
				auto it_end_ptr_left = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.end();
				cur_node->ptr_sons.insert(it_end_ptr, it_begin_ptr_left, it_end_ptr_left);        // доєднуєм вказівники правого сусіда до вузла

				lift_del = true;				// рекурсивний підйом з видалення продовжується
				side = 1;						// правий вказівник вузла батька відносно видаляємого ключа далі теж видалити
				key = swap_element.first;		// ключ що переміщений з батька в вузол потрібно видалити в батьку
			}
		}


	}
}

bool B_Tree::search_node_with_key(Node* cur_node, int key, int& pos)
{	

	bool is_search = false;
	int start = 0;
	int end = cur_node->data.size() - 1;
	int middle;
	while (end != start)
	{
		middle = ((end - start) / 2) + start;
		if (key < cur_node->data[middle].first)
		{
			end = middle;
		}
		else
		{
			if (key > cur_node->data[middle].first)
			{
				start = middle + 1;
			}
			else
			{
				start = middle;
				end = middle;
			}
		}
	}

	if (key == cur_node->data[start].first)
	{
		is_search = true;
		pos = start;
	}
	else
	{
		if (key > cur_node->data[start].first)
		{
			pos = start + 1;
		}
		else
		{
			pos = start;
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
Node* B_Tree::search_elem_left_subroot(Node* cur_node, int pos_next_deep, pair<int, string>& element)
{
	Node* ptr_leet;
	if (cur_node->ptr_sons[pos_next_deep]->data.size() != 0)
	{
		
		int pos_next =  cur_node->ptr_sons[pos_next_deep]->ptr_sons.size() - 1;
		ptr_leet = search_elem_left_subroot(cur_node->ptr_sons[pos_next_deep], pos_next, element);
	}
	else
	{
		element = cur_node->data[cur_node->data.size() - 1];
		return cur_node;
	}
	return ptr_leet;
}
Node* B_Tree::search_elem_right_subroot(Node* cur_node, int pos_next_deep, pair<int, string>& element)
{
	Node* ptr_leet;
	if (cur_node->ptr_sons[pos_next_deep]->data.size() != 0)
	{

		int pos_next = 0;
		ptr_leet = search_elem_right_subroot(cur_node->ptr_sons[pos_next_deep], pos_next, element);
	}
	else
	{
		element = cur_node->data[0];
		return cur_node;
	}
	return ptr_leet;
}
void B_Tree::to_left_son_add_right_son(Node* cur_node, int pos)
{
	auto it_end_left_son = cur_node->ptr_sons[pos]->data.end();
	auto it_begin_right_son = cur_node->ptr_sons[pos + 1]->data.begin();
	auto it_end_right_son = cur_node->ptr_sons[pos + 1]->data.end();
	cur_node->ptr_sons[pos]->data.insert(it_end_left_son, it_begin_right_son, it_end_right_son);   // доєднуєм до лівого сина правого сина

	auto it_end_left_son_ptr = cur_node->ptr_sons[pos]->ptr_sons.end();
	auto it_begin_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.begin();
	auto it_end_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.end();
	cur_node->ptr_sons[pos]->ptr_sons.insert(it_end_left_son_ptr, it_begin_right_son_ptr, it_end_right_son_ptr);	// доєднуєм до вказівників лівого сина вказівники правого сина

}
Node* B_Tree::search_root_element(Node* cur_node, int key, int& pos)
{
	Node* node;
	bool is_elem = search_node_with_key(cur_node, key, pos);
	if (!is_elem)
	{
		node = search_root_element(cur_node->ptr_sons[pos], key, pos);
	}
	else
	{
		return cur_node;
	}
	
	return node;
}




