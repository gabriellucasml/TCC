#ifndef WELL_H
#define WELL_H

#include <vector>

class Well{
private:
    int wellNumber;
    std::vector<double> injectedGas;
    std::vector<double> outputOil;
    double prodRate;
    double Qgmin;
    double Qomin;
    double maxGasInput;
    double maxOilOutput;
    double coefficients[3];
public:
    double getProdRate();
    double getQgmin();
    double getQomin();
    int getWellNumber();
    double getMaxGasInput();
    double getMaxOilOutput();
    void setMin();
    void setProdRate();
    void setOptima();
    void setWellNumber(int _wellNumber);
    std::vector<double> getInjectedGas();
    void setInjectedGas(std::vector<double> _injectedGas);
    std::vector<double> getOutputOil();
    void setOutputOil(std::vector<double> _outputOil);
    double* getCoeficients();
    void setCoeficients();
    double* eliminacaoGauss(int n, double **A, double *b,double *x);
    double GLPC(double Qg);
    bool operator==(const Well &w);
};

#endif