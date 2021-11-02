# Data Structure
Homework and practices for Data Structure, 2021 Fall, NCKU CSIE

Textbook: Fundamentals of Data Structures in C 2nd edition by Ellis Horowitz, Sartaj Sahni, and Susan Anderson-Freed 

## Compile

All the homework problems share the same Makefile. To do the compilation, please navigate into one of the homework directories, and then "make" it with `../Makefile`. 

```bash
cd homework
make -f ../Makefile
```

## Run Tests

There may be several test inputs and golden outputs in each homework directories. To check if the program is correct, please run the following commamd

```bash
make -f ../Makefile test
```

or directly run the test script in the homework subdirectory.

```bash
../test.sh
```

Both ways show the result of each test case, passed or failed. Additionally, if there is any test case failed, it will show you the differences between the program's output and golden output.
