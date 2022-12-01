#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void readFile(vector<vector<double>> &data, string fileName){
    ifstream file;
    file.open(fileName);

    if(!file.is_open()){
        cout << "bruh";
    }
    stringstream row();
    string curRow = "";
    double curValue = 0;

    while(getline(file, curRow)){
        stringstream row(curRow);
        vector<double> temp;
        while(row >> curValue){
            temp.push_back(curValue);
        }
        data.push_back(temp);
        cout << "temp size " << temp.size() << endl;
    }

    file.close();
}

int main(int argc, char** argv){    
    string fileName = "datasets/CS170_";
    cout << "Welcome to Jakin Chan Feature Selection Algorithm" << endl;

    cout << "Large or small file? ('l' or 's'): " << endl;

    char fileType = 'l';
    cin >> fileType;

    while(fileType != 'l' && fileType != 's'){
        cout << "Wrong file type." << endl;
        cout << "Large or small file? ('l' or 's'): " << endl;
        cin >> fileType;
    }

    cout << "What is the file number? (1 - 125): " << endl;

    int fileNumber;
    cin >> fileNumber;

    while(fileNumber > 125 || fileNumber < 1){
        cout << "File Number must be between 1 and 125" << endl;
        cout << "What is the file number? (1 - 125): " << endl;
        cin >> fileNumber;
    }

    if(fileType == 'l'){
        fileName += "Large";
    }else{
        fileName += "Small";
    }

    fileName += "_Data__" + to_string(fileNumber) + ".txt";
    cout << fileName << endl;

    vector<vector<double>> data(0, vector<double>(0, 0));
    readFile(data, fileName);

    cout << data.size() << endl;
    cout << data[0].size() << endl;

    // for(int i = 0; i < data.size(); i++){
    //     for(int j = 0; j < data[0].size(); j++){
    //         cout << "i: " << i << " j: " << j << " " << data[i][j] << endl;
    //     }
    //     cout << endl;
    // }
    return 0;
}