#include "Interface.h"

void Interface::console_interface()
{

    B_Tree tree("BD.txt");
    //output_tree_in_console(tree);
    cout << endl << "elements: " << tree.get_size();
    bool is_exit = false;
    while (!is_exit)
    {
        int a;
        cout << endl << "input (1-search, 2-add, 3-pop, 4-exit, 5-push random 10k)- ";
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
                bool element_add_success = true;
                tree.push(key, value, element_add_success);
                if (element_add_success)
                {
                    cout << endl << "element with key-" << key << " success added";
                }
                else
                {
                    cout << endl << "this key already is in tree (not added)";
                }
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
            
                break;
            }

            case 4:
            {
                is_exit = true;
                break;
            }
            

            case 5:
            {
                int amount;
                cout << endl << "Input amount random element for push: ";
                cin >> amount;
                push_more_element(tree, amount);
                cout << endl << "amount elements: " << tree.get_size();
                break;
            }
        }

    }

}
void Interface::output_tree_in_console(B_Tree tree)
{
    

    queue<Node*> que;
    que.push(tree.get_root());

    while (!que.empty() && que.front()->data.size() != 0)
    {
        Node* node = que.front();
        que.pop();
        for (int i = 0; i < node->data.size(); i++)
        {
            cout << node->data[i].first << ":";
            cout << node->data[i].second << " ";
        }
        for (int i = 0; i < node->ptr_sons.size(); i++)
        {
            que.push(node->ptr_sons[i]);
        }
        cout << endl;
    }
}

void Interface::push_more_element(B_Tree& tree, int amount)
{
    vector<int> keys;
    for (int i = 0; i <= amount; i++)
    {
        keys.push_back(i);
    }

    random_shuffle(keys.begin(), keys.end());


    string value = "";
    int code_ascii;
    char symbol;
    for (int i = 0; i < keys.size(); i++)
    {
        value = "";
        for (int i = 0; i < 3; i++)
        {
            code_ascii = rand() % 58 + 65;
            symbol = code_ascii;
            value += symbol;
        }

        bool element_add_success = true;
        tree.push(keys[i], value, element_add_success);
    }
   
}