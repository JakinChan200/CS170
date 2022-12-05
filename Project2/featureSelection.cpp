#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <bits/stdc++.h>

using namespace std;

// 74 and 24
double bestAccuracyOverall = 0;
vector<int> bestFeaturesOverall;

void readFile(vector<vector<double>> &data, string fileName){
    ifstream file;
    file.open(fileName);

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
    }

    file.close();
}

bool isDuplicate(const vector<int> &featureList, const int feature){
    if(!featureList.size()) return false;
    for(int i = 0; i < featureList.size(); i++){
        if(feature == featureList[i]){
            return true;
        }
    }
    return false;
}

void printSet(const vector<int> &featureList){
    cout << "{";
    for(int i = 0; i < featureList.size(); i++){
        cout << featureList[i];
        if(i != featureList.size()-1){
            cout << ", ";
        }
    }
    cout << "}";
}

double accuracyTest(const vector<vector<double>> &data, vector<int> &current_set, int featureToAdd){
    int numCorrectlyClassified = 0;
    //double nearestNeighborLabel;
    double distance;
    for(int i = 0; i < data.size(); i++){
        double nearestNeighborDistance = numeric_limits<double>::max();
        int nearestNeighborindex = numeric_limits<int>::max();
        for(int j = 0; j < data.size(); j++){
            distance = 0;
            if(j != i){
                for(int k = 0; k < current_set.size(); k++){
                    distance += pow(data[i][current_set[k]] - data[j][current_set[k]], 2);
                }
                distance += pow(data[i][featureToAdd] - data[j][featureToAdd], 2);
                distance = sqrt(distance);
                
                if(distance < nearestNeighborDistance){
                    nearestNeighborDistance = distance;
                    nearestNeighborindex = j;
                    //nearestNeighborLabel = data[j][0];
                }
            }
        }
        if(data[i][0] == data[nearestNeighborindex][0]){
            numCorrectlyClassified++;
        }
    }
    return (double)numCorrectlyClassified/data.size() * 100;
}

void search(const vector<vector<double>> &data){
    vector<int> currentFeatures;
    bool isCurMaxima = false;

    cout << "Dataset has " << data[0].size()-1 << " features (not including the class attribute), with " << data.size() << " instances." << endl; 
    for(int i = 1; i < data[i].size(); i++){
        cout << "\nOn the " << i << "th level of the search tree" << endl;
        int featureToAdd;
        double bestAccuracy = 0;

        for(int j = 1; j < data[j].size(); j++){
            if(isDuplicate(currentFeatures,j)) continue;

            double accuracy = accuracyTest(data, currentFeatures, j); //+1
            cout << "--Using feature(s) ";
            printSet(currentFeatures);
            cout << " while considering " << j << " gives an accuracy of " << accuracy << "\%." << endl;
            if(accuracy > bestAccuracy){
                bestAccuracy = accuracy;
                featureToAdd = j;
                if(bestAccuracy > bestAccuracyOverall){
                    bestAccuracyOverall = bestAccuracy;
                    isCurMaxima = true;
                }
            }
        }
        currentFeatures.push_back(featureToAdd);
        sort(currentFeatures.begin(), currentFeatures.end());

        if(isCurMaxima){
            bestFeaturesOverall = currentFeatures;
            isCurMaxima = false;
        }
        cout << "Feature set ";
        printSet(currentFeatures);
        cout << " was the best, with an accuracy of " << bestAccuracy << "\%." << endl;
    }
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

    search(data);

    cout << "\nThe best features are ";
    printSet(bestFeaturesOverall);
    cout << " with an accuracy of " << bestAccuracyOverall << "\%." << endl;
    return 0;
}