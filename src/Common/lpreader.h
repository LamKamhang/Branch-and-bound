#include "sparse.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <Eigen/Dense>

#define pb push_back
using namespace std;
using namespace Eigen;

class LPReader{
public:
	VectorXd c;
	MatrixXd Ab;
	Data d;

	//void transgte(vector<int>,int,int &,vector<vector<int> >&,vector<int>&);
	//void transequ(vector<int>,int,int &,vector<vector<int> >&,vector<int>&);
	//void translte(vector<int>,int,int &,vector<vector<int> >&,vector<int>&);
	void transgte(vector<int>equ,int b,int &freex,vector<vector<int> >&A,vector<int>&B){
		if(b>0){
			equ.pb(-1);
			freex++;
			transequ(equ,b,freex,A,B);
			return ;
		}
		for(int i=0;i<equ.size();i++)
			equ[i]=-equ[i];
		translte(equ,-b,freex,A,B);
		return ;
	}
	void transequ(vector<int>equ,int b,int &freex,vector<vector<int> >&A,vector<int>&B){
		if(b>=0){
			equ.pb(1);
			freex++;
			translte(equ,b,freex,A,B);
			equ[equ.size()-1]=-1;
			translte(equ,b,freex,A,B);
			return ;
		}
		for(int i=0;i<equ.size();i++)
			equ[i]=-equ[i];
		transequ(equ,-b,freex,A,B);
		return ;
	}
	void translte(vector<int>equ,int b,int &freex,vector<vector<int> >&A,vector<int>&B){
		int i,j;
		if(b>=0){
			for(i=0;i<A.size();i++)
				for(j=A[i].size();j<equ.size();j++)
					A[i].pb(0);
			A.pb(equ);
			B.pb(b);
			return ;
		}
		for(i=0;i<equ.size();i++)
			equ[i]=-equ[i];
		transgte(equ,-b,freex,A,B);
	}
    void print(vector<vector<int> >&A,vector<int>&B){
        int i,j;
		for(i=0;i<A.size();i++){
			for(j=0;j<A[i].size();j++){
                printf("%3d ",A[i][j]);
            }
            printf("%3d\n",B[i]);
		}
        return ;
    }
	LPReader(const string& file){
        puts("succeed in reading");
		d.Parse(file);
		unsigned int i,j;
		int freex=d.indices.size();
		
		vector<int>tempv;
		vector<int>B;
		vector<vector<int> >A;/*
		for(i=0;i<var_num;i++){
			c(d.function[i].index)=d.function[i].coefficient;
		}*/
		for(i=0;i<d.bounds.size();i++){
			if(d.bounds[i].second.lower>0){
				tempv.resize(freex);
				for(j=0;j<freex;j++)
					tempv[j]=0;
				tempv[d.bounds[i].first-1]=1;//l<=x  x>=l
				transgte(tempv,d.bounds[i].second.lower,freex,A,B);
			}
			if(d.bounds[i].second.upper<INT_MAX){
				tempv.resize(freex);
				for(j=0;j<freex;j++)
					tempv[j]=0;
				tempv[d.bounds[i].first-1]=1;//x<=r
				translte(tempv,d.bounds[i].second.upper,freex,A,B);
			}
            //print(A,B);
		}
		for(i=0;i<d.conditions.size();i++){
			tempv.resize(freex);
			for(j=0;j<freex;j++)
				tempv[j]=0;
			for(j=0;j<d.conditions[i].variables.size();j++)
				tempv[d.conditions[i].variables[j].index-1]=d.conditions[i].variables[j].coefficient;
			switch(d.conditions[i].type){
				case Condition::Type::eq:transequ(tempv,d.conditions[i].constant,freex,A,B);break;
				case Condition::Type::leq:translte(tempv,d.conditions[i].constant,freex,A,B);break;
				case Condition::Type::geq:transgte(tempv,d.conditions[i].constant,freex,A,B);break;
			}
		}
		c = VectorXd::Zero(freex);
		for(i=0;i<d.function.size();i++)
			c(d.function[i].index)=d.function[i].coefficient;
        //for(i=0;i<freex;i++)
        //    printf("%02d ",c(i));
        //putchar('\n');
		assert(A.size());
		for(i=1;i<A.size();i++)
			assert(A[i-1].size()==A[i].size());
		Ab.resize(A.size(),A[0].size()+1);
		for(i=0;i<A.size();i++){
			for(j=0;j<A[i].size();j++){
				Ab(i,j)=A[i][j];
                //printf("%02d ",A[i][j]);
            }
			Ab(i,j)=B[i];
            //printf("%02d\n",B[i]);
		}
        puts("succeed in constructing");
		return ;
	}

};