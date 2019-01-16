# Branch-and-bound
A simple program for solving integer programming using the branch and bound method.

## files description
```
bin       : executable files
solution  : our solutions for some cases
src       : our source codes
test_case : test cases
```

## how to compile
required: Eigen3
```
$cmake . -Bbuild
$cd build && make
```
or you can just run the executable files we provided.

## how to run
```
$parse.exe input.lp
```
It will parse the input.lp to a txt file named input.txt

```
$branch_bound.exe input.txt output.txt
```
It will solve the problem and output the result to output.txt
