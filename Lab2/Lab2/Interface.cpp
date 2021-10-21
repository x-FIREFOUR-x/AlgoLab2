#include "Interface.h"

void Interface::console_interface()
{

    B_Tree tree("BD.txt");
    output_tree_in_console(tree);
    bool is_exit = false;
    while (!is_exit)
    {
        int a;
        cout << endl << "input (1-search, 2-add, 3-pop, 4-exit)- ";
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
            is_exit = true; break;
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
