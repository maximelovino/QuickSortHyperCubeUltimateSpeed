#ifndef _HYPERQUICK_H_
#define _HYPERQUICK_H_

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <math.h>

#define MAX_VALUE 1000

void printAll(int* data, int size, int myPE, int p);

void quickSort(int*&data, int &size, int myPE, int p);

void broadcast(int &pivot, int step, int p, int myPE);

void exchange(int*&data, int &size, int step, int myPE);

void partition(int pivot, int* data, int size, int*&dataInf, int &size1,
			   int*&dataSup, int &size2);

void mergeArrays(int* data1, int size1, int* data2, int size2, int*&result);

#endif
