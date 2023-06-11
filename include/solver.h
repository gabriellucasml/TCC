//
// Created by gabriel on 04/05/23.
//

#ifndef UNTITLED_SOLVER_H
#define UNTITLED_SOLVER_H

#include "field.h"

class Solver{
public:
    static void GeneticAlgorithm(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter);
    static void MultiStart(Field* field, int max_iteration, double minProdRate);
    static void SimulatedAnnealing(Field* field, double initialTemp, double finalTemp, int iterations, int numNeighbors);
    /*Classic implementation of Memetic Algorithm (GA + LS)*/
    static void Memetic1(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter, int LS_neighborhoodSize);
    /*Genetic + Constructive method from multi-start + Local search*/
    static void Memetic2(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter, double minProdRate);
    /*Genetic + SA*/
    static void Memetic3(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter, double initialTemp, double finalTemp, int numNeighbors);
    static double evalSolution(Field* field, const std::vector<double>& sol);
    static std::vector<double> localSearch(Field* field, const std::vector<double>& sol, int numNeighbors);
    static std::vector<double> generateRandomSolution(Field* field);
private:
    static std::vector<double> SA_LS(Field* field, std::vector<double> initialSol, double initialTemp, double finalTemp, int iterations, int numNeighbors);
};

#endif //UNTITLED_SOLVER_H
