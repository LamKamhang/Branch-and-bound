#include <cinttypes>
#include <iostream>
#include <time.h>
#include <Eigen/Dense>
#include "Common/lpreader.h"
#include "SimplexSolver/SimplexSolver.h"
#include "SimplexSolver/exception.h"
#include "BranchBound/branch_bound.h"

using namespace std;
using namespace Eigen;

int main(int argc,char* argv[])
{
	if(argc<=2){printf("arg: input.txt output.txt\n"), exit(-1);}
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
		// VectorXd c(7);
		// c << -3, 4, 0, 0, 0, 0, 0;
		// MatrixXd Ab(5,8);
		// Ab<<4,2,-1,0,0,0,0,8,
		// 	3,2,0,1,0,0,0,10,
		// 	-1,3,0,0,1,0,0,1,
		// 	1,0,0,0,0,-1,0,1,
		// 	1,0,0,0,0,0,-1,1;
		
		// SimplexSolver solver1(SIMPLEX_MAXIMIZE, c, Ab);
		// cout<<"c"<<endl; cout<<lpr.c.transpose()<<endl;
		// cout<<"Ab"<<endl; cout<<lpr.Ab<<endl;
		
		BranchBound bbsolver(SIMPLEX_MAXIMIZE, lpr.c, lpr.Ab, 1E-8);
		clock_t start,end;
		start = clock();
		bbsolver.solve();
		end = clock();
		if(bbsolver.foundSolution){
			printf("result:\n");
			printf("solution:\n");
			cout<<bbsolver.solution.transpose()<<endl;
			printf("optimum:\n");
			cout<<bbsolver.optimum<<endl;
		}else{
			cout<<"failed"<<endl;
		}
		double dur = (double)(end - start);
		printf("used %.2e seconds\n", (dur/CLOCKS_PER_SEC));
		
		ofstream outf(argv[2]);
		outf<<bbsolver.solution<<endl;
		outf<<endl;
		outf<<"\noptimum:"<<endl;
		outf<<bbsolver.optimum<<endl;
		outf.close();
		printf("saved to %s", argv[2]);

		// if (solver1.hasSolution()) {
		// 	cout << "The maximum is: " << solver1.getOptimum() << endl;
		// 	cout << "The solution is: " << solver1.getSolution().transpose() << endl;
		// } else {
		// 	cout << "The linear problem has no solution." << endl;
		// }
	} catch (const FException &ex) {
		ex.Print();
		return 1;
	}
	
	return 0;
}
