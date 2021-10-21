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
		Node* root;				// вказівник на корінь B-tree
		string filename;		// файл що зберігає дерево
		int t;					// параметр  B-tree
		int min_keys;			// мінімальна кількість ключів в дереві
		int max_keys;			// максимальна кількість ключів в дереві
		int size;				// кількість ключів в дереві

	public:
		B_Tree(string file);			// конструктор дерева 
		Node* get_root();
		int get_size();

		string search(int key);					// функція пошуку ключа в дереві
		void push(int key, string value,bool& element_add_success);		// функція вставки ключа значення в дерево
		bool pop(int key);						// функція видалення елемента по ключу з дерева

	private:
										// допоміжні функції для (search)
		string binary_search(Node*& curent_node,const int key);						// рекурсивний виклик бінарного пошуку в дереві

												// допоміжні функції для (push)
		void search_node(Node*& curent_node, Node* father_ptr, int& key,string& value, bool& need_break_node, Node*& add_ptr, bool& element_add_success);	// рекурсивний виклик пошуку місця вставлення нового елемента 
		int search_pos_insert(Node* curent_node, int key, bool& is_this_key);									// знаходим місце для вставки елемента в вузол або місце заглиблення
		void insert_element(Node*&, int pos, int key, string value, Node*& add_ptr);							// вставка елемента в вузол
		void cell_node(Node*& curent_node, Node*& n1, Node*& n2);												// розбиття вузла під час добавляння

													// допоміжні функції для (pop)
		void pop(int& key, bool& is_key, Node* cur_node, Node* father_ptr, pair<int, string> swap_element, int pos_deep, bool& lift_del, int& side, bool& descent);				// рекурсивний виклик видалення елемента по ключу
		bool search_node_with_key(Node* cur_node, int key, int& pos);																											// пошук елемента в вузлі або номера вказівника на заглиблення для подальшого пошуку (повертає true якщо елемент в вузлі інакше false) (pos номер шуканого елемента або номер вказівника на подальше заглиблення)
		void lift_deletion(int& key, bool& is_key, Node* cur_node, Node* father_ptr, pair<int, string> swap_element, int pos_deep, bool& lift_del, int& side, bool& descent);   // рукурсивне видалення на підйоми елемента що пішов вниз 
		void delete_element(Node*& node_key,int key, int pos);																													// видалення елемента з не мінімально листа
		Node* search_elem_left_subroot(Node* cur_node,int pos_next_deep, pair<int, string>& element);		// пошук елемента в лівому піддереві для заміни з коренем
		Node* search_elem_right_subroot(Node* cur_node, int pos_next_deep, pair<int, string>& element);		// пошук елемента в правому піддереві для заміни з коренем
		void to_left_son_add_right_son(Node* cur_node, int pos);											// до лівого сина приєднує правого
		Node* search_root_element(Node* cur_node, int key, int& pos);										// пошук елемента з кореня якого нада поміняти 
};

