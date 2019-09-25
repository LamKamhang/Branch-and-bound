#include <Eigen/Dense>

#define INF 1E100

using namespace Eigen;
using namespace std;

class BranchBound{
public:
    VectorXd solution;
    bool foundSolution;
    double optimum;
    int64_t numberOfVariables;

    BranchBound(int mode, const VectorXd &objectiveFunction, 
                          const MatrixXd &constraints, double tol=1E-8)
    : mode(mode), c(objectiveFunction), Ab(constraints), tol(tol) {
        numberOfVariables = objectiveFunction.rows();
        current_opt = optimum = -INF;
        solution.resize(numberOfVariables);
        current_solution.resize(numberOfVariables);
        foundSolution = false;
    }
    
    bool solve();

private:
    int mode;
    VectorXd c;
    MatrixXd Ab;
    double current_opt;
    VectorXd current_solution;
    double tol;

    bool allInteger(const VectorXd &solution){
        for(int64_t i=0;i<numberOfVariables;i++){
            if(abs(solution(i) - int(solution(i))) > tol)return false;
        }
        return true;
    }

    int64_t getFirstNotInt(const VectorXd &solution){
        for(int64_t i=0;i<numberOfVariables;i++){
            if(abs(solution(i) - int(solution(i))) > tol)return i;
        }
        return -1;
    }

    bool needBranch(const VectorXd &solution, double value){
        bool ai = allInteger(solution);
        if(ai)return false;
        if(value <= current_opt)return false;
        return true; 
    }

    void update_opt(const VectorXd &solution, double value){
        bool ai = allInteger(solution);
        if(ai && current_opt < value){
            current_opt = value;
            current_solution = solution;
        }
        foundSolution = true;
    }

    void branch(int64_t index,
                double to_round, 
                const MatrixXd& Ab, 
                MatrixXd& new_Ab,
                bool is_left)
    {
        new_Ab.resizeLike(Ab);
        new_Ab.conservativeResize(Ab.rows()+1, Ab.cols());
        
        VectorXd to_append;
        to_append.resize(Ab.cols());
        for(int64_t i=0;i<Ab.cols();i++){to_append(i) = 0;}
        to_append(index) = is_left ? 1 : -1;
        to_append(Ab.cols()-1) = is_left ? int(to_round) : (int(to_round) + 1);
        
        new_Ab.row(Ab.rows()) = to_append;
    }

    void core_solve(const VectorXd& c, const MatrixXd& Ab, VectorXd& sol, double& opt);
};
