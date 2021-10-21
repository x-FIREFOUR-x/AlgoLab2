#include "Test.h"

Test::Test()
{
	amount_comparison = 0;
}

Test Test::operator++(int)
{
	amount_comparison++;

	return *this;
}

int Test::get_amount_comparison()
{
	return amount_comparison;
}
