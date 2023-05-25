#ifndef FIELD_H
#define FIELD_H
#include "well.h"
#include <string>

class Field{
private:
    int numWells;
    double gasTotal;
    double optimalGas, optimalOil;
    std::vector<Well> wells;
    std::vector<double> solution;
    std::string filePath;
public:
    Field();
    explicit Field(std::string _filePath);
    std::string getFilePath();
    void setFilePath(std::string _filePath);
    int getNumWells();
    void setNumWells();
    std::vector<Well> getWells();
    void setWells();
    std::vector<double> getSolution();
    void setSolution(std::vector<double> sol);
    void setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter);
    void setSolution(int max_iteration, double minProdRate);
    void setSolution(double initialTemp, double finalTemp, int iterations, int numNeighbors);
    void setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter, int LS_neighborhoodSize);
    void setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter, double minProdRate);
    void setSolution(int popSize, double crossoverRate, double mutationRate, int maxIter, double initialTemp, double finalTemp, int numNeighbors);
    double getGasTotal();
    double getOptimalGas() const;
    void setOptimalGas(double optimalGas);
    double getOptimalOil() const;
    void setOptimalOil(double optimalOil);
    void setGasTotal(double _gasTotal);
    Field& operator=(const Field &f);
};

#endif