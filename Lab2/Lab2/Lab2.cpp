#include <iostream>
#include <fstream>
#include<string>
#include <vector>
#include "B_Tree.h"

using namespace std;
int main()
{
  
    B_Tree tree("BD.txt",3);
    //tree.console_write();
    bool is_exit = false;
    while (!is_exit)
    {
        int a;
        cout << endl << "input (1-search, 2-add, 3-pop, 4-exit)";
        cin >> a;
            switch (a)
            {
                case 1: 
                {
                    int key;
                    cout << endl << "input search key: ";
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
                    break;
                }
                    

                case 2: 
                {
                    int key;
                    cout << endl << "input add key: ";
                    cin >> key;
                    string value;
                    cout << endl << "input add value: ";
                    cin >> value;
                    tree.push(key, value);
                    break;
                }


                case 3: 
                {
                    int key;
                    cout << endl << "input delete key: ";
                    cin >> key;
                    bool succses = tree.pop(key);
                    if (succses)
                        cout << endl << " succses delete key- " << key;
                    else
                        cout << endl << "not succses delete key(not have this key)";
                    //tree.console_write();
                    break;
                }

                case 4: 
                    is_exit = true; break;
            }
    }
    
    return 0;
}
