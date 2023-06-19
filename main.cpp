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
using namespace std::chrono;

int main(){
    int repetitions = 24;
    Field field6("../data/well-6.txt");
    Field field10("../data/well-10.txt");
    Field field16("../data/well-16.txt");
    Field field20("../data/well-20.txt");
    Field field30("../data/well-30.txt");
    Field field40("../data/well-40.txt");
    Field field50("../data/well-50.txt");
    Field field56("../data/well-56.txt");

    field6.setGasTotal(3000);
    field10.setGasTotal(4000);
    field16.setGasTotal(5000);
    field20.setGasTotal(6000);
    field30.setGasTotal(7000);
    field40.setGasTotal(8000);
    field50.setGasTotal(9000);
    field56.setGasTotal(10000);

    fstream file;
    file.open("log.txt", fstream::in | fstream::out | fstream::app);
    double solutionGas[repetitions], solutionOil[repetitions], solutionTime[repetitions];

    /*file << "Field 6" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field6.setSolution(500, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field6.getOptimalOil();
        solutionGas[i] = field6.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field6.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field6.getOptimalOil();
        solutionGas[i] = field6.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field6.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field6.getOptimalOil();
        solutionGas[i] = field6.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field6.setSolution(400, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field6.getOptimalOil();
        solutionGas[i] = field6.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field6.setSolution(400, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field6.getOptimalOil();
        solutionGas[i] = field6.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field6.setSolution(400, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field6.getOptimalOil();
        solutionGas[i] = field6.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;

    file << "Field 10" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field10.setSolution(500, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field10.getOptimalOil();
        solutionGas[i] = field10.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field10.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field10.getOptimalOil();
        solutionGas[i] = field10.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field10.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field10.getOptimalOil();
        solutionGas[i] = field10.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field10.setSolution(400, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field10.getOptimalOil();
        solutionGas[i] = field10.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field10.setSolution(400, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field10.getOptimalOil();
        solutionGas[i] = field10.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field10.setSolution(400, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field10.getOptimalOil();
        solutionGas[i] = field10.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;

    file << "Field 16" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field16.setSolution(500, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field16.getOptimalOil();
        solutionGas[i] = field16.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field16.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field16.getOptimalOil();
        solutionGas[i] = field16.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field16.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field16.getOptimalOil();
        solutionGas[i] = field16.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field16.setSolution(400, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field16.getOptimalOil();
        solutionGas[i] = field16.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field16.setSolution(400, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field16.getOptimalOil();
        solutionGas[i] = field16.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field16.setSolution(400, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field16.getOptimalOil();
        solutionGas[i] = field16.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;

    file << "Field 20" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field20.setSolution(500, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field20.getOptimalOil();
        solutionGas[i] = field20.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field20.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field20.getOptimalOil();
        solutionGas[i] = field20.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field20.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field20.getOptimalOil();
        solutionGas[i] = field20.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field20.setSolution(400, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field20.getOptimalOil();
        solutionGas[i] = field20.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field20.setSolution(400, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field20.getOptimalOil();
        solutionGas[i] = field20.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;*/

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field20.setSolution(100, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field20.getOptimalOil();
        solutionGas[i] = field20.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;

    file << "Field 30" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field30.setSolution(100, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field30.getOptimalOil();
        solutionGas[i] = field30.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field30.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field30.getOptimalOil();
        solutionGas[i] = field30.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field30.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field30.getOptimalOil();
        solutionGas[i] = field30.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field30.setSolution(100, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field30.getOptimalOil();
        solutionGas[i] = field30.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field30.setSolution(100, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field30.getOptimalOil();
        solutionGas[i] = field30.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field30.setSolution(100, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field30.getOptimalOil();
        solutionGas[i] = field30.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;

    file << "Field 40" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field40.setSolution(100, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field40.getOptimalOil();
        solutionGas[i] = field40.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field40.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field40.getOptimalOil();
        solutionGas[i] = field40.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field40.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field40.getOptimalOil();
        solutionGas[i] = field40.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field40.setSolution(100, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field40.getOptimalOil();
        solutionGas[i] = field40.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field40.setSolution(100, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field40.getOptimalOil();
        solutionGas[i] = field40.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field40.setSolution(100, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field40.getOptimalOil();
        solutionGas[i] = field40.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;

    file << "Field 50" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field50.setSolution(100, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field50.getOptimalOil();
        solutionGas[i] = field50.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field50.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field50.getOptimalOil();
        solutionGas[i] = field50.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field50.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field50.getOptimalOil();
        solutionGas[i] = field50.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field50.setSolution(100, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field50.getOptimalOil();
        solutionGas[i] = field50.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field50.setSolution(100, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field50.getOptimalOil();
        solutionGas[i] = field50.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field50.setSolution(100, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field50.getOptimalOil();
        solutionGas[i] = field50.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;

    file << "Field 56" << endl;
    cout << "Genetic." << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field56.setSolution(100, 0.6, 0.1, 50);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field56.getOptimalOil();
        solutionGas[i] = field56.getOptimalGas();
    }
    file << "Genetic:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Multi-start:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field56.setSolution(50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field56.getOptimalOil();
        solutionGas[i] = field56.getOptimalGas();
    }
    file << "Multi-start:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Simulated Annealing" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field56.setSolution(1000.0, 10.0, 50, 60);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field56.getOptimalOil();
        solutionGas[i] = field56.getOptimalGas();
    }
    file << "Simulated Annealing:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 1:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field56.setSolution(100, 0.8, 0.4, 50, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field56.getOptimalOil();
        solutionGas[i] = field56.getOptimalGas();
    }
    file << "Memetic 1:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 2" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field56.setSolution(100, 0.8, 0.4, 50, 0.1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field56.getOptimalOil();
        solutionGas[i] = field56.getOptimalGas();
    }
    file << "Memetic 2:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl;

    cout << "Memetic 3:" << endl;
    for(int i = 0; i < repetitions; i++) {
        cout << i << endl;
        auto start = high_resolution_clock::now();
        field56.setSolution(100, 0.8, 0.4, 50, 100.0, 10.0, 10);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        solutionTime[i] = double(duration.count())/1000000;
        solutionOil[i] = field56.getOptimalOil();
        solutionGas[i] = field56.getOptimalGas();
    }
    file << "Memetic 3:" << endl;
    file << "Oil: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionOil[i] << ", ";
    file << endl;
    file << "Gas: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionGas[i] << ", ";
    file << endl;
    file << "Time: ";
    for(int i = 0; i < repetitions; i++)
        file << solutionTime[i] << ", ";
    file << endl << endl << endl;
    file.close();
}