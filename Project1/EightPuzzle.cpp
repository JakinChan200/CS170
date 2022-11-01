#include <vector>
#include <iostream>
#include <queue>
#include <string>
#include <set>
#include <chrono>

using namespace std;
using namespace std::chrono;
int queueCounter = 0;
int expandedCounter = 0;

vector<vector<int>> problems = {{1, 2, 3, 4, 5, 6, 7, 8, 0}, //0
                                {1, 2, 3, 4, 5, 6, 0, 7, 8}, //2
                                {1, 2, 3, 5, 0, 6, 4, 7, 8}, //4
                                {1, 3, 6, 5, 0, 2, 4, 7, 8}, //8
                                {1, 3, 6, 5, 0, 7, 4, 8, 2}, //12
                                {1, 6, 7, 5, 0, 3, 4, 8, 2}, //16
                                {7, 1, 2, 4, 8, 5, 6, 3, 0}, //20
                                {0, 7, 2, 4, 6, 1, 3, 5, 8}}; //24

// vector<vector<int>> problems = {{1, 2, 3, 4, 5, 6, 0, 7, 8}};

struct node{
    vector<vector<int>> state;
    int zeroRow;
    int zeroCol;
    int g = 0;
    vector<string> path;
};

struct CompareG{
    bool operator()(node &a, node &b){
        return a.g > b.g;
    }
};

void printPuzzle(vector<vector<int>> problem){
    cout << endl;
    for(int i = 0; i < problem.size(); i++){
        for(int k = 0; k < problem[i].size(); k++){
            cout << problem[i][k] << " ";
        }
        cout << endl;
    }
    return;
}

bool checkComplete(node &puzzle){
    int dimension = puzzle.state.size() * puzzle.state[0].size();
    puzzle.g = 0;
    for(int i = 0; i < puzzle.state.size(); i++){
        for(int k = 0; k < puzzle.state[i].size(); k++){
            if(!(((3*i+k+1) %dimension) == puzzle.state[i][k])){
                puzzle.g++;
            }
        }
    }
    return puzzle.g == 0;
}

bool checkDupe(node &puzzle, set<vector<vector<int>>> &table){
    int initialSize = table.size();
    table.insert(puzzle.state);
    return initialSize == table.size();
}

node swapValues(node puzzle, int tileRow, int tileCol, string step){
    puzzle.state[puzzle.zeroRow][puzzle.zeroCol] = puzzle.state[tileRow][tileCol];
    puzzle.state[tileRow][tileCol] = 0;
    puzzle.zeroRow = tileRow;
    puzzle.zeroCol = tileCol;
    puzzle.path.push_back(step);
    return puzzle;
}

node buildNode(vector<vector<int>> state){
    node temp;
    temp.state = state;
    int dimension = state.size() * state[0].size();

    for(int i = 0; i < state.size(); i++){
        for(int k = 0; k < state[i].size(); k++){
            if(state[i][k] == 0){
                temp.zeroRow = i;
                temp.zeroCol = k;
                return temp;
            }
            if(!(((3*i+k+1) %dimension) == state[i][k])){
                temp.g++;
            }
        }
    }
    return temp;
}

node buildTree(vector<vector<int>> puzzle){

    queueCounter = 0;
    expandedCounter = 0;
    priority_queue<node, vector<node>, CompareG> tree;
    set<vector<vector<int>>> table;
    node temp;

    tree.push(buildNode(puzzle));
    node puzzleTop = tree.top();
    if(checkComplete(puzzleTop)) return puzzleTop;

    int row;
    int col;

    while(!tree.empty()){
        if(queueCounter < tree.size()){queueCounter = tree.size();}
        puzzleTop = tree.top();
        tree.pop();
        expandedCounter++;

        row = puzzleTop.zeroRow;
        col = puzzleTop.zeroCol;

        if(row != 0){
            temp = swapValues(puzzleTop, row-1, col, "up");
            if(!checkDupe(temp, table)){
                if(checkComplete(temp)){
                    return temp;
                }else{
                    tree.push(temp);
                }
            }
        }
        if(row != puzzle.size()-1){
            temp = swapValues(puzzleTop, row+1, col, "down");
            if(!checkDupe(temp, table)){
                if(checkComplete(temp)){
                    return temp;
                }else{
                    tree.push(temp);
                }
            }
        }

        if(col != 0){
            temp = swapValues(puzzleTop, row, col-1, "left");
            if(!checkDupe(temp, table)){
                if(checkComplete(temp)){
                    return temp;
                }else{
                    tree.push(temp);
                }
            }
        }
        if(col != puzzle[row].size()-1){
            temp = swapValues(puzzleTop, row, col+1, "right");
            if(!checkDupe(temp, table)){
                if(checkComplete(temp)){
                    return temp;
                }else{
                    tree.push(temp);
                }
            }
        }
    }
    return puzzleTop;
}

int main(int argc, char** argv){
    
    auto startAll = high_resolution_clock::now();
    for(int i = 0; i < problems.size(); i++){
        vector<vector<int>> problem;
        for(int k = 0; k < 3; k++){
            vector<int> row = {problems[i][k*3], problems[i][k*3+1], problems[i][k*3+2]};
            problem.push_back(row);
        }

        cout << "=================================================" << endl;

        printPuzzle(problem);
        auto start = high_resolution_clock::now();
        node sol = buildTree(problem);
        auto stop = high_resolution_clock::now();
        
        if(!sol.g){
            cout << "\nSolution:" << endl;
            cout << "Depth: " << sol.path.size() << endl;
            cout << "Path: ";
            for(int k = 0; k < sol.path.size(); k++){
                cout << sol.path[k] << " ";
            }
            cout << "\nMax Queue Length: " << queueCounter << endl;
            cout << "Nodes Expanded: " << expandedCounter << endl;
            cout << "Time: " << duration_cast<milliseconds>(stop - start).count() << " milliseconds" << endl;
            cout << endl;
        }else{
            cout << "\nNo Solution" << endl;
        }
    }
    auto stopAll = high_resolution_clock::now();

    cout << "=======================================================" << endl;
    cout << "Time to run all " << problems.size() << " tests: " << duration_cast<milliseconds>(stopAll - startAll).count() << " milliseconds" << endl;

    return 0;
}