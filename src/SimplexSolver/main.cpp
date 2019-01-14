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
#include "bits/stdc++.h"
#include <Eigen/Dense>
#include "SimplexSolver.h"
#include "exception.h"
#include "../BranchBound/branch_bound.h"

#define pb push_back
#define eps 1e-9

using namespace std;
using namespace Eigen;
struct node{
	vector<vector<double> >vv;
	node(int m=0,int n=0){//m row //n col
		vv.resize(m);
		for(int i=0;i<m;i++)
			vv[i].resize(n);
		return ;
	}
	struct node operator*(const struct node &rhs)const{
		assert(vv.size()&&rhs.vv.size()&&vv[0].size()==rhs.vv.size());
		unsigned int i,j,k;
		node ret(vv.size(),rhs.vv[0].size());
		for(i=0;i<vv.size();i++)
			for(j=0;j<rhs.vv.size();j++)
				for(k=0;k<rhs.vv[j].size();k++)
					ret.vv[i][k]+=vv[i][j]*rhs.vv[j][k];
		return ret;
	}
	bool operator <=(const int x)const{
		unsigned int i,j;
		for(i=0;i<vv.size();i++)
			for(j=0;j<vv[i].size();j++)
				if(vv[i][j]<=eps)
					return true;
		return false;
	}
	bool operator <(const int x)const{
		unsigned int i,j;
		for(i=0;i<vv.size();i++)
			for(j=0;j<vv[i].size();j++)
				if(vv[i][j]<0)
					return true;
		return false;
	}
	bool operator >(const int x)const{
		unsigned int i,j;
		for(i=0;i<vv.size();i++)
			for(j=0;j<vv[i].size();j++)
				if(vv[i][j]>0)
					return true;
		return false;
	}
	void append(vector<double> &obj){
		unsigned int i,j;
		if(vv.size()&&vv[0].size()<obj.size()){
			for(i=0;i<vv.size();i++)
				for(j=vv[i].size();j<obj.size();j++)
					vv[i].pb(0);
		}
		vector<double> tmp(obj);
		vv.push_back(tmp);
		if(obj.size()<vv[0].size()){
			i=vv.size()-1;
			for(j=obj.size();j<vv[0].size();j++)
				vv[i].pb(0);
		}
		return ;
	}
	void append(double obj){
		unsigned int i,j;
		vector<double> tmp(1);
        tmp[0]=obj;
		if(vv.size()&&vv[0].size()<tmp.size()){
			for(i=0;i<vv.size();i++)
				for(j=vv[i].size();j<tmp.size();j++)
					vv[i].pb(0);
		}
		vv.push_back(tmp);
		if(tmp.size()<vv[0].size()){
			i=vv.size()-1;
			for(j=tmp.size();j<vv[0].size();j++)
				vv[i].pb(0);
		}
		return ;
	}
	void rowmul(int ii,double kk){//ii-th row *=kk
		int i;
		for(i=0;i<vv[ii].size();i++)
			vv[ii][i]*=kk;
		return ;
	}
	void rowaddmul(int ii,vector<double> &jj,double kk){//ii-th row add kk* jj
		int i;
		for(i=0;i<vv[ii].size();i++)
			vv[ii][i]+=kk*jj[i];
		return ;
	}
	void rowaddmul(int ii,int jj,double kk){//ii-th row add kk* jj-th row
		int i;
		for(i=0;i<vv[ii].size();i++)
			vv[ii][i]+=kk*(vv[jj][i]);
		return ;
	}
	struct node T(){
		assert(vv.size());
		node ret(vv[0].size(),vv.size());
		unsigned int i,j;
		for(i=0;i<vv.size();i++)
			for(j=0;j<vv[i].size();j++)
				ret.vv[j][i]=vv[i][j];
		return ret;
	}
	void print(){
		unsigned int i,j;
		printf("%d %d:\n",(int)vv.size(),(int)(vv.empty()?0:vv[0].size()));
		for(i=0;i<vv.size();i++){
			for(j=0;j<vv[i].size();j++)
				printf("%7f ",vv[i][j]);
			putchar('\n');
		}
		return ;
	}
};
int fgetint(FILE* &fp){
	bool isminus=false;
	char c;
	int v;
	while((c=fgetc(fp))!=EOF&&isdigit(c)==0&&c!='-');
	if(c=='-')
		isminus=true,v=0;
	else
		v=c-'0';
	while((c=fgetc(fp))!=EOF&&isdigit(c))
		v=(v<<3)+(v<<1)+c-'0';
	return isminus?-v:v;
}
void transgte(vector<double>,double,int &,struct node &,struct node &);
void transequ(vector<double>,double,int &,struct node &,struct node &);
void translte(vector<double>,double,int &,struct node &,struct node &);
void transgte(vector<double>equ,double b,int &freex,struct node &A,struct node &B){
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
void transequ(vector<double>equ,double b,int &freex,struct node &A,struct node &B){
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
void translte(vector<double>equ,double b,int &freex,struct node &A,struct node &B){
    if(b>=0){
        A.append(equ);
        B.append(b);
        return ;
    }
    for(int i=0;i<equ.size();i++)
        equ[i]=-equ[i];
    transgte(equ,-b,freex,A,B);
}
int main(int argc,char* argv[])
{
    FILE *fp=fopen(argv[1],"r");
	assert(fp);
	vector<double>tempv(0);
	int x_m,bnd_m,equ_m=0,i,j,idx,l,r,freex;
	fscanf(fp,"%d%d",&x_m,&bnd_m);
	freex=x_m;
	struct node A,B;
	struct node C(x_m,1);
	for(i=0;i<x_m;i++)
		C.vv[i][0]=fgetint(fp);
	for(i=0;i<bnd_m;i++){
		fscanf(fp,"%d%d%d",&idx,&l,&r);
		idx--;
        if(l>0){
			tempv.resize(0);
			for(j=0;j<idx;j++)
				tempv.pb(0);
			tempv.pb(1);//l<=x
			for(j++;j<freex;j++)
				tempv.pb(0);
            transgte(tempv,l,freex,A,B);
        }
        if(r<INT_MAX){
			tempv.resize(0);
			for(j=0;j<idx;j++)
				tempv.pb(0);
			tempv.pb(1);//x<=r
			for(j++;j<freex;j++)
				tempv.pb(0);
            translte(tempv,r,freex,A,B);
        }
	}
    int equnum=fgetint(fp);
    for(i=0;i<equnum;i++){
        tempv.resize(0);
		for(j=0;j<x_m;j++)
			tempv.pb(fgetint(fp));
        transequ(tempv,fgetint(fp),freex,A,B);
    }
    int ltenum=fgetint(fp);
    for(i=0;i<ltenum;i++){
        tempv.resize(0);
		for(j=0;j<x_m;j++)
			tempv.pb(fgetint(fp));
        translte(tempv,fgetint(fp),freex,A,B);
    }
	//A.print();
	//B.print();
    assert(A.vv.size());
    equ_m=A.vv.size();
    x_m=A.vv[0].size();
	for(i=0;i<freex;i++)
		C.append(0);
    //C.vv[0][0]=1;

    
    MatrixXd constraints(equ_m, x_m+1);
    VectorXd objectiveFunction(x_m);
    
    try {
        /*
            Maximization problem
        */
        for(j=0;j<x_m;j++)
           objectiveFunction(j)=C.vv[0][j];
        for(i=0;i<equ_m;i++)
            for(j=0;j<x_m;j++)
                constraints(i,j)=A.vv[i][j];
        for(i=0;i<equ_m;i++)
            constraints(i,x_m)=B.vv[i][0];
        
        SimplexSolver solver1(SIMPLEX_MAXIMIZE, objectiveFunction, constraints);

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
