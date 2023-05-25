#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "../include/well.h"

using namespace std;

int Well::getWellNumber(){
    return this->wellNumber;
}

void Well::setWellNumber(int _wellNumber){
    this->wellNumber = _wellNumber;
}

vector<double> Well::getInjectedGas(){
    return this->injectedGas;
}

void Well::setInjectedGas(vector<double> _injectedGas){
    this->injectedGas = _injectedGas;
}

vector<double> Well::getOutputOil(){
    return this->outputOil;
}

void Well::setOutputOil(vector<double> _outputOil){
    this->outputOil = _outputOil;
}

double* Well::getCoeficients(){
    return this->coefficients;
}

void Well::setCoeficients(){
    std::string fileName = "well";
    fileName.append(to_string(this->wellNumber).append(".txt"));
    ofstream file;
    file.open(fileName);
    file << "3" << endl;
    file << "1 " << sqrt(this->injectedGas[floor(this->injectedGas.size()/4)]) << " " << this->injectedGas[floor(this->injectedGas.size()/4)] << " " << this->outputOil[floor(this->injectedGas.size()/4)] << endl;
    file << "1 " << sqrt(this->injectedGas[floor(this->injectedGas.size()/2)]) << " " << this->injectedGas[floor(this->injectedGas.size()/2)] << " " << this->outputOil[floor(this->injectedGas.size()/2)] << endl;
    file << "1 " << sqrt(this->injectedGas[this->injectedGas.size()-1]) << " " << this->injectedGas[this->injectedGas.size()-1] << " " << this->outputOil[this->injectedGas.size()-1] << endl;
    file.close();
    ifstream matrixFile;
    matrixFile.open(fileName);//arquivo da matriz
    string line;
    getline(matrixFile, line);
    int n = stoi(line);
    auto **A = new double*[n];
    for(int i = 0; i<n; i++)
        A[i]=new double[n];
    auto *b = new double[n];
    auto *x = new double[n];
    for(int i = 0; i < n; i++){
        stringstream l;
        getline(matrixFile, line);
        l.str(line);
        string aux[n+1];
        for(int j = 0; j <= n; j++){
            l >> aux[j];
            if(j<n){
                try{
                    A[i][j] = stod(aux[j]);
                }
                catch(exception e){
                    cout << "caught" << endl;
                    break;
                }
            }else{
                //cout << "on else: "<< j<< endl;
                try{
                    b[i] = stod(aux[j]);
                }catch(exception e){
                    break;
                }
            }
        }
    }
    this->eliminacaoGauss(n, A, b, x);
    for(int i = 0; i < n; i++){
        this->coefficients[i] = x[i];
    }
    matrixFile.close();
    std::remove(fileName.c_str());
}

double Well::getMaxGasInput(){
    return this->maxGasInput;
}

double Well::getMaxOilOutput(){
    return this->maxOilOutput;
}

void Well::setOptima(){
    /*this->maxGasInput = 0;
    this->maxOilOutput = 0;
    for(double i = 0; i < 100000; i+=0.1){
        if(this->GLPC(i)>this->maxOilOutput){
            this->maxGasInput = i;
            this->maxOilOutput = GLPC(i);
        }
    }*/

    double a = pow(this->coefficients[1], 2);
    double b = 4 * pow(coefficients[2], 2);
    if(b != 0) {
        this->maxGasInput = a/b;
        this->maxOilOutput = GLPC(this->getMaxGasInput());
    }
    else{
        this->maxGasInput = 0;
        this->maxOilOutput = GLPC(this->maxGasInput);
    }
}

double* Well::eliminacaoGauss(int n, double **A, double *b,double *x){
    //transforma A em uma matriz diagonal superior
    int iterations = 0;
    for(int k = 0; k < n; k++){
        iterations += 1;
        for(int i = k+1; i < n; i++){
            iterations += 1;
            double m = A[i][k]/A[k][k];
            A[i][k] = 0;
            for(int j = k+1; j < n; j++){
                iterations += 1;
                A[i][j] -= m*A[k][j];
            }
            b[i] -= m*b[k];
        }
    }
    //resolve o sistema linear
    for(int i = n-1; i>=0; i--){
        iterations += 1;
        double soma = 0;
        for(int j = i+1; j<n; j++){
            iterations += 1;
            soma += A[i][j]*x[j];
        }
        x[i] = (b[i]-soma)/A[i][i];
    }
    return x;
}

double Well::GLPC(double Qg){
    return this->coefficients[0] + this->coefficients[1] * sqrt(Qg) + this->coefficients[2] * Qg;
}

void Well::setMin(){
    double i = 0;
    int iterations = 0;
    while(this->GLPC(i)<0){
        this->Qgmin = i;
        this->Qomin = this->GLPC(i);
        i += 1;
        iterations += 1;
        if(iterations>100000)
            break;
    }
}

double Well::getQgmin(){
    return this->Qgmin;
}

double Well::getQomin(){
    return this->Qomin;
}

double Well::getProdRate() {
    return this->prodRate;
}

void Well::setProdRate() {
    this->maxGasInput > 0 ? prodRate = this->maxOilOutput/this->maxGasInput : prodRate = 0;
}

bool Well::operator==(const Well &w) {
    if(this->coefficients[0] == w.coefficients[0] && this->coefficients[1] == w.coefficients[1] && this->coefficients[2] == w.coefficients[2]){
        return true;
    }else{
        return false;
    }
}

