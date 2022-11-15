/*
Given a stick, split it in 2 random spots (the spots can be the same). What are the chances that the three resulting sticks can form a triangle?
*/

#include <iostream>
#include <math.h>
#include <chrono>
#include<bits/stdc++.h>

using namespace std;

void ratioTriangle(){
    int count = 0;
    int iterations = 10000000;
    float stick, stick1, stick2;
    srand(time(0));

    for(int i = 0; i < iterations; i++){
        stick1 = (float)rand()/RAND_MAX;
        stick2 = (float)rand()/RAND_MAX;
        if(stick1 > stick2){
            stick1 -= stick2;
        }else{
            stick2 -= stick1;
        }
        stick = 1 - stick1 - stick2;

        if(!(stick+stick1 <= stick2 || stick1+stick2 <= stick || stick+stick2 <= stick1)){
            count++;
        }
    }

    cout << "With " << iterations << " iterations, the ratio is: " << fixed << setprecision(3) << (float)count / iterations << endl;
    return;
}
int main(int argc, char** argv){
    for(int i = 0; i < 10; i++){
        ratioTriangle();
    }
    return 0;
}