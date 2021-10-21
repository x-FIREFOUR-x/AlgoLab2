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
	if (curent_node->ptr_sons[0]->data.size() != 0)		// ���� �� ����� �� ����� ������������� ���
	{
		pos_in = search_pos_insert(curent_node, key, is_this_element);	// ����������� � ����� ���� �������������
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
			if (curent_node->data.size() < max_keys)			// ������� ��� �������� �����
			{

				insert_element(curent_node, pos, key, value, add_ptr);
				element_rise = false;
			}
			else											// ������� � ��������� ����� � ��������� �������� �����
			{
				Node* n1 = new Node;
				Node* n2 = new Node;
				cell_node(curent_node, n1, n2);			// �������� ����� �� ��� n1, n2


				if (pos <= curent_node->data.size() / 2 )		// �������� ���� ������� ����� ������� � n1 �� n2
				{
					insert_element(n1, pos, key, value, add_ptr);
				}
				else
				{
					pos = pos - (curent_node->data.size() / 2) - 1;
					insert_element(n2, pos, key, value, add_ptr);
				}
				// ������� �� ������ ������� �������� � ����� �� � ������� �� ���������    
				key = curent_node->data[curent_node->data.size() / 2].first;			// ������ �������� �� ������������ � ���� �������� �� ������
				value = curent_node->data[curent_node->data.size() / 2].second;			// �������� �������� �� ������������ � �������� �������� �� ������
				curent_node = n1;														// �������� �� �������� �� �������� ����� ����� �� ����� �������� ����� �� ��� ��������
				add_ptr = n2;															// ���������� �������� ����� ���������� ����������� �������� ����� �� ����� �������� ����� �� ��� ��������
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
	bool there_node = search_node_with_key(cur_node, key, pos);			//������� ��������, ���� �� � � ����� ���� ,��� ��������� �� ����������� ���
	if (descent)
	{
		if (there_node)
		{
					//�� ������� ����������� � ����
			if (cur_node->ptr_sons[0]->data.size() == 0)			
			{
						//  ��������� ���� ����� �� ���������
				if (cur_node->data.size() > min_keys || father_ptr == nullptr)  
				{
					delete_element(cur_node, key, pos);
				}
						// �������� � ����� ���� ���� ����� �� � ����� ����� �� ��
				else if (pos_deep > 0 && father_ptr->ptr_sons[pos_deep - 1]->data.size() > min_keys) 
				{
					int pos_last_elem = father_ptr->ptr_sons[pos_deep - 1]->data.size() - 1;
					swap_element = father_ptr->ptr_sons[pos_deep - 1]->data[pos_last_elem];					// �����'������ ������� ������� ����� �����		
					delete_element(father_ptr->ptr_sons[pos_deep - 1], swap_element.first, pos_last_elem);	// �������� ��� ������� ������� � ����� �����

					pair<int, string> buf_element = swap_element;
					swap_element = father_ptr->data[pos_deep - 1];			// ����������� ������� � ���������� ��� �� ����������� �� ����� � ���� ����� �����
					father_ptr->data[pos_deep - 1] = buf_element;			// ������ ����� �������� ������ ������� ������� � ����� �����

					for (int i = pos; i > 0; i--)							// ������� ������� ���� ������� �������� ������� � �����
					{
						father_ptr->ptr_sons[pos_deep]->data[i] = father_ptr->ptr_sons[pos_deep]->data[i - 1];
					}
					father_ptr->ptr_sons[pos_deep]->data[0] = swap_element;			// � ������ � ����� �� ������� ������� ��������  ������� � ������

				}
						// �������� � ����� ���� ���� ����� �� � ������� ����� �� ��
				else if (pos_deep < father_ptr->ptr_sons.size() - 1 && father_ptr->ptr_sons[pos_deep + 1]->data.size() > min_keys)	
				{
					swap_element = father_ptr->ptr_sons[pos_deep + 1]->data[0];						//�����'������� ������ ������� � ������� �����
					delete_element(father_ptr->ptr_sons[pos_deep + 1], swap_element.first, 0);		// �������� ��� ������ ������� � ������� �����

					pair<int, string> buf_element = swap_element;
					swap_element = father_ptr->data[pos_deep];			// ����������� ������� � ���������� ��� �� ����������� �� ����� � ���� ������� �����
					father_ptr->data[pos_deep] = buf_element;			// ������ ����� �������� ������ ������ ������� � ������� �����

					int pos_last_elem = father_ptr->ptr_sons[pos_deep]->data.size() - 1;
					for (int i = pos; i < pos_last_elem; i++)					// ������� ������� ���� ������� �������� ������� � ����� 
					{
						father_ptr->ptr_sons[pos_deep]->data[i] = father_ptr->ptr_sons[pos_deep]->data[i + 1];
					}

					father_ptr->ptr_sons[pos_deep]->data[pos_last_elem] = swap_element;		// � ������ � ����� �� ������� ���������� �������� ������� � ������
				}
						// ���� � ���� � � ������� � ����� ���� min
				else		
				{
							// ������ � ���� ������
					if (pos_deep != 0)			
					{
						swap_element = father_ptr->data[pos_deep - 1];								// ����� �������� � ������ � ����� ����� �����
						father_ptr->ptr_sons[pos_deep - 1]->data.push_back(swap_element);

						auto it_end_right_node = father_ptr->ptr_sons[pos_deep - 1]->data.end();	// ������ ����� � ���� ����� �����
						auto it_begin_node = father_ptr->ptr_sons[pos_deep]->data.begin();
						auto it_end_node = father_ptr->ptr_sons[pos_deep]->data.end();
						father_ptr->ptr_sons[pos_deep - 1]->data.insert(it_end_right_node, it_begin_node, it_end_node);

						auto it_end_right_ptr = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.end();	// ������ ��������� ����� � ���� ����� �����
						auto it_begin_ptr = father_ptr->ptr_sons[pos_deep]->ptr_sons.begin();
						auto it_end_ptr = father_ptr->ptr_sons[pos_deep]->ptr_sons.end();
						father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.insert(it_end_right_ptr, it_begin_ptr, it_end_ptr);

						search_node_with_key(father_ptr->ptr_sons[pos_deep - 1], key, pos);		// ��������� � ������ ��� ���� ��������� ��������(������ ���� ����)
						delete_element(father_ptr->ptr_sons[pos_deep - 1], key, pos);

						key = swap_element.first; // ���� �������� ����� ������� �������� � ������������ ���� ��� �� �� ��� � ���
						side = 1; // �������� ������� ������ ��������
						lift_del = true;

					}
							// ������ � ������ ������
					else			
					{
						swap_element = father_ptr->data[pos_deep];										// ����� �������� � ������������ � ������� ������� ����� �����
						vector<pair<int, string>> buf;
						buf.push_back(swap_element);
						auto it_begin_right_node = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
						auto it_begin_elem = buf.begin();
						auto it_end_elem = buf.end();
						father_ptr->ptr_sons[pos_deep + 1]->data.insert(it_begin_right_node, it_begin_elem, it_end_elem);

						it_begin_right_node = father_ptr->ptr_sons[pos_deep + 1]->data.begin();			// ������ ����� � ���� ������� �����
						auto it_begin_node = father_ptr->ptr_sons[pos_deep]->data.begin();
						auto it_end_node = father_ptr->ptr_sons[pos_deep]->data.end();
						father_ptr->ptr_sons[pos_deep + 1]->data.insert(it_begin_right_node, it_begin_node, it_end_node);

						auto it_begin_right_pos = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();			// ������ ��������� ����� � ���� ������� �����
						auto it_begin_pos = father_ptr->ptr_sons[pos_deep]->ptr_sons.begin();
						auto it_end_pos = father_ptr->ptr_sons[pos_deep]->ptr_sons.end();
						father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.insert(it_begin_right_pos, it_begin_pos, it_end_pos);


						search_node_with_key(father_ptr->ptr_sons[pos_deep+1], key, pos);			// ��������� � ������ ��� ���� ��������� �������� (������ ������ ����)
						delete_element(father_ptr->ptr_sons[pos_deep+1], key, pos);

						key = swap_element.first; // ���� �������� ����� ������� �������� � ������������ ���� ��� �� �� ��� � ���
						side = -1; // �������� ������� ���� ��������
						lift_del = true;


					}

				}
			}
					// ������� ����������� �� � ��������� ����
			else   
			{
						// ��������� �������� � ������ ���� ���� ����� ���������
				if (father_ptr == nullptr && cur_node->data.size() == 1)			
				{
							// ���� ������� � ����� �� ������ � �����
					if (cur_node->ptr_sons[0]->data.size() > 0)			
					{
						pair<int, string> element;
						//Node* f_ptr = nullptr;
						Node* ptr_leet = search_elem_left_subroot(cur_node, 0, element);		// ������� ����� ������ ����� ����� �������� 

								// ���� ������ ���� ����� �������� ����� ��
						if (ptr_leet->data.size() > min_keys)					
						{
							cur_node->data[0] = element;						// ������� ���������� ������� � ������ �� ������� ������� � �����
							
							ptr_leet->data.pop_back();
							ptr_leet->ptr_sons.pop_back();
						}
						else   
						{
							ptr_leet = search_elem_right_subroot(cur_node, 1, element);		// ������� ����� ���� ����� ������� ��������
								// ���� ���� ���� ������� �������� ����� ��
							if (ptr_leet->data.size() > min_keys)				// ���� ���� ���� ������� �������� ����� ��
							{
								cur_node->data[0] = element;				// ������� ���������� ������� � ������ �� ������ ������� � �����

								auto it_d = ptr_leet->data.begin();			// �������� ��� ������� � �����
								auto it_s = ptr_leet->ptr_sons.begin();
								ptr_leet->data.erase(it_d);
								ptr_leet->ptr_sons.erase(it_s);
							}
								// ���� ������ ���� ����� �������� �� � ���� ���� ������� �������� ��
							else				
							{
								pair<int, string> root_elem = cur_node->data[pos];
								int key_leet = element.first;
								pop(key_leet, is_key, root, nullptr, swap_element, 0, lift_del, side, descent);     // ������ ��������� �������� � ���� ���� �� ������� � �����

								lift_del = false;		// �������� ������ �� ����� ������������ � �������� ��������� �������� � �����
								descent = true;
								side = 0;

								Node* node_root_elem = root;
								node_root_elem = search_root_element(node_root_elem, root_elem.first, pos);	// ����� ����� � ������� �������� �� ��� � �����
								node_root_elem->data[pos] = element;						// ����� �������� � ���� ���� �� ���� �������� �� ��� �� �������� � ����
							}
						}

					}
							// ���� ������� ������ � �����
					else					
					{
						root->~Node();
						root = new Node;
					}

				}
						// �� �����
				else			
				{		
							// ���� ���� ��� �� ����� �� ������
					if (cur_node->ptr_sons[pos]->data.size() > min_keys)   
					{
						int size_left_son = cur_node->ptr_sons[pos]->data.size();
						swap_element = cur_node->ptr_sons[pos]->data[size_left_son - 1];
						cur_node->data[pos] = swap_element;			// �� ���� �������� �� ������� �������� ������ ������� ������� ����� ����

						pop(swap_element.first, is_key, cur_node->ptr_sons[pos], cur_node, swap_element, pos, lift_del, side, descent);	// �������� ��������� ����� ��������� �������� � ����� ��� 
					}
							// ���� ������ ��� �� ����� �� ������
					else if (cur_node->ptr_sons[pos + 1]->data.size() > min_keys) 
					{
						swap_element = cur_node->ptr_sons[pos + 1]->data[0];
						cur_node->data[pos] = swap_element;						// �� ���� �������� �� ������� �������� ������ ������ ������� ������� ����

						pop(swap_element.first, is_key, cur_node->ptr_sons[pos + 1], cur_node, swap_element, pos + 1, lift_del, side, descent);		// �������� ��������� ����� ��������� �������� � ������� ���
					}
							// ���� ������ � ���� ��� ����� �������� ������� ������
					else		
					{
								// ���� ����� �� �� �������� ������� ������
						if (cur_node->data.size() > min_keys || (father_ptr == nullptr && cur_node->data.size() > 1))		
						{
							swap_element = cur_node->data[pos];
							cur_node->ptr_sons[pos]->data.push_back(swap_element);		// ������� �������� ������� � ����� ����� ����

							to_left_son_add_right_son(cur_node, pos);   // �� ����� ���� ������ �������

							cur_node->ptr_sons[pos + 1]->~Node();					// ������� ����� (������ ���)
							auto it_date = cur_node->data.begin() + pos;
							auto it_ptr = cur_node->ptr_sons.begin() + pos + 1;
							cur_node->data.erase(it_date);							// �������� ��� ������� � ����
							cur_node->ptr_sons.erase(it_ptr);						// ������� �������� �� ������� ���� � �����

							pop(swap_element.first, is_key, cur_node->ptr_sons[pos], cur_node, swap_element, pos, lift_del, side, descent);		// �������� ������� ��������� ����� ����� ��� ����� ����

						}
								// ���� ����� �� �������� ������� ������ (100, 50) (���� ���)
						else   
						{
							
							if (pos_deep > 0 && father_ptr->ptr_sons[pos_deep-1]->data.size()> min_keys)   // ���� ���� ���� �� ��
							{
								to_left_son_add_right_son(cur_node, pos);   // �� ����� ���� ������ �������
							
								for (int i = pos; i > 0; i--)							// ����� �������� ������ �������� ���������� �������
									cur_node->data[i] = cur_node->data[i - 1];
								cur_node->ptr_sons[pos + 1]->~Node();
								for (int i = pos + 1; i > 0; i--)						// ����� ��������� ������ �������� �������� �� ������� ���� 
									cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
				
								cur_node->data[0] = father_ptr->data[pos_deep - 1];					// �� ���� ������� ��������  ��������� �����(������) ������ ������� � ������������ ���� �� ����������� �� ����� � ���� ����� �����

								int size_left_neightbor_d = father_ptr->ptr_sons[pos_deep - 1]->data.size();
								father_ptr->data[pos_deep - 1] = father_ptr->ptr_sons[pos_deep - 1]->data[size_left_neightbor_d - 1];			// �� ���� �������� � ������ ������ ������� ������� ����� �����

								int size_left_neightbor_s = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.size();					
								cur_node->ptr_sons[0] = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons[size_left_neightbor_s - 1];		// �� ���� ������� ��������� ��������� �������� ������ ������� ����� �����
								
								father_ptr->ptr_sons[pos_deep - 1]->data.pop_back();				// ������� ����� ����� ����� (�������� ������� ������� � ��������)
								father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.pop_back();
							}
								// ���� ������ ���� �� ��
							else if (pos_deep < father_ptr->ptr_sons.size()-1 && father_ptr->ptr_sons[pos_deep + 1]->data.size()> min_keys) 
							{
								to_left_son_add_right_son(cur_node, pos);   // �� ����� ���� ������ �������

								for (int i = pos; i < cur_node->data.size() - 1; i++)					// ����� �������� ���� �������� ���������� �������
									cur_node->data[i] = cur_node->data[i + 1];
								cur_node->ptr_sons[pos + 1]->~Node();
								for (int i = pos + 1; i < cur_node->ptr_sons.size() - 1; i++)			// ����� ��������� ���� �������� �������� �� ������� ���� 
									cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];

								cur_node->data[cur_node->data.size() - 1] = father_ptr->data[pos_deep];					// �� ���� ���������� ��������  �������� ������ ������� � ������������ ���� �� ����������� �� ��������� ����� � ������� �����
								
								father_ptr->data[pos_deep] = father_ptr->ptr_sons[pos_deep + 1]->data[0];				// �� ���� �������� � ������ ������ ������ ������� � ������� �����

								cur_node->ptr_sons[cur_node->ptr_sons.size() - 1] = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons[0];		// �� ���� ���������� ��������� ��������� ����� ������ ������ �������� ������� �����

								auto it_right_neightbor_d = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
								auto it_right_neightbor_s = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
								father_ptr->ptr_sons[pos_deep + 1]->data.erase(it_right_neightbor_d);						// �������� ������ ������� � �������� ������� �����
								father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.erase(it_right_neightbor_s);

							}	
								// ���� ������ ����� ��
							else		
							{
									// ������ � ���� ������ (������� ����� �����)
								if (pos_deep > 0)			
								{
									to_left_son_add_right_son(cur_node, pos);		// �� ����� ���� ������ �������
									
									for (int i = pos; i > 0; i--)							// ����� �������� ������ �������� ���������� �������
										cur_node->data[i] = cur_node->data[i - 1];
									cur_node->ptr_sons[pos + 1]->~Node();
									for (int i = pos + 1; i > 0; i--)						// ����� ��������� ������ �������� �������� �� ������� ���� 
										cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];

									swap_element = father_ptr->data[pos_deep - 1];
									cur_node->data[0] = swap_element;					// ������� ������� � ������ � ����� �� ����� ����

									auto it_begin_node_ptr = cur_node->ptr_sons.begin();	
									cur_node->ptr_sons.erase(it_begin_node_ptr);			// �������� ������ �������� ����� (�� ������)

									auto it_begin_node_data = cur_node->data.begin();
									auto it_begin_left_data = father_ptr->ptr_sons[pos_deep - 1]->data.begin();
									auto it_end_left_data = father_ptr->ptr_sons[pos_deep - 1]->data.end();
									cur_node->data.insert(it_begin_node_data, it_begin_left_data, it_end_left_data);   // ������� �������� ����� ����� �� �����

									it_begin_node_ptr = cur_node->ptr_sons.begin();
									auto it_begin_left_ptr = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.begin();
									auto it_end_left_ptr = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.end();
									cur_node->ptr_sons.insert(it_begin_node_ptr, it_begin_left_ptr, it_end_left_ptr);   // ������� ��������� ����� ����� �� �����

									key = swap_element.first; // ���� �������� ����� ������� �������� � ������������ ���� ��� �� �� ��� � ��������� ���� 
									side = -1; // �������� ������� ���� ��������
									lift_del = true;
								}
									// ������ � ������ ������ (������� ������� �����)
								else			
								{
									to_left_son_add_right_son(cur_node, pos);		 // �� ����� ���� ������ �������

									for (int i = pos; i < cur_node->data.size() - 1; i++)					// ����� �������� ���� �������� ���������� �������
										cur_node->data[i] = cur_node->data[i + 1];
									cur_node->ptr_sons[pos + 1]->~Node();
									for (int i = pos + 1; i < cur_node->ptr_sons.size() - 1; i++)			// ����� ��������� ���� �������� �������� �� ������� ���� 
										cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];

									swap_element = father_ptr->data[pos_deep];
									cur_node->data[cur_node->data.size()-1] = swap_element;					// ������� ������� � ������ � ����� �� ������ ����

									cur_node->ptr_sons.pop_back();		// �������� ������� �������� ����� (�� ������)

									auto it_end_node_data = cur_node->data.end();
									auto it_begin_left_data = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
									auto it_end_left_data = father_ptr->ptr_sons[pos_deep + 1]->data.end();
									cur_node->data.insert(it_end_node_data, it_begin_left_data, it_end_left_data);   // ������� �������� ������� ����� �� �����

									auto it_end_node_ptr = cur_node->ptr_sons.end();
									auto it_begin_left_ptr = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
									auto it_end_left_ptr = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.end();
									cur_node->ptr_sons.insert(it_end_node_ptr, it_begin_left_ptr, it_end_left_ptr);   // ������� ��������� ������� ����� �� �����

									key = swap_element.first; // ���� �������� ����� ������� �������� � ������������ ���� ��� �� �� ��� � ��������� ���� 
									side = 1; // �������� ������� ������ ��������
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
			if (cur_node->ptr_sons[pos]->data.size() != 0)				// �� ��� � �����
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
		if (lift_del && side != 0)		// ��������� ����������� ��������
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
		// ���� �������� ����� �� �����
	if (father_ptr == nullptr)		
	{
		search_node_with_key(cur_node, key, pos);	// ������� �������� ���� ������� ��������

			// � ����� ���� �������
		if (cur_node->data.size() == 1)     
		{
			Node* buf = root;
			if (side == 1)
			{
				cur_node->ptr_sons[1]->~Node();
				root = cur_node->ptr_sons[0];				// ������ �������� ������� �������� � ���� ���� �������� ����� ����� �� �����
			}
			else
			{
				cur_node->ptr_sons[0]->~Node();
				root = cur_node->ptr_sons[1];					// ���� �������� ������� �������� � ���� ������ �������� ����� ����� �� �����
			}
			buf->~Node();
		}
			// � ����� ������� ��������
		else   
		{
			int ptrdel_r_l;	// ����� �� ������� �� ���� �� ������ ������ ���������� (��� ������ ����� �� ������� �� � �������� ��� ������ ��� ������ � �����)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i > 0; i--)							// ������ �������� ������ �������� ��� ���� ���� ��������
			{
				cur_node->data[i] = cur_node->data[i - 1];
			}
			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i > 0; i--)				// ������ ��������� ������ �������� ��� ���� ���� �������� (side)
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
			}
			auto it_begin_data = cur_node->data.begin();
			auto it_begin_ptr = cur_node->ptr_sons.begin();

			cur_node->data.erase(it_begin_data);			// �������� ���������� ������ �������
			cur_node->ptr_sons.erase(it_begin_ptr);			// �������� �� �������� ������ ��������
		}
	}
		// ���� �������� ����� �� �� �����
	else      
	{
		search_node_with_key(cur_node, key, pos);	// ������� �������� ���� ������� ��������

			// � ����� ���� �� �� ��������
		if (cur_node->data.size() > min_keys)		
		{
			int ptrdel_r_l;	// ����� �� ������� �� ���� �� ������ ������ ���������� (��� ������ ����� �� ������� �� � �������� ��� ������ ��� ������ � �����)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i > 0; i--)					// ������ ������ �������� ���� ��������� ������� ���� ���� ��������
			{
				cur_node->data[i] = cur_node->data[i - 1];
			}

			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i > 0; i--)				// ������ ������ ��������� ���� ��������� �������� ���� ���� ��������
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
			}

			// �������� ������ �������� � �������� (������� �����)
			auto it_begin_data = cur_node->data.begin();
			cur_node->data.erase(it_begin_data);
			auto it_begin_ptr = cur_node->ptr_sons.begin();
			cur_node->ptr_sons.erase(it_begin_ptr);

			lift_del = false;	// ����� �� ������� ����� ��������
			side = 0;
		}
			// ���� ���� �� ��, ������ ��
		else if (pos_deep > 0 && father_ptr->ptr_sons[pos_deep - 1]->data.size() > min_keys)		
		{
			swap_element = father_ptr->data[pos_deep - 1];  // ������� ���� �� ������� � ������ �� ����� ����������� � �����

			int ptrdel_r_l;	// ����� �� ������� �� ���� �� ������ ������ ���������� (��� ������ ����� �� ������� �� � �������� ��� ������ ��� ������ � �����)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i > 0; i--)					// ������ �������� ������ �������� ����������
			{
				cur_node->data[i] = cur_node->data[i - 1];
			}
			cur_node->data[0] = swap_element;				// ������ �� ���� ������� �������� ����� (� ����� ��������) ������� � ������ �� ��� ������ � ���� ������

			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i > 0; i--)			// ������ ��������� ������ ��������� � ������� �� ����� ������ (side)
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
			}
			int in_last_p_left = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.size() - 1;	// ������ ���������� ������� ����� �����
			cur_node->ptr_sons[0] = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons[in_last_p_left];	// �� ���� ������� ������� � ���� ������� ������� � ����� �����

			int in_last_d_left = father_ptr->ptr_sons[pos_deep - 1]->data.size() - 1;		// ������ ���������� �������� ����� �����

			father_ptr->data[pos_deep - 1] = father_ptr->ptr_sons[pos_deep - 1]->data[in_last_d_left]; // �� ���� �������� � ������������ ���� �� ������ � ���� ������ ������ ������� ������� ����� �����

			father_ptr->ptr_sons[pos_deep - 1]->data.pop_back();			// �������� ������� ������� � ������ ����� �����
			father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.pop_back();

			lift_del = false;	// ����� �� ������� ����� ��������
			side = 0;
		}
			// ������ ���� �� ��, ������ ��
		else if (pos_deep < father_ptr->ptr_sons.size() - 1 && father_ptr->ptr_sons[pos_deep + 1]->data.size() > min_keys)  
		{
			swap_element = father_ptr->data[pos_deep];  // ������� ���� �� ������� � ������ �� ����� ����������� �������� � �����

			int ptrdel_r_l;	// ����� �� ������� �� ���� �� ������ ������ ���������� (��� ������ ����� �� ������� �� � �������� ��� ������ ��� ������ � �����)
			if (side == 1)
				ptrdel_r_l = 1;
			else if (side == -1)
				ptrdel_r_l = 0;

			for (int i = pos; i < cur_node->data.size() - 1; i++)					// ������ �������� ���� �������� ����������
			{
				cur_node->data[i] = cur_node->data[i + 1];
			}
			cur_node->data[cur_node->data.size() - 1] = swap_element;				// ������ �� ���� ����������� �������� ����� (� ����� ��������) ������� � ������ �� ��� ������ � ������ ������

			cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
			for (int i = pos + ptrdel_r_l; i < cur_node->ptr_sons.size() - 1; i++)			// ������ ��������� ���� ��������� � ������� �� ����� ������ (side)
			{
				cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];
			}
			cur_node->ptr_sons[cur_node->ptr_sons.size() - 1] = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons[0];	// �� ���� ������� ������� � ���� ������� ������� � ����� �����

			father_ptr->data[pos_deep] = father_ptr->ptr_sons[pos_deep + 1]->data[0]; // �� ���� �������� � ������������ ���� �� ������ � ������ ������ ������ ������ ������� ������� �����


			auto it_begin_data_right = father_ptr->ptr_sons[pos_deep + 1]->data.begin();			// �������� ������ ������� � ������ ������� �����
			auto it_begin_ptr_right = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
			father_ptr->ptr_sons[pos_deep + 1]->data.erase(it_begin_data_right);
			father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.erase(it_begin_ptr_right);

			lift_del = false;	// ����� �� ������� ����� ��������
			side = 0;
		}
			// ���� � ������ ���� �� ������ ��
		else        
		{
			if (pos_deep > 0)	//������ � ���� ������ (����� ������)
			{
				int ptrdel_r_l;	// ����� �� ������� �� ���� �� ������ ������ ���������� (��� ������ ����� �� ������� �� � �������� ��� ������ ��� ������ � �����)
				if (side == 1)
					ptrdel_r_l = 1;
				else if (side == -1)
					ptrdel_r_l = 0;

				swap_element = father_ptr->data[pos_deep - 1];			// ����� �������(�� ������ � ���� ������) � ������ �� ���������� � �����
				for (int i = pos; i > 0; i--)							// ������ �������� ������ �������� ��������� 
				{
					cur_node->data[i] = cur_node->data[i - 1];
				}
				cur_node->data[0] = swap_element;	// �� ������ ������� ������� �������� ������� � ������

				cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
				for (int i = pos + ptrdel_r_l; i < 0; i--)			// ������ ��������� ������ �������� ���������  (side ���� �� ������)
				{
					cur_node->ptr_sons[i] = cur_node->ptr_sons[i - 1];
				}
				auto it_begin_ptr = cur_node->ptr_sons.begin();
				cur_node->ptr_sons.erase(it_begin_ptr);					// ������ �� �������� ������ ��������

				auto it_begin_data_node = cur_node->data.begin();
				auto it_begin_data_left = father_ptr->ptr_sons[pos_deep - 1]->data.begin();
				auto it_end_data_left = father_ptr->ptr_sons[pos_deep - 1]->data.end();
				cur_node->data.insert(it_begin_data_node, it_begin_data_left, it_end_data_left); // ������ �������� ����� ����� �� �����

				it_begin_ptr = cur_node->ptr_sons.begin();
				auto it_begin_ptr_left = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.begin();
				auto it_end_ptr_left = father_ptr->ptr_sons[pos_deep - 1]->ptr_sons.end();
				cur_node->ptr_sons.insert(it_begin_ptr, it_begin_ptr_left, it_end_ptr_left);		// ������ ��������� ����� ����� �� �����

				lift_del = true;				// ����������� ����� � ��������� ������������
				side = -1;						// ���� �������� ����� ������ ������� ����������� ����� ��� ��� ��������
				key = swap_element.first;		// ���� �� ���������� � ������ � ����� ������� �������� � ������
			}
				//������ � ������ ������ (������� ������)
			else		
			{
				int ptrdel_r_l;	// ����� �� ������� �� ���� �� ������ ������ ���������� (��� ������ ����� �� ������� �� � �������� ��� ������ ��� ������ � �����)
				if (side == 1)
					ptrdel_r_l = 1;
				else if (side == -1)
					ptrdel_r_l = 0;

				swap_element = father_ptr->data[pos_deep];			// ����� �������(�� ������ � ������ ������) � ������ �� ���������� � �����
				for (int i = pos; i > cur_node->data.size() - 1; i++)							// ������ �������� ���� �������� ��������� 
				{
					cur_node->data[i] = cur_node->data[i + 1];
				}
				cur_node->data[cur_node->data.size() - 1] = swap_element;	// �� ������� ������� ������� �������� ������� � ������

				cur_node->ptr_sons[pos + ptrdel_r_l]->~Node();
				for (int i = pos + ptrdel_r_l; i > cur_node->ptr_sons.size() - 1; i++)			// ������ ��������� ���� �������� ���������  (side ���� �� ������)
				{
					cur_node->ptr_sons[i] = cur_node->ptr_sons[i + 1];
				}
				//auto it_end_ptr = cur_node->ptr_sons.begin() + cur_node->ptr_sons.size() - 1;
				cur_node->ptr_sons.pop_back();				// ������ �� �������� ������� ��������

				auto it_end_data_node = cur_node->data.end();
				auto it_begin_data_left = father_ptr->ptr_sons[pos_deep + 1]->data.begin();
				auto it_end_data_left = father_ptr->ptr_sons[pos_deep + 1]->data.end();
				cur_node->data.insert(it_end_data_node, it_begin_data_left, it_end_data_left);		// ������ �������� ������� ����� �� �����

				auto it_end_ptr = cur_node->ptr_sons.end();
				auto it_begin_ptr_left = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.begin();
				auto it_end_ptr_left = father_ptr->ptr_sons[pos_deep + 1]->ptr_sons.end();
				cur_node->ptr_sons.insert(it_end_ptr, it_begin_ptr_left, it_end_ptr_left);        // ������ ��������� ������� ����� �� �����

				lift_del = true;				// ����������� ����� � ��������� ������������
				side = 1;						// ������ �������� ����� ������ ������� ����������� ����� ��� ��� ��������
				key = swap_element.first;		// ���� �� ���������� � ������ � ����� ������� �������� � ������
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
	cur_node->ptr_sons[pos]->data.insert(it_end_left_son, it_begin_right_son, it_end_right_son);   // ������ �� ����� ���� ������� ����

	auto it_end_left_son_ptr = cur_node->ptr_sons[pos]->ptr_sons.end();
	auto it_begin_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.begin();
	auto it_end_right_son_ptr = cur_node->ptr_sons[pos + 1]->ptr_sons.end();
	cur_node->ptr_sons[pos]->ptr_sons.insert(it_end_left_son_ptr, it_begin_right_son_ptr, it_end_right_son_ptr);	// ������ �� ��������� ����� ���� ��������� ������� ����

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




