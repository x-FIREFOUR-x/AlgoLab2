#pragma once
#include <fstream>
#include <string>
#include <queue>
#include "Node.h"
using namespace std;
class Fileworker
{
public:
	
	Node* read_BD(string filename, int& t, int& min_keys, int& max_keys);			// зчитує дерево з файла
	int parsing_key(string& line, int& cursor);				// відділяє з рядка один ключ і скорочує сам рядок
	string parsing_value(string& line, int& cursor);		// відділяє з рядка одне значення і скорочує сам рядок

	void write_BD(string filename, Node* root, int t);	   // записує у файл дерево
};

