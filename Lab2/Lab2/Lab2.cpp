#include <iostream>
#include <fstream>
#include<string>
#include "B_Tree.h"

using namespace std;
int main()
{
    B_Tree tree("BD.txt",3);
    //cout << "555" << endl;
    tree.console_write();

    int key;
    cin >> key;
    string val = tree.search(key);

    if (val == "")
    {
        cout << "non key" << endl;
    }
    else
    {
        cout << val << endl;
    }

    return 0;
}
