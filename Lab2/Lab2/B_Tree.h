#pragma once
#include "Node.h"
#include "Fileworker.h"
#include<fstream>
#include <iostream>
#include <queue>
using namespace std;



class B_Tree
{
	private:
		Node* root;				// �������� �� ����� B-tree
		string filename;		// ���� �� ������ ������
		int t;					// ��������  B-tree
		int min_keys;			// �������� ������� ������ � �����
		int max_keys;			// ����������� ������� ������ � �����
		int size;				// ������� ������ � �����

	public:
		B_Tree(string file);			// ����������� ������ 
		Node* get_root();
		int get_size();

		string search(int key);					// ������� ������ ����� � �����
		void push(int key, string value,bool& element_add_success);		// ������� ������� ����� �������� � ������
		bool pop(int key);						// ������� ��������� �������� �� ����� � ������

	private:
										// ������� ������� ��� (search)
		string binary_search(Node*& curent_node,const int key);						// ����������� ������ �������� ������ � �����

												// ������� ������� ��� (push)
		void search_node(Node*& curent_node, Node* father_ptr, int& key,string& value, bool& need_break_node, Node*& add_ptr, bool& element_add_success);	// ����������� ������ ������ ���� ���������� ������ �������� 
		int search_pos_insert(Node* curent_node, int key, bool& is_this_key);									// �������� ���� ��� ������� �������� � ����� ��� ���� �����������
		void insert_element(Node*&, int pos, int key, string value, Node*& add_ptr);							// ������� �������� � �����
		void cell_node(Node*& curent_node, Node*& n1, Node*& n2);												// �������� ����� �� ��� ����������

													// ������� ������� ��� (pop)
		void pop(int& key, bool& is_key, Node* cur_node, Node* father_ptr, pair<int, string> swap_element, int pos_deep, bool& lift_del, int& side, bool& descent);				// ����������� ������ ��������� �������� �� �����
		bool search_node_with_key(Node* cur_node, int key, int& pos);																											// ����� �������� � ���� ��� ������ ��������� �� ����������� ��� ���������� ������ (������� true ���� ������� � ���� ������ false) (pos ����� �������� �������� ��� ����� ��������� �� �������� �����������)
		void lift_deletion(int& key, bool& is_key, Node* cur_node, Node* father_ptr, pair<int, string> swap_element, int pos_deep, bool& lift_del, int& side, bool& descent);   // ���������� ��������� �� ������ �������� �� ���� ���� 
		void delete_element(Node*& node_key,int key, int pos);																													// ��������� �������� � �� �������� �����
		Node* search_elem_left_subroot(Node* cur_node,int pos_next_deep, pair<int, string>& element);		// ����� �������� � ����� ������� ��� ����� � �������
		Node* search_elem_right_subroot(Node* cur_node, int pos_next_deep, pair<int, string>& element);		// ����� �������� � ������� ������� ��� ����� � �������
		void to_left_son_add_right_son(Node* cur_node, int pos);											// �� ����� ���� ������ �������
		Node* search_root_element(Node* cur_node, int key, int& pos);										// ����� �������� � ������ ����� ���� ������� 
};

