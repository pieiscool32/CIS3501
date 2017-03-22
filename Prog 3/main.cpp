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
#include <algorithm>

using namespace std;

struct edge {
    int parent, weight, count;
};

struct input {
    int node1, node2, weight, store;
    
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
        edge temp;
        
        for(int pos = 0; pos < junctions; pos++) {
            temp.weight = 0;
            temp.parent = -1;
            temp.count = -1;
            connections.push_back(temp);
        }
    }
    
    vector<edge> getSet() {
        return connections;
    }
    
    
    int doFind(const vector<edge> input, int find) {
        //preforms set find
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
                connections[b].parent = a;
                connections[a].count += connections[b].count;
            } else {
                connections[a].parent = b;
                connections[b].count += connections[a].count;
            }
        } else {
            return true;
        }
        return false;
    }
    
    void doPrint() {
        for (int pos = 0; pos < connections.size(); pos++) {
            cout << pos+1 << "   ";
        }
        cout << endl;
        for (int pos = 0; pos < connections.size(); pos++) {
            if(connections[pos].parent == -1) {
                cout << connections[pos].count;
            } else {
                cout << connections[pos].parent + 1 << "  ";
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
            temp.store = -1;
            inputs.push_back(temp);
        }
    }
    
    int diff() {
        restoreWeights();
        sort(span.begin(), span.end());
        return span.back().store - span.front().store;
    }
    
    void doBias(int bias) {
        if(bias == 0) {
            int average = 0;
            for(int index = 0; index < inputs.size(); index++) {
                average += inputs[index].store;
            }
            bias = average / inputs.size();
        }
        
        for(int index = 0; index < inputs.size(); index++) {
            inputs[index].weight = bias - inputs[index].store;
        }
    }
    
    void storeWeights() {
        for(int index = 0; index < inputs.size(); index++) {
            inputs[index].store = inputs[index].weight;
        }
    }
    
    void restoreWeights() {
        for(int index = 0; index < span.size(); index++) {
            span[index].weight = span[index].store;
        }
    }
    
    int checkAround(int pos, int amount) {
        int average = 0, lower = 0, upper = 0, diff = 0;
        if(amount - pos < 0) {
            lower = 0;
            upper = amount;
        } else if (amount + pos > inputs.size()) {
            upper = inputs.size();
            lower = upper - amount;
        } else {
            lower = pos - (amount/2);
            upper = pos + (amount/2);
        }
        for(int index = lower; index < upper; index ++) {
            average += inputs[index].store;
        }
        average = average / amount;
        for(int index = lower; index < upper; index ++) {
            diff += abs(average - inputs[index].store);
        }
        return diff;
    }
    
    int clusterBias() {
        int bias = INT_MAX, result, pos = 0;
        for(int num = 0; num < inputs.size(); num++) {
            result = checkAround(num, junctions-1);
            if(result < bias) {
                bias = result;
                pos = num;
            }
        }
        return inputs[pos].weight;
    }
    
    int findTree(vector<input> tree) {
        set mst(junctions);
        
        while(inputs.size() > 0 && span.size() < junctions - 1) {
            //uses kruskal Algo
            if(!mst.doUnion(tree.front().node1, tree.front().node2)) {
                //if it doesn't make a cycle
                span.push_back(inputs.front());
                tree.erase(tree.begin());
            } else {
                return -1;
            }
        }
        //mst.doPrint();
        return diff();
    }
};

int main() { //4 5 1 2 3 1 3 5 1 4 6 2 4 6 3 4 7
    int junct, edges;
    vector<tree> inpt;
    cin >> junct >> edges;
    while (junct != 0) {
        //cout << junct << " junctions and " << edges << " edges\n";
        tree given(edges, junct);
        inpt.push_back(given);
        cin >> junct >> edges;
    }
    for (int index = 0; index < inpt.size(); index++) {
        cout << inpt[index].findTree(inpt[index].clusterBias()) << endl;
    }
    return 0;
}
