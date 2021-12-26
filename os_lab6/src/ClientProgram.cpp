#include <bits/stdc++.h>
#include "CalculationNode.h"
#include "ZMQFunctions.h"
#include "BalancedTree.h"

using namespace std;

/*

    ВАРИАНТ 35
    4 1 2
    все вычислительные узлы хранятся в идеально сбалансированном бинарном дереве,
    каждый следующий узел должен добавляться в самое наименьшее поддерево.

    тип команды -- подсчет суммы n чисел
    exec id n k1 k2 ... kn
    id -- целочисленный идентификатор вычислительного узла, на который отправляется команда
    n -- количество складываемых чисел
    k1 ... kn -- складываемые числа

    формат команды ping id
    команда проверяет доступность конкретного узла, если узла нет, то необходимо
    выводить ошибку.

*/

int main() {

    string command, ans;
    BalancedTree tree;
    CalculationNode node(-1, -1, -1);

    cout << "Enter the command: \n\n";
    cout << "create id: for creating a new calculation node\n";
    cout << "exec id n n1 n2... n: for calculating a sum\n";
    cout << "pind id: for checking node-availabilty\n";
    cout << "kill id: for killing a calculation node\n\n";

    while ((cout << "Enter command: ") && (cin >> command)) {

        if (command == "create") {
            int child;
            cin >> child;
            if (tree.Exist(child)) {
                cout << "Error: Already exists" << std::endl;
            } else {
                while (true) {
                    int idParent = tree.FindID();
                    if (idParent == node.id) {
                        ans = node.create(child);
                        tree.AddInTree(child, idParent);
                        break;
                    } else {
                        string message = "create " + to_string(child);
                        ans = node.sendstring(message, idParent);
                        if (ans == "Error: Parent not found") {
                            tree.AvailabilityCheck(idParent);
                        } else {
                            tree.AddInTree(child, idParent);
                            break;
                        }
                    }
                }
                cout << ans << endl;
            }
        } else if (command == "exec") {
            string str;
            int child;
            cin >> child;
            getline(cin, str);
            if (!tree.Exist(child)) {
                cout << "Error: Parent is not existed" << std::endl;
            } else {
                string message = "exec " + str;
                ans = node.sendstring(message, child);
                cout << ans << endl;
            }
        } else if (command == "ping") {
            int child;
            cin >> child;
            if (!tree.Exist(child)) {
                cout << "Error: Parent is not existed" << endl;
            } else if (node.left_id == child || node.right_id == child) {
                ans = node.ping(child);
                cout << ans << endl;
            } else {
                string message = "ping " + to_string(child);
                ans = node.sendstring(message, child);
                if (ans == "Error: Parent not found") {
                    ans = "Ok: 0";
                }
                cout << ans << endl;
            }
        } else if (command == "kill") {
            int child;
            std::cin >> child;
            std::string message = "kill";
            if (!tree.Exist(child)) {
                std::cout << "Error: Parent is not existed" << std::endl;
            } else {
                ans = node.sendstring(message, child);
                if (ans != "Error: Parent not found") {
                    tree.RemoveFromRoot(child);
                    if (child == node.left_id) {
                        unbind(node.left, node.left_port);
                        node.left_id = -2;
                        ans = "Ok";
                    } else if (child == node.right_id) {
                        node.right_id = -2;
                        unbind(node.right, node.right_port);
                        ans = "Ok";
                    } else {
                        message = "clear " + std::to_string(child);
                        ans = node.sendstring(message, std::stoi(ans));
                    }
                    std::cout << ans << std::endl;
                }
            }
        } else {
            std::cout << "Please enter correct command!" << std::endl;
        }

    }

    node.kill();
}