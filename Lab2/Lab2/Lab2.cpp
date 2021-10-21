#include <ctime>
#include "Interface.h"

using namespace std;
int main()
{
	srand(time(NULL));
	
    Interface interface;
	interface.console_interface();
	

    return 0;
}
