#ifndef _HYPERQUICK_H_
#define _HYPERQUICK_H_

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <math.h>

#define MAX_VALUE 1000

void printAll(int* data, int size);

void quickSort(int* data, int &size);

void broadcast(int pivot, int size);

void exchange(int* data, int &size, int step);

void partition(int pivot, int* data, int size, int* dataInf, int &size1, int* dataSup, int &size2);

void reunion(int* data1, int size1, int* data2, int size2, int* result);

#endif
