#pragma once
#include "Node.h"
#include<fstream>
#include <iostream>
#include <queue>
using namespace std;



class B_Tree
{
	private:
		Node* root;
		string filename;
		int t;
		int min_keys;
		int max_keys;

	public:
		B_Tree(string file, int t1);

		string search(int key);
		void push(int key, string value);

		bool pop(int key);

		void console_write();
		void TLR(Node* p);

	private:
		void read_BD();
		int parsing_key(string& line, int& cursor);
		string parsing_value(string& line, int& cursor);

		void write_BD();
		
		string search_in_node(Node*& curent_node,const int key);

		void search_node(Node*& curent_node, int& key,string& value, bool& need_break_node, Node*& add_ptr);
		int search_pos_insert(Node* curent_node, int key);
		void insert_element(Node*&, int pos, int key, string value, Node*& add_ptr);
		void cell_node(Node*& curent_node, Node*& n1, Node*& n2);

		void pop(int& key, bool& is_key, Node* cur_node, Node* father_ptr, pair<int, string> swap_element, int pos_deep, bool& lift_del, int& side, bool& descent);
		bool search_node_with_key(Node* root, int key, int& pos);
		void delete_element(Node*& node_key,int key, int pos);
		Node* search_swap_root_left(Node* cur_node,int pos, pair<int, string>& element);
		Node* search_swap_root_right(Node* cur_node, int pos, pair<int, string>& element);
		void to_left_son_add_right_son(Node* cur_node, int pos);
};

