#include <iostream>
#include "branch_bound.h"
#include "../SimplexSolver/SimplexSolver.h"
using namespace Eigen;
using namespace std;

bool BranchBound::solve()
{
    VectorXd tmp_c(c);
    VectorXd sol;
    double   opt;
    if(mode==SIMPLEX_MINIMIZE){
        tmp_c = c * -1.0;
    }
    
    core_solve(tmp_c, Ab, sol, opt);

    if(foundSolution){
        optimum  = (mode==SIMPLEX_MINIMIZE) ? -current_opt : current_opt;
        solution = current_solution;
        return true;
    }
    else return false;
}

void BranchBound::core_solve(const VectorXd& c, 
                             const MatrixXd& Ab, 
                             VectorXd& sol, 
                             double& opt)
{
    SimplexSolver ssolver(SIMPLEX_MAXIMIZE, c, Ab);
    if(ssolver.hasSolution()){
        opt = ssolver.getOptimum();
        cout<<"DEBUG : current opt = "<<opt<<endl;
        sol = ssolver.getSolution();
        update_opt(sol, opt);
        if(needBranch(sol, opt)){
            int64_t index = getFirstNotInt(sol);
            double to_round = sol(index);
            MatrixXd left_Ab,  right_Ab;
            VectorXd left_sol, right_sol;
            double   left_opt, right_opt;
            branch(index, to_round, Ab, left_Ab,  true);
            branch(index, to_round, Ab, right_Ab, false);
            
            core_solve(c, left_Ab, left_sol, left_opt);
            core_solve(c, right_Ab, right_sol, right_opt);
        }
    }
}
