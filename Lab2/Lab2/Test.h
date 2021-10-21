#pragma once
class Test
{
	int amount_comparison;
public:
	Test();
	Test operator++(int);
	int get_amount_comparison();
};

