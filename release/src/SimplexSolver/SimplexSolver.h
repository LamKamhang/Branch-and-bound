#pragma once

#include <cinttypes>
#include <Eigen/Dense>

using namespace Eigen;

#define SIMPLEX_MINIMIZE 1
#define SIMPLEX_MAXIMIZE 2

class SimplexSolver {
private:
    MatrixXd tableau;
    bool foundSolution;
    double optimum;
    VectorXd solution;
    int64_t numberOfVariables;

    int64_t findPivot_min(int64_t column);
    bool simplexAlgorithm(int64_t variableNum);
    int64_t getPivotRow(int64_t column);

protected:

public:
    SimplexSolver(int mode, const VectorXd &objectiveFunction, const MatrixXd &constraints);
    bool hasSolution();
    double getOptimum();
    VectorXd getSolution();
};
