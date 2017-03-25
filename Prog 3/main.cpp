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
#include <stack>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

struct edge {
    int parent, weight, count;
    
    edge(int par = -1, int wei = 0, int cnt = -1) {
        parent = par;
        weight = par;
        count = cnt;
    }
};

struct input {
    int node1, node2, weight;
    
    bool operator < (const input &given) const {
        return abs(weight) < abs(given.weight);
    }
    
    bool operator > (const input &given) const {
        return abs(weight) > abs(given.weight);
    }
};

class set {
  private:
    vector<edge> connections;
  public:
    set(int junctions){
        for(int pos = 0; pos < junctions; pos++) {
            connections.push_back(edge());
        }
    }
    
    int doFind(const vector<edge> input, int find) {
        //preforms set find
        //cout << "looking for " << find << endl;
        if(input[find].parent >= 0) {
            return doFind(input, input[find].parent);
        }
        return find;
    }
    
    bool doUnion(int nodex, int nodey) {
        //preforms set union true if cycle is made
        int a = doFind(connections, nodex);
        int b = doFind(connections, nodey);
        if(a != b){
            if(a < b) {
                //cout << a << " is now " << b << "'s parent\n";
                connections[b].parent = a;
                connections[a].count += connections[b].count;
            } else {
                //cout << b << " is now " << a << "'s parent\n";
                connections[a].parent = b;
                connections[b].count += connections[a].count;
            }
        } else {
            return true;
        }
        return false;
    }
    
    bool isTree() {
        int count = 0;
        for(int index = 0; index < connections.size(); index ++) {
            if(connections[index].parent != -1) {
                count++;
            }
        }
        if(count == connections.size() - 1) {
            return true;
        } else {
            return false;
        }
    }
    
    void doPrint() {
        for (int pos = 0; pos < connections.size(); pos++) {
            cout << pos+1 << "   ";
        }
        cout << endl;
        for (int pos = 0; pos < connections.size(); pos++) {
            if(connections[pos].parent == -1) {
                cout << connections[pos].count << "  ";
            } else {
                cout << connections[pos].parent + 1 << "   ";
            }
        }
        cout << endl;
    }
};

class tree {
  private:
    vector<input> inputs;
    vector<input> span;
    int junctions;
  public:
    tree(int edges, int junct) {
        int x, y, weight;
        input temp;
        
        junctions = junct;
        for (int num = 0; num < edges; num++) {
            cin >> x >> y >> weight;
            temp.node1 = x;
            temp.node2 = y;
            temp.weight = weight;
            inputs.push_back(temp);
        }
    }
    
    int diff() {
        sort(span.begin(), span.end());
        return span.back().weight - span.front().weight;
    }
    
    int doKruskal(vector<input> tree) {
        set mst(junctions);
        span.erase(span.begin(), span.end());
        
        while(tree.size() > 0 && span.size() < junctions - 1) {
            //uses kruskal Algo
            if(!mst.doUnion(tree.front().node1 - 1, tree.front().node2 - 1)) {
                //if it doesn't make a cycle
                span.push_back(tree.front());
                tree.erase(tree.begin());
            } else {
                tree.erase(tree.begin());
            }
        }
        //mst.doPrint();
        if(mst.isTree()) {
            return diff();
        } else {
            return -1;
        }
    }

    int findTree() {
        int smallest = INT_MAX, result;
        sort(inputs.begin(), inputs.end());
        
        while(inputs.size() >= junctions - 1) {
            result = doKruskal(inputs);
            //cout << result << " | " << smallest << endl;
            if (result < smallest && result != -1) {
                smallest = doKruskal(inputs);
            }
            inputs.erase(inputs.begin());
        }
        if (smallest == INT_MAX) {
            return -1;
        }
        return smallest;
    }

};


int main() { //4 5 1 2 3 1 3 5 1 4 6 2 4 6 3 4 7 0 0
    int junct, edges;
    vector<tree> inpt;
    
    cin >> junct >> edges;
    while (junct != 0) {
        //cout << junct << " junctions and " << edges << " edges\n";
        tree given(edges, junct);
        inpt.push_back(given);
        cin >> junct >> edges;
    }
    for(int index = 0; index < inpt.size(); index++) {
        cout << inpt[index].findTree() << endl;
    }
    return 0;
}
