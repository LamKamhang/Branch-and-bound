import math
from scipy.optimize import linprog
import sys
import numpy as np


def lpreader(path):
    f = open(path)
    avr_num = int(f.readline())
    bound_num = int(f.readline())
    bound = [[0, None]]*avr_num
    for _ in range(bound_num):
        l = [int(s) for s in f.readline().split()]
        l[1] = None if l[1]==2147483647 or l[1]==-2147483647 else l[1]
        l[2] = None if l[2]==2147483647 or l[2]==-2147483647 else l[2]
        bound[l[0]-1] = [l[1], l[2]]
    
    eq_num = int(f.readline())
    A_eq, b_eq = [], []
    for _ in range(eq_num):
        l = [int(s) for s in f.readline().split()]
        A_eq.append(l[0:-1])
        b_eq.append(l[-1])
    
    ub_num = int(f.readline())
    A_ub, b_ub = [], []
    for _ in range(ub_num):
        l = [int(s) for s in f.readline().split()]
        A_ub.append(l[0:-1])
        b_ub.append(l[-1])
    f.close()
    return avr_num, A_ub, b_ub, A_eq, b_eq, bound


def integerPro(c, Aub, bub, Aeq, beq, bounds, t=1.0E-3, root=True, result=None):
    c = np.array(c)
    A = np.array(Aub)
    b = np.array(bub)
    Aeq = np.array(Aeq)
    beq = np.array(beq)
    len_c = len(c)
    if not root:
        res = result
    else:
        res = linprog(c, A_ub=A, b_ub=b, A_eq=Aeq, b_eq=beq, bounds=bounds)
    bestX = res.x
    if bestX is np.nan:
        return (np.inf, np.full((len_c,), np.nan))
    bestVal = c.dot(bestX)
    print("bestVal", bestVal)
    if all(((x-math.floor(x))<t or (math.ceil(x)-x)<t) for x in bestX):
        return (bestVal, bestX)
    else:
        ind = [i for i, x in enumerate(bestX) if (x-math.floor(x))>t and (math.ceil(x)-x)>t][0]
        newCon1 = np.zeros((len_c, ))
        newCon2 = np.zeros((len_c, ))
        newCon1[ind] = -1.0
        newCon2[ind] = 1.0
        newA1 = np.vstack([A, newCon1])
        newA2 = np.vstack([A, newCon2])
        newB1 = np.hstack([b, -math.ceil(bestX[ind])])
        newB2 = np.hstack([b, math.floor(bestX[ind])])
        
        r1 = integerPro(c, newA1, newB1, Aeq, beq, bounds)
        r2 = integerPro(c, newA2, newB2, Aeq, beq, bounds)
        if r1[0] < r2[0]:
            return r1
        else:
            return r2


def test():
    num, A_ub, b_ub, A_eq, b_eq, bound = lpreader("1.txt")
    c = [0]*num
    c[num-1] = -1
    res = linprog(c, A_ub=A_ub, b_ub=b_ub, A_eq=A_eq, b_eq=b_eq, bounds=bound)
    print(res.x, res.fun)


def test1():
    num, A_ub, b_ub, A_eq, b_eq, bound = lpreader("5.txt")
    c = [0]*num
    c[num-1] = -1
    res = integerPro(c, A_ub,b_ub,A_eq,b_eq,bound)
    print(res)


if __name__=="__main__":
    test()