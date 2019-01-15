import math
from scipy.optimize import linprog
from simplex import *
import sys
import numpy as np


def lpreader(path):
    f = open(path)
    avr_num = int(f.readline())
    l = np.array([int(s) for s in f.readline().split()]).reshape(-1, 2)
    c = [0]*avr_num
    for row in l:
        c[row[1]] = row[0]
    
    bound_num = int(f.readline())
    bound = [[0, None]]*avr_num
    for _ in range(bound_num):
        l = [int(s) for s in f.readline().split()]
        l[1] = None if l[1]==2147483647 or l[1]==-2147483648 else l[1]
        l[2] = None if l[2]==2147483647 or l[2]==-2147483648 else l[2]
        bound[l[0]] = [l[1], l[2]]

    eq_num = int(f.readline())
    A_eq, b_eq = [], []
    for _ in range(eq_num):
        l = [int(s) for s in f.readline().split()]
        A_eq.append(l[0:-1])
        b_eq.append(l[-1])
    if A_eq==[] and b_eq==[]:
        A_eq, b_eq = None, None
    
    ub_num = int(f.readline())
    A_ub, b_ub = [], []
    for _ in range(ub_num):
        l = [int(s) for s in f.readline().split()]
        A_ub.append(l[0:-1])
        b_ub.append(l[-1])
    if A_ub==[] and b_ub == []:
        A_ub, b_ub = None, None
    
    f.close()
    return c, A_ub, b_ub, A_eq, b_eq, bound


class IPsolver:
    def __init__(self, c, Aub, bub, Aeq, beq, bounds, tol=1.0E-8):
        self.c = np.array(c)
        self.Aub = np.array(Aub) if Aub else None
        self.bub = np.array(bub) if bub else None
        self.Aeq = np.array(Aeq) if Aeq else None
        self.beq = np.array(beq) if beq else None
        self.bounds = np.array(bounds) if bounds else None
        self.tol = tol

        self.solution = np.zeros(self.c.shape)
        self.optimum = -np.inf
        self.isFoundSolution = False

        self.cur_sol = np.zeros(self.c.shape)
        self.cur_opt = -np.inf

        self.max_branch_num = 5
        self.cur_branch_num = 0

    def allInteger(self, sol):
        tmp = np.array([abs(x-np.round(x)) for x in list(sol)])
        return all(tmp <= self.tol)

    def getFirstNotInt(self, sol):
        tmp = np.array([abs(x-np.round(x)) for x in list(sol)])
        l = tmp > self.tol
        for i in range(len(l)):
            if l[i]: return i
        return -1

    def needBranch(self, sol, opt):
        if self.allInteger(sol): return False
        elif opt <= self.cur_opt: return False
        return True

    def update_opt(self, sol, opt):
        if self.allInteger(sol) and self.cur_opt<opt:
            self.cur_opt = opt
            self.cur_sol = sol.copy()
            self.isFoundSolution = True

    def core_solve(self, c, Aub, bub, Aeq, beq, bounds):
        sol, opt = None, None
        res = linprog(-c, A_ub=Aub, b_ub=bub, A_eq=Aeq, b_eq=beq, bounds=bounds) # for min
        # if Aub is not None:
        #     Aub_ = Aub.tolist()
        #     bub_ = bub.tolist()
        # else: 
        #     Aub_, bub_ = [],[]
        
        # if Aeq is not None:
        #     Aeq_ = Aeq.tolist()
        #     beq_ = beq.tolist()
        # else: 
        #     Aeq_, beq_ = [],[]
        
        # if bounds is not None:
        #     bounds_ = bounds.tolist()
        # else:
        #     bounds_ = []
        # c_ = c.tolist()
        # res = my_simplex_solver(c_, Aub_, bub_, Aeq_, beq_, bounds_)
        if res.success:
            opt = -res.fun # for min
            # opt = res.fun
            sol = res.x
            print(opt, sol)
            print(self.allInteger(sol))
            self.update_opt(sol, opt)
            if self.needBranch(sol, opt) and self.cur_branch_num < self.max_branch_num:
                index = self.getFirstNotInt(sol)
                # print("index =", index)
                to_round = sol[index]
                len_c = len(self.c)
                Con1 = np.zeros((len_c, ))
                Con2 = np.zeros((len_c, ))
                Con1[index] = -1.0
                Con2[index] = 1.0
                if Aub is None and bub is None:
                    A1 = Con1.reshape(1, len_c)
                    A2 = Con2.reshape(1, len_c)
                    B1 = np.array([-math.ceil(to_round)])
                    B2 = np.array([math.floor(to_round)])
                else:
                    A1 = np.vstack([Aub, Con1])
                    A2 = np.vstack([Aub, Con2])
                    B1 = np.hstack([bub, -math.ceil(to_round)])
                    B2 = np.hstack([bub, math.floor(to_round)])

                self.cur_branch_num += 1
                self.core_solve(c, A1, B1, Aeq, beq, bounds) # right branch
                self.core_solve(c, A2, B2, Aeq, beq, bounds) # left branch

    def solve(self):
        self.core_solve(self.c, self.Aub, self.bub, self.Aeq, self.beq, self.bounds)
        if self.isFoundSolution:
            self.solution = np.array([int(np.round(x)) for x in list(self.cur_sol)])
            self.optimum  = self.cur_opt
            return True
        else:
            return False


def test():
    c, A_ub, b_ub, A_eq, b_eq, bound = lpreader(sys.argv[1])
    # print(c, A_ub, b_ub, A_eq, b_eq, bound)
    c = [-x for x in c]
    res = linprog(c, A_ub=A_ub, b_ub=b_ub, A_eq=A_eq, b_eq=b_eq, bounds=bound)
    print(res)


def test1():
    c, A_ub, b_ub, A_eq, b_eq, bound = lpreader(sys.argv[1])
    res = my_simplex_solver(c, A_ub, b_ub, A_eq, b_eq, bound)
    print(res.x, res.fun, res.success)


def test2():
    c, A_ub, b_ub, A_eq, b_eq, bound = lpreader(sys.argv[1])
    solver = IPsolver(c, A_ub, b_ub, A_eq, b_eq, bound)
    solver.solve()
    print(solver.solution, solver.optimum)
    np.savetxt(sys.argv[2], solver.solution, fmt='%d')
    f = open(sys.argv[2], "a")
    f.write('\noptimum:'+'\n')
    f.write(str(int(np.round(solver.optimum)))+'\n')
    f.close()


if __name__=="__main__":
    # test()
    # test1()
    test2()
