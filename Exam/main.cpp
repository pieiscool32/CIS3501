#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

struct Node {
    char data;
    Node * left;
    Node * right;
    Node(char d = ' ') {
        data = d;
        left = nullptr;
        right = nullptr;
    }
};

void fromStr(Node *&, string);

int main() {
    Node * cool = new Node();
    fromStr(cool, "+13");
    return 0;
}

void fromStr(Node *& root, string input) {
    root->data = input[0];
    input.erase(input.front());
    if(input[0] >= '0' && input[0] <= '9'){
        root->left = new Node(input[0]);
        input.erase(input.front());
    }
    if(input.front() == input.back()){
        root->right = new Node(input[0]);
        return;
    } else {
        root->right = new Node();
        fromStr(root->right, input);
        return;
    }
}
