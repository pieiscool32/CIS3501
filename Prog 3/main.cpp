/*
 Christian Munte 3-16-17
 Looks like Flint is back in the news:
 People at flint want to try ad optimize their
 water system; for some reason. So they decided
 to hire college students to try and code a way.
 They told the students to get two numbers to start,
 the number of junctions and edges. They then are
 given the connecting points and weights for each
 edge and told to store it somehow. After they store
 all the edges, they have to start from the beginning
 and keep storing these sets until Flint people
 stop making a fuss. After that, the students go
 ahead and somehow figure out how which edges are
 most closely related without making a cycle and
 output the value of how  close they are. They do
 this for each set of input to make the Edison
 company a Flint happy. After the college students
 finish this assignment, they will be expected to
 hand it over to the Company free of charge to be
 used for the rest of eternity. In your comment,
 find the obvious error in this block. Any students
 that do not comply will fail.
 */

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

struct edge {
    int parent, weight, count;
};

struct input {
    int node1, node2, weight;
};

void doPrint(const vector<edge> input) {
    for (int pos = 0; pos < input.size(); pos++) {
        cout << pos+1 << "   ";
    }
    cout << endl;
    for (int pos = 0; pos < input.size(); pos++) {
        cout << input[pos].parent << "  ";
    }
}

class set {
  private:
    vector<edge> connections;
  public:
    set(int edges){
        edge temp;
        
        for(int pos = 0; pos < edges; pos++) {
            temp.weight = 0;
            temp.parent = -1;
            temp.count = -1;
            connections.push_back(temp);
        }
    }
    
    int doFind(const vector<edge> input, int find) {
        //preforms set find
        if(input[find-1].parent != find) {
            return doFind(input, find);
        }
        return find;
    }
    
    void doUnion(vector<edge> &input, int x, int y) {
        //preforms set union
        int a = doFind(input, x);
        int b = doFind(input, y);
        input[a].parent = b;
    }
    
    void addConnection(vector<edge> &input) {
        int x, y, weight;
        cin >> x >> y >> weight;
        input[y-1].weight = weight;
        doUnion(input, x, y);
    }
};

class tree {
  private:
    
    vector<input> inputs;
  public:
    tree(int edges) {
        input tempI;
        
        for(int pos = 0; pos < edges; pos++) {
            tempI.node1 = -1;
            tempI.node2 = -1;
            tempI.weight = -1;
            inputs.push_back(tempI);
        }
    }
};

int main() { //4 5 1 2 3 1 3 5 1 4 6 2 4 6 3 4 7
    int junct, edges;
    vector<tree> trees;
    cin >> junct >> edges;
    cout << junct << " junctions and " << edges << " edges\n";
    tree temp(edges);
    
    return 0;
}
