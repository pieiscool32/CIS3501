//  Created by Christian Munte on 2/6/17

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

int IMG_SIZE = 32;

struct QuadNode {
    char type;
    int level;
    QuadNode * quads[4];
    QuadNode(){
        quads[0]=nullptr;
        quads[1]=nullptr;
        quads[2]=nullptr;
        quads[3]=nullptr;
    }
};

class QuadTree {
private:
    QuadNode * root;
    
public:
    QuadTree();
    char queryTree(int, int);
    char digTree(QuadNode *, int, int, int);
    bool sameTree(QuadNode *, QuadNode *);
    int numBlack();
    int largestRepeat();
    void largestDig(QuadNode *, vector<QuadNode*> &);
    void frmStr(string);
    void dig(QuadNode *&, int, int , stringstream &);
    void printTree();
    void printElement(QuadNode *);
    void digPrint(QuadNode *);
    void digBlack(QuadNode *, int &);
    void printImage();
};

/*
 Description:
 PRE:
 POST:
 */

int main() { //32iibbwwiwwbwwb
    int size;
    cin >> size;
    IMG_SIZE = size;
    string input;
    cin >> input;
    QuadTree tree;
    tree.frmStr(input);
    //tree.printTree();
    tree.printImage();
    //cout << tree.queryTree(10, 19) << endl;
    cout << tree.numBlack() << " " << tree.largestRepeat() << endl;
    return 0;
}

/*
Description: Constructor
PRE: None
POST: root = new QuadNpde
*/

QuadTree::QuadTree() {
    root =  new QuadNode;
}

/*
 Description: creates a tree from input string
 PRE: input != null
 POST: root points to a tree
 */

void QuadTree::frmStr(string input) {
    if(input.length() == 1) {
        root = new QuadNode;
        root->level = 0;
        root->type = input[0];
    } else {
        stringstream stream(input);
        dig(root, 0, 0, stream);
    }
}

/*
 Description: recursively builds a tree
 PRE: root != nullptr, level >= 0, 0 <= quad <= 4, stream != null
 POST: resursive loop or new node is made
 */

void QuadTree::dig(QuadNode *&root, int level, int quad, stringstream &stream) {
    char next;
    bool deeper = false;
    while (stream >> next) {
        //cout << "level: " << level << " quad: " << quad << " type: " << next << endl;
        if(root->level < level) {
            deeper = true;
        }
        root->quads[quad] = new QuadNode;
        if(next=='i'){
            if(deeper) {
                //cout << "going deeper\n";
                root->quads[quad]->type = next;
                root->quads[quad]->level = level;
                dig(root->quads[quad], level+1, 0, stream);
            } else {
                root->type = next;
                root->level = level;
                dig(root, level+1, 0, stream);
            }
            //cout << "set level: " << root->level << " set type: " << root->type << endl;
            quad++;
        } else {
            root->quads[quad]->type = next;
            root->quads[quad]->level = level;
            quad++;
        }
        if(quad == 4){
            return;
        }
    }
}

/*
 Description: Checks to see what char is at row, col
 PRE: 0 < row < IMG_SIZE, 0 < col < IMG_SIZE
 POST: returns 'w' or 'b'
 */

char QuadTree::queryTree(int row, int col) {
    if(root->type != 'i'){
        return root->type;
    } else {
        return digTree(root, row, col, IMG_SIZE);
    }
}

/*
 Description: Recursively looks for char
 PRE: root != nullptr, 0 < row < IMG_SIZE, 0 < col < IMG_SIZE, 0 < img < IMG_SIZE
 POST: recursive loop or returns 'w' or 'b'
 */

char QuadTree::digTree(QuadNode *root, int row, int col, int img) {
    if(root->type == 'b' || root->type == 'w'){
        return root->type;
    } else if (row <= img/2 && col <= img/2) {
        return digTree(root->quads[0], row, col, img/2);
    } else if (row <= img/2 && col > img/2) {
        return digTree(root->quads[1], row, col-(img/2), img/2);
    } else if (row > img/2 && col > img/2) {
        return digTree(root->quads[2], row-(img/2), col-(img/2), img/2);
    } else if (row > img/2 && col <= img/2) {
        return digTree(root->quads[3], row-(img/2), col, img/2);
    } else {
        return '?';
    }
}

/*
 Description: Prints the image in the tree
 PRE: root points to a vaild tree
 POST: image is printed
 */

void QuadTree::printImage() {
    for (int row=1; row <= IMG_SIZE; row++) {
        for (int col=1; col <= IMG_SIZE; col++) {
            if (queryTree(row, col) == 'b') {
                cout << '@';
            } else {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

/*
 Description: prints the tree
 PRE: root pooints to a vlaid tree
 POST: tree is printed
 */

void QuadTree::printTree() {
    printElement(root);
    digPrint(root);
}

/*
 Description: prints each tree node
 PRE: node != nullptr
 POST: node is printed
 */

void QuadTree::printElement(QuadNode *node) {
    for (int num=0; num < node->level*5; num++) {
        cout << " ";
    }
    cout << node->type << " " << to_string(node->level) << endl;
}

/*
 Description: recursively finds the tree elements
 PRE: root != nullptr
 POST: resursive loop or prints element
 */

void QuadTree::digPrint(QuadNode * root) {
    for(int quad = 3; quad >= 0; quad--) {
        if(root->quads[quad] != nullptr) {
            printElement(root->quads[quad]);
            digPrint(root->quads[quad]);
        }
    }
}

/*
 Description: Returns the number of black pixels
 PRE: root != nullptr
 POST: retruns number of black pixels
 */

int QuadTree::numBlack() {
    int count = 0;
    if(root->type != 'i') {
        return pow(IMG_SIZE,2);
    }
    digBlack(root, count);
    return count;
}

/*
 Description: Recursively finds all the black elements
 PRE: root != nullptr, count >= 0
 POST: resursive loop or adds number of black pixels at node to count
 */

void  QuadTree::digBlack(QuadNode * root, int &count) {
    for(int quad = 0; quad < 4; quad++) {
        if (root->quads[quad] != nullptr) {
            digBlack(root->quads[quad], count);
        }
    }
    if (root->type == 'b') {
        count += pow((IMG_SIZE/pow(2,root->level)),2);
    }
}

/*
 Description: Cheks to see if two trees are identical
 PRE: root1 and roo2 != nullptr
 POST: returns true or false
 */

bool QuadTree::sameTree(QuadNode *root1, QuadNode *root2) {
    if(root1->type == root2->type) {
        for (int quad=0; quad < 4; quad++) {
            if (root1->quads[quad] != nullptr && root2->quads[quad] != nullptr) {
                if(sameTree(root1->quads[quad], root2->quads[quad]) == false){
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

/*
 Description: returns the largest repeating tree
 PRE: root points to a valid tree
 POST: returns largest tree
 */

int QuadTree::largestRepeat(){
    vector<QuadNode*> nodes;
    int ret = 0;
    if(root->type != 'i') {
        return ret;
    }
    largestDig(root, nodes);
    for(int one=0; one < nodes.size(); one++){
        for (int two=0; two < nodes.size(); two++) {
            if(sameTree(nodes[one], nodes[two]) && one != two && nodes[one]->quads[0] != nullptr && nodes[two]->quads[0] != nullptr){
                if(nodes[one]->quads[0]->level < nodes[two]->quads[0]->level){
                    ret = IMG_SIZE/pow(2,nodes[two]->quads[0]->level);
                } else if (ret == 0) {
                    ret = IMG_SIZE/pow(2,nodes[one]->quads[0]->level);
                }
            }
        }
    }
    return ret;
}

/*
 Description: recursively finds all tree combos
 PRE: root1 != nullptr, nodes is initiated
 POST: nodes contains all elements

 */

void QuadTree::largestDig(QuadNode * root, vector<QuadNode*> &nodes) {
    if(root != nullptr) {
        for(int quad=0; quad < 4; quad++) {
            largestDig(root->quads[quad], nodes);
            nodes.push_back(root);
        }
    }
}
