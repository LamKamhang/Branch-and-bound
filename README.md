# Branch-and-bound
A simple program for solving integer programming using the branch and bound method.

## files description
```
bin        : executable files
solution   : our solutions for some cases
src        : our source codes
test_case  : test cases
report.pdf : our report
```

## how to compile
required: Eigen3
```
$cmake . -Bbuild
$cd build && make
```
or you can just run the executable files we provided.


## how to run
you need first parse the .lp file to .txt file in our formal, then use branch_bound.exe to solve it. All executable files can be found in floder bin.

```
$parse.exe input.lp
```
It will parse the input.lp to a txt file named input.txt

```
$branch_bound.exe input.txt output.txt
```
It will solve the problem and output the result to output.txt

>example
```
$cd bin
$parse.exe ../test_case/from_web_site/case3.lp
$branch_bound.exe ../test_case/from_web_site/case3.txt ../solution/case3.txt
```

## attention
We use python script **test.py** to test the programs, and the executable files we provided also were related to it. So we do not assure you can compile the program in the right way.

## team members
陈翰逸, 林锦铿, 黄文璨, 赵竟霖
