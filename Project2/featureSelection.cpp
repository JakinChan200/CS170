#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <set>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 74 and 24
double bestAccuracyOverall = 0;
vector<int> bestFeaturesOverall;
auto anchor = high_resolution_clock::now();

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

void removeElement(vector<int> &newFeatures, vector<int> &features, int element){
    newFeatures.clear();
    for(int i = 0; i < features.size(); i++){
        if(features[i] != element){
            newFeatures.push_back(features[i]);
            //cout << i << " " << element << endl;
        }
    }
}

void populateVector(vector<int> &features, int size){
    for(int i = 1; i <= size; i++){
        features.push_back(i);
    }
}

double accuracyTest(const vector<vector<double>> &data, vector<int> &current_set){
    int numCorrectlyClassified = 0;
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
                //distance += pow(data[i][featureToAdd] - data[j][featureToAdd], 2);
                distance = sqrt(distance);

                if(distance < nearestNeighborDistance){
                    nearestNeighborDistance = distance;
                    nearestNeighborindex = j;
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
        vector<int> featureAdded;

        for(int j = 1; j < data[j].size(); j++){
            if(isDuplicate(currentFeatures, j)) continue;

            featureAdded = currentFeatures;
            featureAdded.push_back(j);
            sort(featureAdded.begin(), featureAdded.end());
            
            double accuracy = accuracyTest(data, featureAdded); //+1 j
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

        auto checkpoint = high_resolution_clock::now();
        cout << "The time to run this level was " << duration_cast<milliseconds>(checkpoint - anchor).count() << " milliseconds " << endl;
        anchor = checkpoint;
    }
}

void search2(const vector<vector<double>> &data){
    vector<int> currentFeatures;
    populateVector(currentFeatures, data[0].size()-1);
    bool isCurMaxima = false;
    vector<int> tempFeatures;

    cout << "Dataset has " << data[0].size()-1 << " features (not including the class attribute), with " << data.size() << " instances." << endl; 
    for(int i = 1; i < data[i].size(); i++){
        cout << "\nOn the " << i << "th level of the search tree" << endl;
        int featureToErase = 0;
        double bestAccuracy = 0;

        for(int j = 1; j < data[i].size(); j++){
            if(!isDuplicate(currentFeatures, j)) continue;
            
            removeElement(tempFeatures, currentFeatures, j);
            double accuracy = accuracyTest(data, tempFeatures);

            cout << "--Using feature(s) ";
            printSet(tempFeatures);
            cout << " after removing " << j << " gives an accuracy of " << accuracy << "\%." << endl;
            if(accuracy > bestAccuracy){
                bestAccuracy = accuracy;
                featureToErase = j;
                if(bestAccuracy > bestAccuracyOverall){
                    bestAccuracyOverall = bestAccuracy;
                    isCurMaxima = true;
                }
            }
        }
        removeElement(tempFeatures, currentFeatures, featureToErase);
        currentFeatures = tempFeatures;

        if(isCurMaxima){
            bestFeaturesOverall = currentFeatures;
            isCurMaxima = false;
        }
        cout << "Feature set ";
        printSet(currentFeatures);
        cout << " was the best, with an accuracy of " << bestAccuracy << "\%." << endl;

        auto checkpoint = high_resolution_clock::now();
        cout << "The time to run this level was " << duration_cast<milliseconds>(checkpoint - anchor).count() << " milliseconds " << endl;
        anchor = checkpoint;
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

    cout << "Would you like to run:\n\t1: Forward Selection\n\t2: Backwards Elimination" << endl;

    int searchType;
    cin >> searchType;

    while(searchType > 2 || searchType < 1){
        cout << "Not a valid search type, please try again." << endl;
        cout << "Would you like to run:\n\t1: Forward Selection\n\t 2: Backwards Elimination" << endl;
        cin >> searchType;
    }

    vector<vector<double>> data(0, vector<double>(0, 0));
    readFile(data, fileName);

    auto start = high_resolution_clock::now();
    anchor = high_resolution_clock::now();
    if(searchType == 1){
        search(data);
    }else{
        search2(data);
    }
    auto stop = high_resolution_clock::now();

    cout << "\nThe best features are ";
    printSet(bestFeaturesOverall);
    cout << " with an accuracy of " << bestAccuracyOverall << "\%." << endl;

    cout << "Time: " << duration_cast<milliseconds>(stop - start).count() << " milliseconds  " << endl;
    cout << "Time: " << duration_cast<seconds>(stop - start).count() << " seconds  " << endl;

    return 0;
}