/*#include <iostream>
#include <cstdio>
#include <chrono>
#include <list>
#include <vector>
#include <iterator>
#include "include/field.h"

using namespace std;

int main(){
    Field field("/home/gabriel/CLionProjects/untitled/data/well-56.txt");
    field.setGasTotal(3000);
    cout << "GA Running..." << endl;
    field.setSolution(400, 0.8, 0.4, 100);
    cout << "Done." << endl << endl << "MS running..." << endl;
    field.setSolution(100, 0.1);
    cout << "Done." << endl << endl << "SA running..." << endl;
    field.setSolution(1000.0, 10.0, 100, 60);
    cout << "Done." << endl << endl << "M1 Running..." << endl;
    field.setSolution(400, 0.8, 0.4, 100, 10);
    cout << "Done." << endl << endl << "M2 Running..." << endl;
    field.setSolution(400, 0.8, 0.4, 100, 0.1);
    cout << "Done." << endl << endl << "M3 Running..." << endl;
    field.setSolution(400, 0.8, 0.4, 100, 100.0, 10.0, 10);
    cout << "Done." << endl;

}*/

#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>
#include <random>
#include <iterator>
#include <bits/stdc++.h>
#include "include/field.h"
#include "include/well.h"

using namespace std;

int main(){
    time_t start, end;
    Field field6("../data/well-6.txt");
    Field field10("../data/well-10.txt");
    Field field15("../data/well-15.txt");
    Field field20("../data/well-20.txt");
    Field field30("../data/well-30.txt");
    Field field40("../data/well-40.txt");
    Field field50("../data/well-50.txt");
    Field field56("../data/well-56.txt");
    field6.setGasTotal(3000);
    field10.setGasTotal(4000);
    field15.setGasTotal(5000);
    field20.setGasTotal(6000);
    field30.setGasTotal(7000);
    field40.setGasTotal(8000);
    field50.setGasTotal(9000);
    field56.setGasTotal(10000);
    double oilSoultion[24];
    double gasSolution[24];
    double times[24];
    double oilMean = 0, gasMean = 0;
    double oilVar = 0, gasVar = 0;;
    for(int i = 0; i < 24; i++){
        time(&start);
        field6.setSolution(500, 0.6, 0.1, 50);
        time(&end);
        oilSoultion[i] = field6.getOptimalOil();
        gasSolution[i] = field6.getOptimalGas();
        oilMean += field6.getOptimalOil();
        gasMean += field6.getOptimalGas();
    }
    oilMean = oilMean/24;
    gasMean = gasMean/24;
    for(int i = 0; i < 24; i++){
        oilVar+=pow((oilSoultion[i]-oilMean),2);
        gasVar+=pow((gasSolution[i]-gasMean),2);
    }
    oilVar = oilVar/24;
    gasVar = gasVar/24;
    auto time_taken = double(end-start);
    cout<<"24 exectutions for the genetic algorith:"<<endl;
    cout<<"oil Mean: "<< oilMean<< endl;
    cout<<"oil variance: "<< oilVar << endl;
    cout<<"oil sd: "<< sqrt(oilVar)<<endl;
    cout<<"gas Mean: "<< gasMean << endl;
    cout<<"gas varianve" << gasVar << endl;
    cout<<"gas sd: " << sqrt(gasVar) << endl;
    cout<<"time taken to execute 24 times: " << time_taken << setprecision(5) << "s" << endl;
}