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
		B_Tree( string file, int t1);
		string search(int key);
		void console_write();
		void TLR(Node* p);

	private:
		void read_BD();
		int parsing_key(string& line, int& cursor);
		string parsing_value(string& line, int& cursor);

		
};

