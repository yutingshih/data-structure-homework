# Data Structure
Homework and practices for Data Structure, 2021 Fall, NCKU CSIE

Textbook: Fundamentals of Data Structures in C 2nd edition by Ellis Horowitz, Sartaj Sahni, and Susan Anderson-Freed 

## Compile

All the homework problems share the same Makefile. To do the compilation, please navigate into one of the homework directories, and then "make" it with `../ Makefile`. 

```bash
cd homework
make -f ../Makefile
```

## Run Tests

There may be several test inputs and golden outputs in each homework directories. To check if the program is correct, please run the following commamd which shows the differences between the program's output and the correct answer.

```bash
./a.out < input.txt | diff -w - output.txt
```

