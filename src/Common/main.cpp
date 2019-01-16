/*
Copyright 2012-2018 Tamas Bolner

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <cinttypes>
#include <iostream>
// #include "bits/stdc++.h"
#include <Eigen/Dense>
#include "lpreader.h"
#include "SimplexSolver.h"
#include "../BranchBound/branch_bound.h"
#include "exception.h"

using namespace std;
using namespace Eigen;

int main(int argc,char* argv[])
{
	//string s(argv[1]);
	string s;
	char c;
	FILE* fp=fopen(argv[1],"r");
	while(!feof(fp)){
		c=fgetc(fp);
		if(c<=0)
			continue;
		s+=c;
	}
	fclose(fp);
	//cin>>s;
	//MatrixXd constraints(equ_m, x_m+1);
	//VectorXd objectiveFunction(x_m);
	
	try {
		LPReader lpr(s);
		/*
			Maximization problem
		*/
		VectorXd c(7);
		c << -3, 4, 0, 0, 0, 0, 0;
		MatrixXd Ab(5,8);
		Ab<<4,2,-1,0,0,0,0,8,
			3,2,0,1,0,0,0,10,
			-1,3,0,0,1,0,0,1,
			1,0,0,0,0,-1,0,1,
			1,0,0,0,0,0,-1,1;
		
		SimplexSolver solver1(SIMPLEX_MAXIMIZE, c, Ab);
		// cout<<"c"<<endl; cout<<lpr.c.transpose()<<endl;
		// cout<<"Ab"<<endl; cout<<lpr.Ab<<endl;

		// BranchBound bbsolver(SIMPLEX_MAXIMIZE, lpr.c, lpr.Ab, 1E-8);
		// bbsolver.solve();
		// if(bbsolver.foundSolution){
		// 	cout<<"success"<<endl;
		// 	cout<<bbsolver.optimum<<endl;
		// 	cout<<bbsolver.solution.transpose()<<endl;
		// }else{
		// 	cout<<"failed"<<endl;
		// }

		if (solver1.hasSolution()) {
			cout << "The maximum is: " << solver1.getOptimum() << endl;
			cout << "The solution is: " << solver1.getSolution().transpose() << endl;
		} else {
			cout << "The linear problem has no solution." << endl;
		}

		cout << endl;
	} catch (const FException &ex) {
		ex.Print();
		return 1;
	}
	
	return 0;
}