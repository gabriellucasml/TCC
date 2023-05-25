#include <iostream>
#include <cstdio>
#include <chrono>
#include <list>
#include <vector>
#include <iterator>
#include "include/field.h"

using namespace std;

int main(){
    Field field("/home/gabriel/CLionProjects/untitled/data/well-six.txt");
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

}