#include <vector>
#include <iostream>
#include <queue>
#include <string>


using namespace std;

vector<vector<int>> problems = {{1, 2, 3, 4, 5, 6, 7, 8, 0}, //0
                                {1, 2, 3, 4, 5, 6, 0, 7, 8}, //2
                                {1, 2, 3, 5, 0, 6, 4, 7, 8}, //4
                                {1, 3, 6, 5, 0, 2, 4, 7, 8}, //8
                                {1, 3, 6, 5, 0, 7, 4, 8, 2}, //12
                                {1, 6, 7, 5, 0, 3, 4, 8, 2}, //16
                                {7, 1, 2, 4, 8, 5, 6, 3, 0}, //20
                                {0, 7, 2, 4, 6, 1, 3, 5, 8}}; //24


struct node{
    vector<vector<int>> state;
    int zeroRow;
    int zeroCol;
    int depth = 0;
    vector<string> path;
    bool solution = false;
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

bool checkComplete(node puzzle){
    int counter = 1;
    for(int i = 0; i < puzzle.state.size(); i++){
        for(int k = 0; k < puzzle.state[i].size(); k++){
            if(((3*i+k+1) %(puzzle.state.size()*puzzle.state[0].size())) == puzzle.state[puzzle.state.size()][puzzle.state[i].size()]){
                counter++;
            }else{
                return false;
            }
        }
    }
    return true;
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

    for(int i = 0; i < state.size(); i++){
        for(int k = 0; k < state[i].size(); k++){
            if(state[i][k] == 0){
                temp.zeroRow = i;
                temp.zeroCol = k;
            }
        }
    }
    return temp;
}

node buildTree(vector<vector<int>> puzzle){

    queue<node> tree;
    tree.push(buildNode(puzzle));
    int row;
    int col;

    node puzzleTop;
    while(!tree.empty()){
        puzzleTop = tree.front();
        if(checkComplete(puzzleTop)){
            puzzleTop.solution = true;
            return puzzleTop;
        }
        tree.pop();

        row = puzzleTop.zeroRow;
        col = puzzleTop.zeroCol;

        if(row == 0){
            tree.push(swapValues(puzzleTop, row+1, col, "dowm"));
        }else if(row == puzzle.size()-1){
            tree.push(swapValues(puzzleTop, row-1, col, "up"));
        }else{
            tree.push(swapValues(puzzleTop, row-1, col, "up"));
            tree.push(swapValues(puzzleTop, row+1, col, "down"));
        }

        if(col == 0){
            tree.push(swapValues(puzzleTop, row, col+1, "right"));
        }else if(col == puzzle[row].size()-1){
            tree.push(swapValues(puzzleTop, row, col-1, "left"));
        }else{
            tree.push(swapValues(puzzleTop, row, col-1, "left"));
            tree.push(swapValues(puzzleTop, row, col+1, "right"));
        }
    }
    return puzzleTop;
}

int main(int argc, char** argv){
    
    for(int i = 0; i < problems.size(); i++){
        vector<vector<int>> problem;
        for(int k = 0; k < 3; k++){
            vector<int> row = {problems[i][k*3], problems[i][k*3+1], problems[i][k*3+2]};
            problem.push_back(row);
        }

        cout << "=================================================" << endl;
        printPuzzle(problem);

        node sol = buildTree(problem);
        if(sol.solution){
            for(int k = 0; k < sol.path.size(); k++){
                cout << sol.path[k] << " ";
            }
            cout << endl;
        }else{
            cout << "No Solution" << endl;
        }
    }

    return 0;
}