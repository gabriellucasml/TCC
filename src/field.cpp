#include <cstdio>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <utility>
#include <vector>
#include <random>
#include <iterator>
#include "../include/field.h"
#include "../include/solver.h"

using namespace std;

Field::Field()= default;

Field::Field(string _filePath){
    filePath = std::move(_filePath);
    cout << "Setting wells..." << endl << "\t Reading files..." << endl;
    setNumWells();
    for(int i = 0; i<numWells; i++){
        Well well;
        well.setWellNumber(i);
        wells.push_back(well);
    }
    setWells();//O(m^2n)
    cout << "\t Calculating well information" << endl;
    for(auto & well : wells){//n
        well.setCoeficients();//O(1)
        well.setOptima();//O(1)
        well.setMin();
        well.setProdRate();
    }
    cout << "Done." << endl;
}
string Field::getFilePath(){
    return this->filePath;
}

void Field::setFilePath(std::string _filePath){
    this->filePath = _filePath;
}

int Field::getNumWells(){
    return this->numWells;
}

void Field::setNumWells(){
    ifstream file;
    file.open(this->filePath);
    string n;
    getline(file, n);
    this->numWells = stod(n);
    file.close();
}

vector<Well> Field::getWells(){
    return this->wells;
}

void Field::setWells(){
    ifstream file;
    file.open(this->filePath);
    string n;
    getline(file, n);
    vector<double> _injectedGas;
    vector<double> _outputOil;
    for(auto & well : this->wells){
        stringstream l;
        getline(file, n);
        l.str(n);
        string aux;
        while(!l.eof()){
            l >> aux;
            _injectedGas.push_back(stod(aux));
        }
        well.setInjectedGas(_injectedGas);
        _injectedGas.clear();
        getline(file, n);
        l.clear();
        l.seekg(0);
        l.str(n);
        while(!l.eof()){
            l>>aux;
            _outputOil.push_back(stod(aux));
        }
        well.setOutputOil(_outputOil);
        _outputOil.clear();
        l.str("");
    }
}

vector<double> Field::getSolution(){
    return this->solution;
}

//implementação da metaheurística
void Field::setSolution(vector<double> sol){
    this->solution = std::move(sol);
}
//Genetic
void Field::setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter){
    Solver::GeneticAlgorithm(this, popSize, crossoverRate, mutationRate, maxIter);
}
//Multi-start
void Field::setSolution(int max_iteration, double minProdRate) {
    Solver::MultiStart(this, max_iteration, minProdRate);
}
//Simulated Annealing
void Field::setSolution(double initialTemp, double finalTemp, int iterations, int numNeighbors) {
    Solver::SimulatedAnnealing(this, initialTemp, finalTemp, iterations, numNeighbors);
}

void Field::setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter, int LS_neighborhoodSize){
    Solver::Memetic1(this, popSize, crossoverRate, mutationRate, maxIter, LS_neighborhoodSize);
}

void Field::setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter, double minProdRate){
    Solver::Memetic2(this, popSize, crossoverRate, mutationRate, maxIter, minProdRate);
}

void Field::setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter, double initialTemp, double finalTemp, int numNeighbors){
    Solver::Memetic3(this, popSize, crossoverRate, mutationRate, maxIter, initialTemp, finalTemp, numNeighbors);
}

double Field::getGasTotal(){
    return this->gasTotal;
}

void Field::setGasTotal(double _gasTotal){
    this->gasTotal = _gasTotal;
}

Field& Field::operator=(const Field &f){
    this->numWells = f.numWells;
    this->gasTotal = f.gasTotal;
    this->wells = f.wells;
    this->solution = f.solution;
    this->filePath = f.filePath;
    return *this;
}

double Field::getOptimalGas() const {
    return optimalGas;
}

void Field::setOptimalGas(double optimalGas) {
    Field::optimalGas = optimalGas;
}

double Field::getOptimalOil() const {
    return optimalOil;
}

void Field::setOptimalOil(double optimalOil) {
    Field::optimalOil = optimalOil;
}