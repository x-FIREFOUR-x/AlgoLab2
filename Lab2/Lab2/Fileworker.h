#pragma once
#include <fstream>
#include <string>
#include <queue>
#include "Node.h"
using namespace std;
class Fileworker
{
public:
	
	Node* read_BD(string filename, int& t, int& min_keys, int& max_keys);			// ����� ������ � �����
	int parsing_key(string& line, int& cursor);				// ������ � ����� ���� ���� � ������� ��� �����
	string parsing_value(string& line, int& cursor);		// ������ � ����� ���� �������� � ������� ��� �����

	void write_BD(string filename, Node* root, int t);	   // ������ � ���� ������
};

