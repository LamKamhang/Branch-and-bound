# 应用运筹学基础

## 分枝定界法求解整数规划

### 背景介绍

有很多规划问题，他们的决策变量都是连续的，并且约束和目标函数都是线性的，这种规划我们称为线性规划。线性规划是相对比较容易求解的，但是有许多实际问题，譬如人员、机器或者车辆的分配，它们是不可分割的整体，决策变量只有在它们具有整数值时才有意义。在线性规划中，增加决策变量的整数限制，这种我们称为线性整数规划，一般情况下，我们会简称为整数规划。

整数规划有很多求解方法，例如割平面发和分枝定界法。它们都是先将问题转化为线性规划求解，然后增加整数约束进行约化，直到最后找到整数解。而这两种方法都可以求纯或混合整数线性规划问题。我们组采用的是分枝定界的方法来求解整数规划。

在分支定界法中，我们将整数规划转换为线性规划后，是利用单纯形法进行求解。一言概之，我们组的大作业是利用单纯形法+分支定界发求解整数规划问题。

整数规划问题是NP困难问题，特别的，$0-1$规划是整数规划的特殊情况，它的决策变量要么取$0$要么取$1$， 这是Karp的21个NP完全问题之一。

### 算法描述

#### 单纯形法

单纯形是$N$维中的$N+1​$个顶点的凸包，是一个多胞体，譬如是直线上的一个线段，平面上的一个三角形，三维空间中的一个四面体等等，这些都是单纯形。

![1547307775376](..\img\simplex.png)

##### 标准形式

在使用单纯形法之前，我们需要将线性规划转换为以下标准形式：
$$
\begin{matrix}
\max&: & \sum\limits_{1 \le k \le n}c_{k}x_{k}\\
s.t.&: & Ax \le b \\
    &  & x \ge 0
\end{matrix}
$$
所有其他形式的线性规划方程组都可以转化称这个标准形式：

- 目标函数不是极大化：只需要将$c_k$取为原来的相反数，就可以从极小化问题转化为极大化问题。
- 约束条件中存在大于或等于约束：只需要将约束两边同乘$-1$。
- 约束条件中存在等式：只需要将其转化为两个不等式，一个为大于等于，另一个为小于等于。
- 有的变量约束为大于等于$R$：只需要做简单的仿射变换，用$x' = x - R$代替原来的变量即可。
- 有的变量约束小于等于$0$：只需要将与该变量有关的所有系数取相反数即可。
- 有的变量没有非负约束：加入新变量$x'$，并用$x - x'$替换原来的变量$x$。

通过以上总总，我们就可以将一个一般的线性规划转换为标准形式。

##### 松弛形式

在使用单纯形法进行变换之前，我们需要先计算出一个可行解。我们可以通过将标准形式的线性规划转化为松弛形式，这样能够快速得到线性规划的初始可行解。只需要在原来$n$个变量，$m$个约束的线性规划中，加入$m$个新变量，就可以将原来的不等式化为等式：
$$
\forall j \in \{1, 2, \dots, m\}, \\
\sum\limits_{1 \le k \le n} a_{j, k}x_{k} + x_{n+j} = b_{j}, \\
x_{n+j} \ge 0
$$
我们可以首先通过新加入的变量快速得到一组初始可行解：
$$
x_{n+j} = b_{j} - \sum\limits_{1 \le k \le n} a_{j,k}x_{k}
$$
我们现在称$x_{1}, x_{2}, \dots, x_{n}$这些变量为**非基变量**，而称$x_{n+1}, x_{n+2}, \dots, x_{n+m}$这些变量为**基变量**。非基变量能够由基变量唯一确定，也就是课上老师所说的**典则形式**。

我们通过两阶段法求得原标准形式的初始可行解：

1. 第一阶段的目标函数为$\min: \sum\limits_{n+1 \le k \le n+m}x_{n+k}$， 如果得到该目标函数值为$0$，则通过转轴变换将基变量全部转换为原来的变量。如果目标函数值非$0$，则表明原规划问题无解。
2. 第一阶段结束以后，以第一阶段得到的可行解进行求解原始问题。

**单纯形表**则是将松弛形式（或者标准形式）的规划问题中的系数放入一个增广矩阵中，通过矩阵变换求得最终的最优解和最优值。

##### 转轴变换

转轴变换是单纯形法中的核心操作，作用就是将一个基变量与一个非基变量进行互换。从几何的理解上就是从单纯形的一个极点走向另一个极点。设变量$x_{n+d}$是基变量，变量$x_{e}$是非基变量，那么转轴操作**pivot(d, e)**以后，$x_{n+d}$将变为非基变量，相应的$x_{e}$变为基变量。将这些转化为用数学符号描述则如下：
$$
\begin{matrix}
\text{起初}& : & x_{n+d} = b_{d} - \sum\limits_{k \in N}a_{d, k}x_{k} \\
\text{移项}& : & a_{d, e}x_{e} = b_{d} - \sum\limits_{k \in N \and k \ne e} a_{d, k}x_{k} - x_{n+d} \\
\text{若}a_{d, e} \ne 0 & : & x_{e} = \dfrac{b_{d}}{a_{d, e}} - \sum\limits_{k \in N \and k \ne e}\dfrac{a_{d, k}}{a_{d, e}}x_{k} - \dfrac{1}{a_{d, e}}x_{n+d}
\end{matrix}
$$
将这个式子代入其他的约束等式以及目标函数中，就实现了$x_{n+d}$和$x_{e}$的基变量与非基变量的转换。

这在增广矩阵中的操作则对应为第$i$行的基变量变为第$j$个变量，然后利用消元法将其他行中第$j$列的系数消去。我们称这个操作为转轴变换。

##### 最优化过程

而我们挑选哪一个非基变量与基变量进行转轴变换则是最优化过程了，这个过程如下：

1. 得到原规划问题的初始可行解（两阶段法）。

2. 任取一个非基变量$x_{e}$，使得$c_{e} \gt 0$

3. 考虑基变量$x_{d}$，$\min\limits_{a_{d, e} \gt 0} \dfrac{b_{d}}{a_{d, e}}$ 

4. 交换$x_{e}和x_{d}$， 即转轴变换**pivot(d, e)** 

5. 如果所有的非基变量的系数都是小于等于$0$时，我们已经得到最优解了。将基变量及其增广列对应值作为输出即可。

   如果只剩$c{e} \gt 0$且$\forall i \in \{1, 2, \dots, m\}, a_{d, e} \le 0$ 则原规划问题没有有限最有解，目标函数值为正无穷。

##### Bland法则

而我们选取非基变量入基的时候，不能够每次都选择检验数最大的入基，这样会导致单纯形法退化，进入搜索循环的bug。根据**Bland法则**，我们可以每次选择下标最小的非基变量入基，就可以避免单纯形法退化。

##### 实现过程

- 数据结构
- 函数方法

#### 分枝定界法

分枝定界法不只是解决整数规划的一种方法，它其实可以认为是一种组合优化问题以及数学优化算法设计的范式。 分枝定界法由通过状态空间搜索的候选解决方案的系统枚举组成：候选解决方案集被认为是在根处形成具有全集的根树。 该算法探索此树的分支，它代表解决方案集的子集。 在枚举分支的候选解之前，针对最优解的**上下估计边界检查分支**，并且如果它不能产生比迄今为止由算法找到的最佳解决方案更好的解，则丢弃该分支（**称为剪枝**）。

在整数规划问题中，我们先将原问题放松成线性规划问题，解这个线性规划，就得到了整数规划最优解的上界。这是因为减少了约束，得到的目标函数值自然更大，所以是上界。然后我们检查最优解，如果最优解中有非整数变量，记为$x_{i}$， $N \lt x_{i} \lt N+1$，这时候就会有两种可能：$x_{i} \le N$或者$x_{i} \ge N+1$。这时候我们分枝，一枝增加约束$x_{i} \le N$，另一枝增加约束$x_{i} \ge N+1$。然后递归进行搜索。如果中间过程得到的线性规划最优解也是整数规划最优解，就记其为下界。如果某一枝的上界比下界还小，则将这一枝剪去，称为剪枝，这一枝称为死枝。直到最后找到最优解。中间过程中需要反复降为线性规划以单纯形法进行求解。

这里我们分枝定界法是需要维护两个界的，一个是上界，一个是下界：

- 上界初始化为没有增加约束的原问题的线性规划最优解
  - 更新则在于从一个节点分成两个节点后，取两个节点中线性规划的最优解的最大值。
- 下界初始化为负无穷
  - 更新则在于每次求解出一个线性规划也正好为整数规划且比已知的下界大时，更新下界。
- 如果计算得到的线性规划最优解比已知的下界小，则进行剪枝。
- 如此计算，上界会不断减小，下界会不断提高，直到上界等于下界。

![1547309808570](..\img\BB.png)

> 图引自MIT http://web.mit.edu/15.053/www/AMP-Chapter-09.pdf

##### 实现过程

```python
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
```

定义如上所示整数规划求解器类，以c, Aub, bub, Aeq, beq, bounds和tol为输入。默认求解最大化问
题，其中c表示目标函数中各变量的系数；Aub为小于等于约束Aub\*x<=bub中的Aub矩阵，bub为其中右边的系数；Aeq为等于约束Aeq\*x=beq中的Aeq矩阵，beq为其中右边的系数；bounds为各变量的上下界约束；tol为整数容忍度。
初始化中设置最大分支数为max_branch_num，并初始化当前的解cur_sol和目标函数值cur_opt。

```python
    def core_solve(self, c, Aub, bub, Aeq, beq, bounds):
        sol, opt = None, None
        res = linprog(-c, A_ub=Aub, b_ub=bub, A_eq=Aeq, b_eq=beq, bounds=bounds)
```
核心求解函数，输入与类构造函数相同，首先将问题松弛为一般的线性规划问题，调用线性规划求解器求解。注意，这里的线性规划求解器是求解最小化问题的，故将c反号。

```python
        if res.success:
            opt = -res.fun
            sol = res.x
            self.update_opt(sol, opt)
```
若该线性规划求解成功，则拿解和值更新当前的解和目标函数值；否则不用分支，直接退出。

```python
            if self.needBranch(sol, opt) and self.cur_branch_num < self.max_branch_num:
                index = self.getFirstNotInt(sol)
                to_round = sol[index]
                len_c = len(self.c)
```
进一步判断是否需要分支（该函数将在之后详细讲解），并判断branch次数是否超过阈值，若需要分支则获得第一个非整数的变量的索引，和解中的对应变量的值，并继续进行下述操作；否则，直接退出。

```python
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
```
上述代码段将当前非整数索引进行划分，即分别添加x_index<=-math.ceil(to_round)和x_index>=math.floor(to_round)的约束到原来的小于等于约束中，变成两个不同的分支。

```python
                self.cur_branch_num += 1
                self.core_solve(c, A1, B1, Aeq, beq, bounds) # right branch
                self.core_solve(c, A2, B2, Aeq, beq, bounds) # left branch
```
将分好支的变量继续调用核心求解器函数，并将分支次数加1。因为每次求解时，我们都保存了整数解，故不需要多余的代码来处理之后的结果。当各个分支运行完毕之后求解结束。

```python
    def allInteger(self, sol):
        tmp = np.array([abs(x-np.round(x)) for x in list(sol)])
        return all(tmp <= self.tol)
```
该函数判断解是否都是整数。

```python
    def getFirstNotInt(self, sol):
        tmp = np.array([abs(x-np.round(x)) for x in list(sol)])
        l = tmp > self.tol
        for i in range(len(l)):
            if l[i]: return i
        return -1
```
该函数取得解中第一个非整数的索引号。

```python
    def needBranch(self, sol, opt):
        if self.allInteger(sol): return False
        elif opt <= self.cur_opt: return False
        return True
```
该函数判断当前解和值的情况下，是否需要分支。并非都是整数或者，该解的目标函数值比当前函数值要大，则需要分支。

```python
    def update_opt(self, sol, opt):
        if self.allInteger(sol) and self.cur_opt<opt:
            self.cur_opt = opt
            self.cur_sol = sol.copy()
            self.isFoundSolution = True
```
该函数用于更新当前解和目标函数值，只有在解都是整数，并且当前函数值比该解的函数值小时，才更新。

```python
    def solve(self):
        self.core_solve(self.c, self.Aub, self.bub, self.Aeq, self.beq, self.bounds)
        if self.isFoundSolution:
            self.solution = np.array([int(np.round(x)) for x in list(self.cur_sol)])
            self.optimum  = self.cur_opt
            return True
        else:
            return False
```
该函数是对核心求解器函数的一个封装，用于将构造求解器类的各个参数，作为输入传入core_solve函数中求解，最后并将结果保存到最后结果self.solution和self.optimum中。

### 测试结果



### 分析与评价

#### 时间复杂度

##### 单纯形法

如果采用了**Bland**法则选择非基变量进行转轴变换，我们时能够证明单纯形法在有限步内时一定能够终止的。单纯形法在实践中非常有效，并且比 [Fourier–Motzkin elimination](https://en.wikipedia.org/wiki/Fourier%E2%80%93Motzkin_elimination)等早期方法有了很大的改进。 然而，在1972年，Klee和Minty [1]给出了一个例子，即Klee-Minty立方体，表明由Dantzig制定的单形方法的最坏情况复杂度是指数时间。 

##### 分枝定界法

求解整数规划的精确解是NP困难的，我们没有多项式时间复杂度的算法求解。分枝定界法中，我们可能需要遍历所有的枝，所以需要$O(2^n)$次计算线性规划。而我们是使用单纯形法进行计算，所以这里我们的时间复杂度将是$O(2^n)\times O(2^n) \approx O(2^n)$ 

#### 空间复杂度



### 参考文献

[1] http://mathscinet.ams.org/mathscinet-getitem?mr=0332165

[2] https://en.wikipedia.org/wiki/Simplex_algorithm

[3] https://en.wikipedia.org/wiki/Integer_programming