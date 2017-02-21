//  Created by Christian Munte on 2/6/17


#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

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
    void largestDig(QuadNode *, QuadNode *, int &);
    void frmStr(string);
    void dig(QuadNode *&, int, int , stringstream &);
    void printTree();
    void printElement(QuadNode *);
    void digPrint(QuadNode *);
    void digBlack(QuadNode *root, int &);
    void printImage();
    
};

string cleanInput();

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

string cleanInput() {
    string output;
    getline(cin, output, '#');
    output.erase(remove(output.begin(), output.end(), ' '), output.end());
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    cout << output << endl;
    return output;
}

QuadTree::QuadTree() {
    root =  new QuadNode;
}

void QuadTree::frmStr(string input) {
    stringstream stream(input);
    dig(root, 0, 0, stream);
    
    //cout << "done" << endl;
}

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

bool QuadTree::sameTree(QuadNode *root1, QuadNode *root2) {
    for (int quad=0; quad < 4; quad++) {
        if (root1->quads[quad] != nullptr && root2->quads[quad] != nullptr) {
            return sameTree(root1->quads[quad], root2->quads[quad]);
        }
    }
    if (root1->type != root2->type) {
        return false;
    } else {
        return true;
    }
}

char QuadTree::queryTree(int row, int col) {
    return digTree(root, row, col, IMG_SIZE);
}

void QuadTree::printImage() {
    //cout << queryTree(9, 18) << endl;
    //cout << " ";
    //for (int a=1; a<IMG_SIZE+1; a++) {
        //cout << a%10;
    //}
    //cout << endl;
    for (int row=1; row < IMG_SIZE+1; row++) {
        //cout << row%10;
        for (int col=0; col < IMG_SIZE; col++) {
            if (queryTree(row, col) == 'b') {
                cout << '@';
            } else {
                cout << ' ';
            }
        }
        //cout << row%10 << endl;
        cout << endl;
    }
    //cout << " ";
    //for (int a=1; a<IMG_SIZE+1; a++) {
        //cout << a%10;
    //}
    //cout << endl;
}

char QuadTree::digTree(QuadNode *root, int row, int col, int img) {
    if(root->type == 'b' || root->type == 'w'){
        return root->type;
    } else if (row <= img/2 && col < img/2) {
        return digTree(root->quads[0], row, col, img/2);
    } else if (row <= img/2 && col >= img/2) {
        return digTree(root->quads[1], row, col-(img/2), img/2);
    } else if (row > img/2 && col >= img/2) {
        return digTree(root->quads[2], row-(img/2), col-(img/2), img/2);
    } else if (row > img/2 && col < img/2) {
        return digTree(root->quads[3], row-(img/2), col, img/2);
    } else {
        return '?';
    }
}

void QuadTree::printTree() {
    printElement(root);
    digPrint(root);
}

void QuadTree::digPrint(QuadNode * root) {
    for(int quad = 3; quad >= 0; quad--) {
        if(root->quads[quad] != nullptr) {
            printElement(root->quads[quad]);
            digPrint(root->quads[quad]);
        }
    }
}

void QuadTree::printElement(QuadNode *node) {
    for (int num=0; num < node->level*5; num++) {
        cout << " ";
    }
    cout << node->type << " " << to_string(node->level) << endl;
}

int QuadTree::numBlack() {
    int count = 0;
    digBlack(root, count);
    return count;
}


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

int QuadTree::largestRepeat(){
    int biggest = 0;
    largestDig(root, root, biggest);
    return (IMG_SIZE/pow(2,biggest));
}

void QuadTree::largestDig(QuadNode * root1, QuadNode * root2, int &level) {
    for(int one=0; one < 4; one++){
        for (int two=0; two < 4; two++) {
            if(root1->quads[one] != nullptr && root2->quads[two] != nullptr){
                if(sameTree(root1->quads[one], root2->quads[two]) && one != two){
                    level = root1->quads[one]->level;
                } else if (one != two) {
                    largestDig(root1->quads[one], root2->quads[two], level);
                }
            }
        }
    }
}
