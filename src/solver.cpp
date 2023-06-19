//
// Created by gabriel on 04/05/23.
//
#include "../include/solver.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <utility>
#include <vector>
#include <random>
#include <iterator>

using namespace std;

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

void Solver::GeneticAlgorithm(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter) {
    field->evaluations = 0;
    cout << "Optimizing solution..." << endl;
    random_device rd;// only used once to initialise (seed) engine
    mt19937 rng(rd());// random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> allele(0, field->getNumWells());//guaranteed unbiased
    uniform_real_distribution<> real(0,1);
    uniform_real_distribution<> mutationValue(0,2);
    double Gmax = 0;
    double Omax = 0;
    for(auto it : field->getWells()){
        Gmax += it.getMaxGasInput();
        Omax += it.getMaxOilOutput();
    }//O(n)
    /*Checks to see if the amount of gas available is enough to allocate all wells to their max oil output*/
    if(Gmax < field->getGasTotal()){//O(2n)
        ofstream file;
        file.open("solucao.txt");
        for(auto & it : field->getWells()){
            file << it.getMaxGasInput() << " ";
        }
        file << endl;
        for(auto & it : field->getWells()){
            file << it.getMaxOilOutput() << " ";
        }
        file << endl;
        file << "Total optimal gas injection: " << Gmax << " MSCF/D" << endl;
        file << "Total optimal oil output: " << Omax << " B/D" << endl;
        file.close();
        /*If that is not the case, begin optimizing*/
    }else{
        vector<double> currentBest;
        double currentFitness = 0;
        //generating first generation
        vector<vector<double>> population(popSize);
        vector<double> fitness(popSize);
        for(int i = 0; i < popSize; i++){//O(2mn)
            vector<double> randFactor(field->getNumWells());
            double randSum = 0;
            for(int j = 0; j < field->getNumWells(); j++){
                randFactor[j] = real(rng);
                randSum += randFactor[j];
            }
            for(int j = 0; j < field->getNumWells(); j++){
                population[i].push_back(field->getGasTotal()*randFactor[j]/randSum);
                if(population[i][j]<field->getWells()[j].getQgmin()){
                    randSum -= randFactor[j];
                    randFactor[j] = 0;
                }
            }
        }
        ofstream points;
        points.open(to_string(field->getNumWells())+"genetic_points.txt", fstream::in | fstream::out | fstream::trunc);
        for(int iteration = 0; iteration < maxIter; iteration++){//O(i)
            if(field->evaluations > 100000000)
                break;
            //look for fitest
            int fitestIndex = 0;
            double fitest = 0;
            for(int i = 0; i < popSize; i++){
                if(fitness[i]>fitest){
                    fitestIndex = i;
                    fitest = fitness[i];
                }
            }//O(m)
            currentBest = population[fitestIndex];
            currentFitness = fitness[fitestIndex];
            //elitism
            vector<vector<double>> nextGen;
            vector<vector<double>> elite;
            elite.push_back(population[fitestIndex]);
            for(int i = 0; i < popSize; i++){
                if(i != fitestIndex && fitness[i]/fitness[fitestIndex]>=0.95){
                    elite.push_back(population[i]);
                }
            }
            //crossover and mutation
            while(nextGen.size() < popSize){//O(2mn)
                //single point crossover
                double p1Fitness = 0, p2Fitness = 0, child1Fitness = 0, child2Fitness = 0;
                vector<double> p1 = *select_randomly(elite.begin(), elite.end());//random parent from elite
                vector<double> p2 = *select_randomly(population.begin(), population.end());//random parent from general population
                vector<double> child1(field->getNumWells()), child2(field->getNumWells());
                double crossoverFactor = real(rng);
                if(crossoverFactor<=crossoverRate){
                    for(int i = 0; i < field->getNumWells(); i++){//O(n)
                        if(i < floor(field->getNumWells()*0.76)){
                            child1[i] = p1[i];
                            child2[i] = p2[i];
                        }else{
                            child1[i] = p2[i];
                            child2[i] = p1[i];
                        }
                    }
                    for(int i = 0; i<field->getNumWells(); i++){//O(n)
                        p1Fitness += (field->getWells()[i].GLPC(p1[i]) >= 0 ? field->getWells()[i].GLPC(p1[i]) : 0); field->evaluations++;
                        p2Fitness += (field->getWells()[i].GLPC(p2[i]) >= 0 ? field->getWells()[i].GLPC(p2[i]) : 0); field->evaluations++;
                        child1Fitness += (field->getWells()[i].GLPC(child1[i]) >= 0 ? field->getWells()[i].GLPC(child1[i]) : 0); field->evaluations++;
                        child2Fitness += (field->getWells()[i].GLPC(child2[i]) >= 0 ? field->getWells()[i].GLPC(child2[i]) : 0); field->evaluations++;
                    }
                    double P[] = {p1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), p2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness)};
                    double accumulatedP = 0;
                    double weightedProbability = real(rng);
                    for(int i = 0; i<4; i++){//O(1)
                        accumulatedP += P[i];
                        if(weightedProbability <= accumulatedP){
                            if(i==0){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(p1);
                            }else if(i==1){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(p2);
                            }else if(i==2){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(child1);
                            }else if(i==3){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(child2);
                            }
                        }
                    }
                }
            }
            //mutations
            for(int i = 0; i<popSize; i++){//O(m)
                double mutationFactor = real(rng);
                if(mutationFactor<=mutationRate){
                    int mutationIndex = allele(rng);
                    nextGen[i][mutationIndex] *= mutationValue(rng);
                }
            }
            population = nextGen;
            nextGen.clear();
            elite.clear();
            //reevaluate fitness
            double injectedGas = 0;
            double geneFitness = 0;
            for(int i = 0; i < popSize; i++){//O(mn)
                injectedGas = 0;
                geneFitness = 0;
                for(int j = 0; j < field->getNumWells(); j++){//O(n)
                    injectedGas += population[i][j];
                    geneFitness += (field->getWells()[j].GLPC(population[i][j]) >= 0 ? field->getWells()[j].GLPC(population[i][j]) : 0); field->evaluations++;
                }
                if(injectedGas > field->getGasTotal()){
                    fitness[i] = 0;
                }else{
                    fitness[i] = geneFitness;
                }
            }
            points << iteration << " " << currentFitness << endl;
        }
        points.close();
        double totalInjection = 0;
        for(int i = 0; i < field->getNumWells(); i++){
            totalInjection += currentBest[i];
        }
        fstream file;
        file.open("solucao.txt", fstream::in | fstream::out | fstream::app);
        if(!file){
            cout << "solution log files does not exist. Creating." << endl;
            file.open("solucao.txt", fstream::in | fstream::out | fstream::trunc);
        }else
            cout << "\t writing solution to log" << endl;

        file << "-----------------------------------------------------------------Genetic Algorithm-----------------------------------------------------------------" << endl << endl;
        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << currentBest[i] << " ";
        }
        file<<endl;
        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << (field->getWells()[i].GLPC(currentBest[i])>=0 ? field->getWells()[i].GLPC(currentBest[i]):0) << " "; field->evaluations++;
        }
        file << endl << endl;
        file << "Total gas available: " << field->getGasTotal() << endl;
        file << "Maximum possible oil production: " << Omax <<" B/D"<< endl;
        file << "Gas input necessary: " << Gmax << " MSCF/D" << endl << endl;
        file << "Total optimal gas injection: " << totalInjection << " MSCF/D" << endl;
        field->setOptimalGas(totalInjection);
        file << "Total optimal oil output: " << currentFitness << " B/D" << endl;
        field->setOptimalOil(currentFitness);
        file.close();
        cout << "Done." << endl;
    }
}

void Solver::MultiStart(Field* field, int max_iteration, double minProdRate){
    field->evaluations = 0;
    double Gmax = 0;
    double Omax = 0;
    vector<double> bestSol(field->getNumWells(),0);
    for(auto it : field->getWells()){
        Gmax += it.getMaxGasInput();
        Omax += it.getMaxOilOutput();
    }
    if(Gmax < field->getGasTotal()){
        ofstream file;
        file.open("solucao.txt");
        for(auto it : field->getWells()){
            file << it.getMaxGasInput() << " ";
        }
        file << endl;
        for(auto it : field->getWells()){
            file << it.getMaxOilOutput() << " ";
        }
        file << endl;
        file << "Total optimal gas injection: " << Gmax << " MSCF/D" << endl;
        file << "Total optimal oil output: " << Omax << " B/D" << endl;
        file.close();
    }else{
        double currBestGas = 0;
        double currBestOil = 0;
        vector<Well> candidates;
        vector<Well> lrc;
        ofstream points;
        points.open(to_string(field->getNumWells())+"multi_start_points.txt", fstream::in | fstream::out | fstream::trunc);
        for(int it = 0; it < max_iteration; it++){
            if(field->evaluations > 100000000)
                break;
            //fase cosntrutiva
            //gerar lista restrita de candidatos
            vector<double> currSol(field->getNumWells(), 0);
            for (auto well : field->getWells()){
                if(well.getProdRate() >= minProdRate){
                    lrc.push_back(well);
                }
            }//n
            while(currBestGas < field->getGasTotal() && !lrc.empty()){
                auto wellIter = select_randomly(lrc.begin(), lrc.end());
                Well well = *wellIter;
                lrc.erase(wellIter);
                if(currBestGas + well.getMaxGasInput() <= field->getGasTotal()){
                    currBestGas += well.getMaxGasInput();
                    currBestOil += well.getMaxOilOutput();
                    candidates.push_back(well);
                }
            }//n
            //candidato a solução
            for(Well well : candidates){
                currSol[well.getWellNumber()] = well.getMaxGasInput();
            }//n
            //fase de busca
            currSol = localSearch(field, currSol, 1000);

            if(evalSolution(field, bestSol) < evalSolution(field, currSol))
                bestSol = currSol;
            points << it << " " << evalSolution(field, currSol) << endl;
            currSol.clear();
            currBestGas = 0;
            currBestOil = 0;
            lrc.clear();
            candidates.clear();
        }

        fstream file;
        file.open("solucao.txt", fstream::in | fstream::out | fstream::app);
        if(!file){
            cout << "solution log files does not exist. Creating." << endl;
            file.open("solucao.txt", fstream::in | fstream::out | fstream::trunc);
        }else
            cout << "\t writing solution to log" << endl;
        file << endl << "------------------------------------------------------------Multi-start Algorithm-----------------------------------------------------------" << endl << endl;

        for(double gas : bestSol){
            file << gas << " ";
        }
        int i = 0;
        for(auto n : bestSol){
            currBestGas += n;
            currBestOil += (field->getWells()[i].GLPC(n)>0 ? field->getWells()[i].GLPC(n) : 0); field->evaluations++;
            i++;
        }//n
        field->setSolution(bestSol);
        file << endl;
        for(int j = 0; j < field->getNumWells(); j++){
            file << (field->getWells()[j].GLPC(bestSol[j]) > 0 ? field->getWells()[j].GLPC(bestSol[j]) : 0) << " "; field->evaluations++;
        }
        file << endl << endl;
        file << "Total gas available: " << field->getGasTotal() << endl;
        file << "Maximum possible oil production: " << Omax <<" B/D"<< endl;
        file << "Gas input necessary: " << Gmax << " MSCF/D" << endl << endl;
        file << "Total optimal gas injection: " << currBestGas << " MSCF/D" << endl;
        field->setOptimalGas(currBestGas);
        file << "Total optimal oil output: " << currBestOil << " B/D" << endl;
        field->setOptimalOil(currBestOil);
        file.close();
    }
}

void Solver::SimulatedAnnealing(Field* field, double initialTemp, double finalTemp, int iterations, int numNeighbors){
    field->evaluations = 0;
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<> real(0,1);
    uniform_real_distribution<> neighborFactor(1, 10);
    uniform_int_distribution<> index(0,field->getNumWells()-1);
    uniform_int_distribution<> neighborhoodIndex(0, numNeighbors-1);
    double Gmax = 0;
    double Omax = 0;
    for(auto & it : field->getWells()){
        Gmax += it.getMaxGasInput();
        Omax += it.getMaxOilOutput();
    }
    if(Gmax < field->getGasTotal()){
        ofstream file;
        file.open("solucao.txt");

        for(auto & it : field->getWells()){
            file << it.getMaxGasInput() << " ";
        }
        file << endl;
        for(auto & it : field->getWells()){
            file << it.getMaxOilOutput() << " ";
        }
        file << endl;
        file << "Total optimal gas injection: " << Gmax << " MSCF/D" << endl;
        file << "Total optimal oil output: " << Omax << " B/D" << endl;
        file.close();
    }else{
        vector<double> sol, bestSol;
        sol = generateRandomSolution(field);
        bestSol = sol;
        ofstream points;
        points.open(to_string(field->getNumWells())+"simulated_annealing_points.txt", fstream::in | fstream::out | fstream::trunc);
        for(int iter = 0; iter < iterations; iter++){
            if(field->evaluations > 100000000)
                break;
            double temp = initialTemp;
            //generate neighborhood
            vector<vector<double>> neighborhood(numNeighbors);
            for(int j = 0; j < numNeighbors; j++){
                double randFactor = neighborFactor(rng);
                int index1,index2;
                do{
                    index1 = index(rng);
                    index2 = index(rng);
                }while(index1 == index2);
                vector<double> nextNeighbor(field->getNumWells(),0);
                for(int i = 0; i < field->getNumWells(); i ++) {
                    if (i != index1 && i != index2) {
                        nextNeighbor[i] = bestSol[i];
                    } else if (i == index1) {
                        if (bestSol[index1] - randFactor >= 0) {
                            nextNeighbor[index1] = bestSol[index1] - randFactor;
                        } else {
                            nextNeighbor[index1] = 0;
                        }
                    } else if (i == index2) {
                        if (bestSol[index1] - randFactor >= 0) {
                            nextNeighbor[index2] = bestSol[index2] + randFactor;
                        } else {
                            nextNeighbor[index2] = bestSol[index1] + bestSol[index2];
                        }
                    }
                }
                neighborhood[j] = nextNeighbor;
            }
            while(temp > finalTemp){

                vector<double> randSol = neighborhood[neighborhoodIndex(rng)];
                double var = evalSolution(field, sol) - evalSolution(field, randSol);
                if(var < 0){
                    sol = randSol;
                    if(evalSolution(field, sol)>evalSolution(field, bestSol)){
                        bestSol = sol;
                    }
                }else if(real(rng)>exp(-var/temp)){
                    sol = randSol;
                }

                double beta = (initialTemp - finalTemp)/(iterations*initialTemp*finalTemp);
                temp = temp/(1+beta*temp);
            }
            points << iter << " " << evalSolution(field, sol) << endl;
            neighborhood.clear();
        }
        points.close();
        fstream file;
        file.open("solucao.txt", fstream::in | fstream::out | fstream::app);
        if(!file){
            cout << "solution log files does not exist. Creating." << endl;
            file.open("solucao.txt", fstream::in | fstream::out | fstream::trunc);
        }else
            cout << "\t writing solution to log" << endl;

        file << endl << "------------------------------------------------------------Simulated Annealing Algorithm-----------------------------------------------------------" << endl << endl;

        double utilizedGas = 0;
        for(double d : bestSol){
            file << d << ", ";
            utilizedGas += d;
        }
        file << endl;
        int i = 0;
        for(double d : bestSol){
            file << (field->getWells()[i].GLPC(d) > 0 ? field->getWells()[i].GLPC(d) : 0) << ", "; field->evaluations++;
            i++;
        }
        i = 0;
        double solucaoOleo = evalSolution(field, bestSol);
        double noGasOilProd = 0;
        for(Well well : field->getWells()){
            noGasOilProd += (well.GLPC(0) > 0 ? well.GLPC(0): 0); field->evaluations++;
            i++;
        }
        file << endl;
        file << endl << endl;

        file << "Total gas available: " << field->getGasTotal() << endl;
        file << "Maximum possible oil production: " << Omax <<" B/D"<< endl;
        file << "Gas input necessary: " << Gmax << " MSCF/D" << endl;
        file << "Oil production without gas injection: " << noGasOilProd << endl<< endl;

        file << "Total optimal gas injection: " << utilizedGas << " MSCF/D" << endl;
        field->setOptimalGas(utilizedGas);
        file << "Total optimal oil output: " << solucaoOleo << " B/D" << endl;
        field->setOptimalOil(solucaoOleo);
        file.close();
    }
}

void Solver::Memetic1(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter, int LS_neighborhoodSize){
    field->evaluations = 0;
    random_device rd;// only used once to initialise (seed) engine
    mt19937 rng(rd());// random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> allele(0, field->getNumWells());//guaranteed unbiased
    uniform_real_distribution<> real(0,1);
    uniform_real_distribution<> mutationValue(0,1);
    double Gmax = 0;
    double Omax = 0;
    for(auto it : field->getWells()){
        Gmax += it.getMaxGasInput();
        Omax += it.getMaxOilOutput();
    }//O(n)
    if(Gmax < field->getGasTotal()){//O(2n)
        ofstream file;
        file.open("solucao.txt");
        for(auto & it : field->getWells()){
            file << it.getMaxGasInput() << " ";
        }
        file << endl;
        for(auto & it : field->getWells()){
            file << it.getMaxOilOutput() << " ";
        }
        file << endl;
        file << "Total optimal gas injection: " << Gmax << " MSCF/D" << endl;
        file << "Total optimal oil output: " << Omax << " B/D" << endl;
        file.close();
    }else{
        vector<double> currentBest;
        double currentFitness = 0;
        //generating first generation
        vector<vector<double>> population(popSize);
        vector<double> fitness(popSize);
        for(int i = 0; i < popSize; i++){//O(2mn)
            vector<double> randFactor(field->getNumWells());
            double randSum = 0;
            for(int j = 0; j < field->getNumWells(); j++){
                randFactor[j] = real(rng);
                randSum += randFactor[j];
            }
            for(int j = 0; j < field->getNumWells(); j++){
                population[i].push_back(field->getGasTotal()*randFactor[j]/randSum);
                if(population[i][j]<field->getWells()[j].getQgmin()){
                    randSum -= randFactor[j];
                    randFactor[j] = 0;
                }
            }
        }
        ofstream points;
        points.open(to_string(field->getNumWells())+"memetic1_points.txt", fstream::in | fstream::out | fstream::trunc);
        for(int iteration = 0; iteration < maxIter; iteration++){//O(i)
            if(field->evaluations > 100000000)
                break;
            //look for fittest
            int fittestIndex = 0;
            double fittest = 0;
            for(int i = 0; i < popSize; i++){
                if(fitness[i] > fittest){
                    fittestIndex = i;
                    fittest = fitness[i];
                }
            }//O(m)
            currentBest = population[fittestIndex];
            currentFitness = fitness[fittestIndex];
            //elitism
            vector<vector<double>> nextGen;
            vector<vector<double>> elite;
            elite.push_back(population[fittestIndex]);
            for(int i = 0; i < popSize; i++){
                if(i != fittestIndex && fitness[i] / fitness[fittestIndex] >= 0.95){
                    elite.push_back(population[i]);
                }
            }
            //crossover and mutation
            while(nextGen.size() < popSize){//O(2mn)
                //single point crossover
                double p1Fitness = 0, p2Fitness = 0, child1Fitness = 0, child2Fitness = 0;
                vector<double> p1 = *select_randomly(elite.begin(), elite.end());//random parent from elite
                vector<double> p2 = *select_randomly(population.begin(), population.end());//random parent from general population
                vector<double> child1(field->getNumWells()), child2(field->getNumWells());
                double crossoverFactor = real(rng);
                if(crossoverFactor<=crossoverRate){
                    for(int i = 0; i < field->getNumWells(); i++){//O(n)
                        if(i < floor(field->getNumWells()*0.76)){
                            child1[i] = p1[i];
                            child2[i] = p2[i];
                        }else{
                            child1[i] = p2[i];
                            child2[i] = p1[i];
                        }
                    }
                    for(int i = 0; i<field->getNumWells(); i++){//O(n)
                        p1Fitness += field->getWells()[i].GLPC(p1[i]); field->evaluations++;
                        p2Fitness += field->getWells()[i].GLPC(p2[i]); field->evaluations++;
                        child1Fitness += field->getWells()[i].GLPC(child1[i]); field->evaluations++;
                        child2Fitness += field->getWells()[i].GLPC(child2[i]); field->evaluations++;
                    }
                    double P[] = {p1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), p2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness)};
                    double accumulatedP = 0;
                    double weightedProbability = real(rng);
                    for(int i = 0; i<4; i++){
                        accumulatedP += P[i];
                        if(weightedProbability <= accumulatedP){
                            if(i==0){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, p1, LS_neighborhoodSize));
                            }else if(i==1){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, p2, LS_neighborhoodSize));
                            }else if(i==2){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, child1, LS_neighborhoodSize));
                            }else if(i==3){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, child2, LS_neighborhoodSize));
                            }
                        }
                    }
                }
            }
            //mutations
            for(int i = 0; i<popSize; i++){//O(m)
                double mutationFactor = real(rng);
                if(mutationFactor<=mutationRate){
                    int mutationIndex = allele(rng);
                    nextGen[i][mutationIndex] += mutationValue(rng);
                }
            }
            population = nextGen;
            nextGen.clear();
            elite.clear();
            //reevaluate fitness
            double injectedGas = 0;
            double geneFitness = 0;
            for(int i = 0; i < popSize; i++){//O(mn)
                injectedGas = 0;
                geneFitness = 0;
                for(int j = 0; j < field->getNumWells(); j++){//O(n)
                    injectedGas += population[i][j];
                    geneFitness += (field->getWells()[j].GLPC(population[i][j]) >= 0 ? field->getWells()[j].GLPC(population[i][j]) : 0); field->evaluations++;
                }
                //cout << "generation " << iteration << " gene "<< i << " gas:" << injectedGas << endl;
                if(injectedGas > field->getGasTotal()){
                    fitness[i] = 0;
                }else{
                    fitness[i] = geneFitness;
                }
            }
            points << iteration << " " << currentFitness << endl;
        }
        points.close();
        double totalInjection = 0;
        for(int i = 0; i<field->getNumWells(); i++){
            totalInjection += currentBest[i];
        }
        fstream file;
        file.open("solucao.txt", fstream::in | fstream::out | fstream::app);
        if(!file){
            cout << "solution log files does not exist. Creating." << endl;
            file.open("solucao.txt", fstream::in | fstream::out | fstream::trunc);
        }else
            cout << "\t writing solution to log" << endl;

        file << "-----------------------------------------------------------------Memetic1 Algorithm-----------------------------------------------------------------" << endl << endl;

        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << currentBest[i] << " ";
        }
        file<<endl;
        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << (field->getWells()[i].GLPC(currentBest[i])>=0 ? field->getWells()[i].GLPC(currentBest[i]):0) << " "; field->evaluations++;
        }
        file << endl << endl;
        file << "Total gas available: " << field->getGasTotal() << endl;
        file << "Maximum possible oil production: " << Omax <<" B/D"<< endl;
        file << "Gas input necessary: " << Gmax << " MSCF/D" << endl << endl;
        file << "Total optimal gas injection: " << totalInjection << " MSCF/D" << endl;
        field->setOptimalGas(totalInjection);
        file << "Total optimal oil output: " << currentFitness << " B/D" << endl;
        field->setOptimalOil(currentFitness);
        file.close();

    }
}

void Solver::Memetic2(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter, double minProdRate){
    field->evaluations = 0;
    cout << "Optimizing solution..." << endl;
    random_device rd;// only used once to initialise (seed) engine
    mt19937 rng(rd());// random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> allele(0, field->getNumWells());//guaranteed unbiased
    uniform_real_distribution<> real(0,1);
    uniform_real_distribution<> mutationValue(0,2);
    double Gmax = 0;
    double Omax = 0;
    for(auto it : field->getWells()){
        Gmax += it.getMaxGasInput();
        Omax += it.getMaxOilOutput();
    }//O(n)
    /*Checks to see if the amount of gas available is enough to allocate all wells to their max oil output*/
    if(Gmax < field->getGasTotal()){//O(2n)
        ofstream file;
        file.open("solucao.txt");
        for(auto & it : field->getWells()){
            file << it.getMaxGasInput() << " ";
        }
        file << endl;
        for(auto & it : field->getWells()){
            file << it.getMaxOilOutput() << " ";
        }
        file << endl;
        file << "Total optimal gas injection: " << Gmax << " MSCF/D" << endl;
        file << "Total optimal oil output: " << Omax << " B/D" << endl;
        file.close();
        /*If that is not the case, begin optimizing*/
    }else{
        vector<double> currentBest;
        double currentFitness = 0;
        //generating first generation
        vector<vector<double>> population(popSize);
        vector<double> fitness(popSize);
        vector<Well> lrc;
        for(int i = 0; i < popSize; i++){//O(2mn)
            vector<double> currSol(field->getNumWells(), 0);
            vector<Well> candidates;
            for (auto well : field->getWells()){
                if(well.getProdRate() >= minProdRate){
                    lrc.push_back(well);
                }
            }//n
            while(currentFitness < field->getGasTotal() && !lrc.empty()){
                auto wellIter = select_randomly(lrc.begin(), lrc.end());
                Well well = *wellIter;
                lrc.erase(wellIter);
                if(currentFitness + well.getMaxGasInput() <= field->getGasTotal()){
                    currentFitness += well.getMaxGasInput();
                    candidates.push_back(well);
                }
            }//n
            //candidato a solução
            for(Well well : candidates){
                currSol[well.getWellNumber()] = well.getMaxGasInput();
            }//n
            population[i] = currSol;
            currSol.clear();
            candidates.clear();
        }
        ofstream points;
        points.open(to_string(field->getNumWells())+"memetic2_points.txt", fstream::in | fstream::out | fstream::trunc);
        for(int iteration = 0; iteration < maxIter; iteration++){//O(i)
            if(field->evaluations > 100000000)
                break;
            //look for fittest
            int fittestIndex = 0;
            double fittest = 0;
            for(int i = 0; i < popSize; i++){
                if(fitness[i] > fittest){
                    fittestIndex = i;
                    fittest = fitness[i];
                }
            }//O(m)
            currentBest = population[fittestIndex];
            currentFitness = fitness[fittestIndex];
            //elitism
            vector<vector<double>> nextGen;
            vector<vector<double>> elite;
            elite.push_back(population[fittestIndex]);
            for(int i = 0; i < popSize; i++){
                if(i != fittestIndex && fitness[i] / fitness[fittestIndex] >= 0.95){
                    elite.push_back(population[i]);
                }
            }
            //crossover and mutation
            while(nextGen.size() < popSize){//O(2mn)
                //single point crossover
                double p1Fitness = 0, p2Fitness = 0, child1Fitness = 0, child2Fitness = 0;
                vector<double> p1 = *select_randomly(elite.begin(), elite.end());//random parent from elite
                vector<double> p2 = *select_randomly(population.begin(), population.end());//random parent from general population
                vector<double> child1(field->getNumWells()), child2(field->getNumWells());
                double crossoverFactor = real(rng);
                if(crossoverFactor<=crossoverRate){
                    for(int i = 0; i < field->getNumWells(); i++){//O(n)
                        if(i < floor(field->getNumWells()*0.76)){
                            child1[i] = p1[i];
                            child2[i] = p2[i];
                        }else{
                            child1[i] = p2[i];
                            child2[i] = p1[i];
                        }
                    }
                    for(int i = 0; i<field->getNumWells(); i++){//O(n)
                        p1Fitness += field->getWells()[i].GLPC(p1[i]); field->evaluations++;
                        p2Fitness += field->getWells()[i].GLPC(p2[i]); field->evaluations++;
                        child1Fitness += field->getWells()[i].GLPC(child1[i]); field->evaluations++;
                        child2Fitness += field->getWells()[i].GLPC(child2[i]); field->evaluations++;
                    }
                    double P[] = {p1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), p2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness)};
                    double accumulatedP = 0;
                    double weightedProbability = real(rng);
                    for(int i = 0; i<4; i++){//O(1)
                        accumulatedP += P[i];
                        if(weightedProbability <= accumulatedP){
                            if(i==0){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, p1, 10));
                            }else if(i==1){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, p2, 10));
                            }else if(i==2){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, child1, 10));
                            }else if(i==3){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(localSearch(field, child2, 10));
                            }
                        }
                    }
                }
            }
            //mutations
            for(int i = 0; i<popSize; i++){//O(m)
                double mutationFactor = real(rng);
                if(mutationFactor<=mutationRate){
                    int mutationIndex = allele(rng);
                    nextGen[i][mutationIndex] *= mutationValue(rng);
                }
            }
            population = nextGen;
            nextGen.clear();
            elite.clear();
            //reevaluate fitness
            double injectedGas = 0;
            double geneFitness = 0;
            for(int i = 0; i < popSize; i++){//O(mn)
                injectedGas = 0;
                geneFitness = 0;
                for(int j = 0; j < field->getNumWells(); j++){//O(n)
                    injectedGas += population[i][j];
                    geneFitness += (field->getWells()[j].GLPC(population[i][j]) >= 0 ? field->getWells()[j].GLPC(population[i][j]) : 0); field->evaluations++;
                }
                if(injectedGas > field->getGasTotal()){
                    fitness[i] = 0;
                }else{
                    fitness[i] = geneFitness;
                }
            }
            points << iteration << " " << currentFitness << endl;
        }
        points.close();
        double totalInjection = 0;
        for(int i = 0; i < field->getNumWells(); i++){
            totalInjection += currentBest[i];
        }
        fstream file;
        file.open("solucao.txt", fstream::in | fstream::out | fstream::app);
        if(!file){
            cout << "solution log files does not exist. Creating." << endl;
            file.open("solucao.txt", fstream::in | fstream::out | fstream::trunc);
        }else
            cout << "\t writing solution to log" << endl;

        file << "-----------------------------------------------------------------Memetic2 Algorithm-----------------------------------------------------------------" << endl << endl;
        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << currentBest[i] << " ";
        }
        file<<endl;
        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << (field->getWells()[i].GLPC(currentBest[i])>=0 ? field->getWells()[i].GLPC(currentBest[i]):0) << " "; field->evaluations++;
        }
        file << endl << endl;
        file << "Total gas available: " << field->getGasTotal() << endl;
        file << "Maximum possible oil production: " << Omax <<" B/D"<< endl;
        file << "Gas input necessary: " << Gmax << " MSCF/D" << endl << endl;
        file << "Total optimal gas injection: " << totalInjection << " MSCF/D" << endl;
        field->setOptimalGas(totalInjection);
        file << "Total optimal oil output: " << currentFitness << " B/D" << endl;
        field->setOptimalOil(currentFitness);
        file.close();
        cout << "Done." << endl;
    }
}

void Solver::Memetic3(Field* field, int popSize, double crossoverRate, double mutationRate, int maxIter, double initialTemp, double finalTemp, int numNeighbors){
    field->evaluations = 0;
    cout << "Optimizing solution..." << endl;
    random_device rd;// only used once to initialise (seed) engine
    mt19937 rng(rd());// random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> allele(0, field->getNumWells());//guaranteed unbiased
    uniform_real_distribution<> real(0,1);
    uniform_real_distribution<> mutationValue(0,2);
    double Gmax = 0;
    double Omax = 0;
    for(auto it : field->getWells()){
        Gmax += it.getMaxGasInput();
        Omax += it.getMaxOilOutput();
    }//O(n)
    /*Checks to see if the amount of gas available is enough to allocate all wells to their max oil output*/
    if(Gmax < field->getGasTotal()){//O(2n)
        ofstream file;
        file.open("solucao.txt");
        for(auto & it : field->getWells()){
            file << it.getMaxGasInput() << " ";
        }
        file << endl;
        for(auto & it : field->getWells()){
            file << it.getMaxOilOutput() << " ";
        }
        file << endl;
        file << "Total optimal gas injection: " << Gmax << " MSCF/D" << endl;
        file << "Total optimal oil output: " << Omax << " B/D" << endl;
        file.close();
        /*If that is not the case, begin optimizing*/
    }else{
        vector<double> currentBest;
        double currentFitness = 0;
        //generating first generation
        vector<vector<double>> population(popSize);
        vector<double> fitness(popSize);
        for(int i = 0; i < popSize; i++){//O(2mn)
            vector<double> randFactor(field->getNumWells());
            double randSum = 0;
            for(int j = 0; j < field->getNumWells(); j++){
                randFactor[j] = real(rng);
                randSum += randFactor[j];
            }
            for(int j = 0; j < field->getNumWells(); j++){
                population[i].push_back(field->getGasTotal()*randFactor[j]/randSum);
                if(population[i][j]<field->getWells()[j].getQgmin()){
                    randSum -= randFactor[j];
                    randFactor[j] = 0;
                }
            }
        }
        ofstream points;
        points.open(to_string(field->getNumWells())+"memetic3_points.txt", fstream::in | fstream::out | fstream::trunc);
        for(int iteration = 0; iteration < maxIter; iteration++){//O(i)
            if(field->evaluations > 100000000)
                break;
            //look for fitest
            int fitestIndex = 0;
            double fitest = 0;
            for(int i = 0; i < popSize; i++){
                if(fitness[i]>fitest){
                    fitestIndex = i;
                    fitest = fitness[i];
                }
            }//O(m)
            currentBest = population[fitestIndex];
            currentFitness = fitness[fitestIndex];
            //elitism
            vector<vector<double>> nextGen;
            vector<vector<double>> elite;
            elite.push_back(population[fitestIndex]);
            for(int i = 0; i < popSize; i++){
                if(i != fitestIndex && fitness[i]/fitness[fitestIndex]>=0.95){
                    elite.push_back(population[i]);
                }
            }
            //crossover and mutation
            while(nextGen.size() < popSize){//O(2mn)
                //single point crossover
                double p1Fitness = 0, p2Fitness = 0, child1Fitness = 0, child2Fitness = 0;
                vector<double> p1 = *select_randomly(elite.begin(), elite.end());//random parent from elite
                vector<double> p2 = *select_randomly(population.begin(), population.end());//random parent from general population
                vector<double> child1(field->getNumWells()), child2(field->getNumWells());
                double crossoverFactor = real(rng);
                if(crossoverFactor<=crossoverRate){
                    for(int i = 0; i < field->getNumWells(); i++){//O(n)
                        if(i < floor(field->getNumWells()*0.76)){
                            child1[i] = p1[i];
                            child2[i] = p2[i];
                        }else{
                            child1[i] = p2[i];
                            child2[i] = p1[i];
                        }
                    }
                    for(int i = 0; i<field->getNumWells(); i++){//O(n)
                        p1Fitness += field->getWells()[i].GLPC(p1[i]); field->evaluations++;
                        p2Fitness += field->getWells()[i].GLPC(p2[i]); field->evaluations++;
                        child1Fitness += field->getWells()[i].GLPC(child1[i]); field->evaluations++;
                        child2Fitness += field->getWells()[i].GLPC(child2[i]); field->evaluations++;
                    }
                    double P[] = {p1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), p2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child1Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness), child2Fitness/(p1Fitness+p2Fitness+child1Fitness+child2Fitness)};
                    double accumulatedP = 0;
                    double weightedProbability = real(rng);
                    for(int i = 0; i<4; i++){//O(1)
                        accumulatedP += P[i];
                        if(weightedProbability <= accumulatedP){
                            if(i==0){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(SA_LS(field, p1, initialTemp, finalTemp, 1, numNeighbors));
                            }else if(i==1){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(SA_LS(field, p2, initialTemp, finalTemp, 1, numNeighbors));
                            }else if(i==2){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(SA_LS(field, child1, initialTemp, finalTemp, 1, numNeighbors));
                            }else if(i==3){
                                if(nextGen.size()+1<=popSize)
                                    nextGen.push_back(SA_LS(field, child2, initialTemp, finalTemp, 1, numNeighbors));
                            }
                        }
                    }
                }
            }
            //mutations
            for(int i = 0; i<popSize; i++){//O(m)
                double mutationFactor = real(rng);
                if(mutationFactor<=mutationRate){
                    int mutationIndex = allele(rng);
                    nextGen[i][mutationIndex] *= mutationValue(rng);
                }
            }
            population = nextGen;
            nextGen.clear();
            elite.clear();
            //reevaluate fitness
            double injectedGas = 0;
            double geneFitness = 0;
            for(int i = 0; i < popSize; i++){//O(mn)
                injectedGas = 0;
                geneFitness = 0;
                for(int j = 0; j < field->getNumWells(); j++){//O(n)
                    injectedGas += population[i][j];
                    geneFitness += (field->getWells()[j].GLPC(population[i][j]) >= 0 ? field->getWells()[j].GLPC(population[i][j]) : 0); field->evaluations++;
                }
                if(injectedGas > field->getGasTotal()){
                    fitness[i] = 0;
                }else{
                    fitness[i] = geneFitness;
                }
            }
            points << iteration << " " << currentFitness << endl;
        }
        points.close();
        double totalInjection = 0;
        for(int i = 0; i < field->getNumWells(); i++){
            totalInjection += currentBest[i];
        }
        fstream file;
        file.open("solucao.txt", fstream::in | fstream::out | fstream::app);
        if(!file){
            cout << "solution log files does not exist. Creating." << endl;
            file.open("solucao.txt", fstream::in | fstream::out | fstream::trunc);
        }else
            cout << "\t writing solution to log" << endl;

        file << "-----------------------------------------------------------------Memetic3 Algorithm-----------------------------------------------------------------" << endl << endl;
        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << currentBest[i] << " ";
        }
        file<<endl;
        for(int i = 0; i < field->getNumWells(); i++){//O(n)
            file << (field->getWells()[i].GLPC(currentBest[i])>=0 ? field->getWells()[i].GLPC(currentBest[i]):0) << " "; field->evaluations++;
        }
        file << endl << endl;
        file << "Total gas available: " << field->getGasTotal() << endl;
        file << "Maximum possible oil production: " << Omax <<" B/D"<< endl;
        file << "Gas input necessary: " << Gmax << " MSCF/D" << endl << endl;
        file << "Total optimal gas injection: " << totalInjection << " MSCF/D" << endl;
        field->setOptimalGas(totalInjection);
        file << "Total optimal oil output: " << currentFitness << " B/D" << endl;
        field->setOptimalOil(currentFitness);
        file.close();
        cout << "Done." << endl;
    }
}

vector<double> Solver::generateRandomSolution(Field* field){
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<> real(0,1);
    vector<double> sol;
    vector<double> randFactor(field->getNumWells());
    double randSum = 0;
    for(int j = 0; j < field->getNumWells(); j++){
        randFactor[j] = real(rng);
        randSum += randFactor[j];
    }
    for(int j = 0; j < field->getNumWells(); j++){
        sol.push_back(field->getGasTotal()*randFactor[j]/randSum);
        if(sol[j]<field->getWells()[j].getQgmin()){
            randSum -= randFactor[j];
            randFactor[j] = 0;
        }
    }
    return sol;
}

double Solver::evalSolution(Field* field, const vector<double>& sol){
    double custo = 0;
    for(int i = 0; i < field->getNumWells(); i++){
        custo += (field->getWells()[i].GLPC(sol[i]) > 0 ? field->getWells()[i].GLPC(sol[i]) : 0); field->evaluations++;
    }
    return custo;
}

vector<double> Solver::localSearch(Field* field, const vector<double>& sol, int numNeighbors){
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<> real(1, 500);
    uniform_int_distribution<> index(0,field->getNumWells()-1);
    vector<double> bestSol(sol);
    for(int i = 0; i < numNeighbors; i++){
        double randFactor = real(rng);
        int index1,index2;
        do{
            index1 = index(rng);
            index2 = index(rng);
        }while(index1 == index2);
        vector<double> nextNeighbor(field->getNumWells());
        for(int j = 0; j < field->getNumWells(); j++){
            if(j != index1 && j != index2){
                nextNeighbor[j] = bestSol[j];
            }else if(j == index1) {
                if (bestSol[index1] - randFactor >= 0) {
                    nextNeighbor[index1] = bestSol[index1] - randFactor;
                } else {
                    nextNeighbor[index1] = 0;
                }
            }else if(j == index2){
                if (bestSol[index1] - randFactor >= 0){
                    nextNeighbor[index2] = bestSol[index2] + randFactor;
                } else {
                    nextNeighbor[index2] = bestSol[index1] + bestSol[index2];
                }
            }
        }
        if(evalSolution(field, nextNeighbor)>evalSolution(field, bestSol)){
            bestSol = nextNeighbor;
        }
        nextNeighbor.clear();
    }
    return bestSol;
}

std::vector<double> Solver::SA_LS(Field *field, vector<double> initialSol, double initialTemp, double finalTemp, int iterations, int numNeighbors) {
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<> real(0,1);
    uniform_real_distribution<> neighborFactor(1, 10);
    uniform_int_distribution<> index(0,field->getNumWells()-1);
    uniform_int_distribution<> neighborhoodIndex(0, numNeighbors-1);
    double Gmax = 0;
    double Omax = 0;
    for(auto & it : field->getWells()){
        Gmax += it.getMaxGasInput();
        Omax += it.getMaxOilOutput();
    }
    vector<double> sol, bestSol;
    sol = std::move(initialSol);
    bestSol = sol;
    for (int iter = 0; iter < iterations; iter++) {
        double temp = initialTemp;
        //generate neighborhood
        vector<vector<double>> neighborhood(numNeighbors);
        for (int j = 0; j < numNeighbors; j++) {
            double randFactor = neighborFactor(rng);
            int index1, index2;
            do {
                index1 = index(rng);
                index2 = index(rng);
            } while (index1 == index2);
            vector<double> nextNeighbor(field->getNumWells(), 0);
            for (int i = 0; i < field->getNumWells(); i++) {
                if (i != index1 && i != index2) {
                    nextNeighbor[i] = bestSol[i];
                } else if (i == index1) {
                    if (bestSol[index1] - randFactor >= 0) {
                        nextNeighbor[index1] = bestSol[index1] - randFactor;
                    } else {
                        nextNeighbor[index1] = 0;
                    }
                } else if (i == index2) {
                    if (bestSol[index1] - randFactor >= 0) {
                        nextNeighbor[index2] = bestSol[index2] + randFactor;
                    } else {
                        nextNeighbor[index2] = bestSol[index1] + bestSol[index2];
                    }
                }
            }
            neighborhood[j] = nextNeighbor;
        }
        while (temp > finalTemp) {

            vector<double> randSol = neighborhood[neighborhoodIndex(rng)];
            double var = evalSolution(field, sol) - evalSolution(field, randSol);
            if (var < 0) {
                sol = randSol;
                if (evalSolution(field, sol) > evalSolution(field, bestSol)) {
                    bestSol = sol;
                }
            } else if (real(rng) > exp(-var / temp)) {
                sol = randSol;
            }

            double beta = (initialTemp - finalTemp) / (iterations * initialTemp * finalTemp);
            temp = temp / (1 + beta * temp);
        }
        neighborhood.clear();
    }
    return bestSol;
}

