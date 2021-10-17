#pragma once
#include <string>
#include <fstream>
#include <vector>

using namespace std;
struct Node
{
	vector<Node*> ptr_sons;
	vector<pair<int,string>> data;

	
};

