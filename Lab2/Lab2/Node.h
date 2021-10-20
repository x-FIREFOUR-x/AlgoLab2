#pragma once
#include <string>
#include <fstream>
#include <vector>

using namespace std;
struct Node
{
	vector<Node*> ptr_sons;				// список вказівників в вузлі
	vector<pair<int,string>> data;		// список елементів(ключ, значення) в вузлі
};

