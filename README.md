MiniMatlab
=========

MiniMatlab is a C++ program that imitates the functionality of the MATLAB console.

  - Type a statment like 'a = [1 3; 2 4]'
  - Press 'Enter'
  - MiniMatlab stores the variable and performs any operations you enter!

MiniMatlab supports these types of statements:
  - Assignment
```
a = 2
b = a //b now has value 2
```
  - Unary Operation
```
a++ //Adds 1 to a
```
  - Binary Operation
```
5 + 3 //Stores the value 9 in the default variable 'ans'
a + 3 //Adds 3 to a and stores the result in 'ans'
```
  - Binary Operation and Assignment
```
c = a / 2 //Stores the result of 'a/2' in c
```
  - Compound Operation
```
a *= 4
```
  - Matrix Declaration and Operation
```
a = [1 2; 3 4]
b = [5; 6]
a * b //Computes the result of matrix multiplication
```
  - Special Commands
```
who //Prints a list of all the currenlty stored variables
quit //Terminates the program
```
