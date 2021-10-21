#pragma once
#include <iostream>
#include<string>
#include<algorithm>
#include "B_Tree.h"

using namespace std;
class Interface
{

	public:
		void console_interface();
	private:
		void output_tree_in_console(B_Tree tree);
		void push_more_element(B_Tree& tree, int amount);
};

