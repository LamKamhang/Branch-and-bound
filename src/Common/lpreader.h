#include "sparse.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;

class LPReader{
public:
    VectorXd c;
    MatrixXd Ab;
    Data d;

    LPReader(const string& file){
        d.Parse(file);
        int64_t var_num = d.function.size();
        c = VectorXd::Zero(var_num);
        for(int64_t i=0;i<var_num;i++){
            c(d.function[i].index)=d.function[i].coefficient;
        }
        
    }

};