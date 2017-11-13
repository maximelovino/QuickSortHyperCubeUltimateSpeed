#ifndef _HYPERQUICK_H_
#define _HYPERQUICK_H_

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

#define MAX_VALUE 1000

/**
 * Function to display the data from each processing unit together
 * @param data The data of this processing unit
 * @param myPE The identifier of the processing unit
 * @param p The number of processing units
 */
void printAll(vector<int> &data, int myPE, int p);

/**
 * Function to do a quicksort of data using processing units in an hypercube configuration
 * @param data The data to sort
 * @param myPE The identifier of the processing unit
 * @param p The number of processing units
 */
void quickSort(vector<int> &data, int myPE, int p);

/**
 * Function to broadcast the pivot
 * @param pivot The pivot to send, will contain the pivot received then
 * @param step The step in the algorithm
 * @param p The number of processing units
 * @param myPE The identifier of the processing unit
 */
void broadcast(int &pivot, int step, int p, int myPE);

/**
 * Function to exchange data with the neigbor processing unit
 * @param data The data to send, will then contain the data received
 * @param step The step in the algorithm
 * @param myPE The identifier of the processing unit
 */
void exchange(vector<int> &data, int step, int myPE);

/**
 * Function to split a vector in two vectors
 * with elements smaller than the vector for the first, and bigger or equal for the second
 * @param pivot The pivot to use to split the data
 * @param data The data to split
 * @param dataInf The vector to put the inferior data in
 * @param dataSup The vector to put the superior or equal data in
 */
void split(int pivot, vector<int> &data, vector<int> &dataInf,
		   vector<int> &dataSup);

/**
 * Function to merge two sorted vectors in a third sorted one
 * @param data1 The first vector to merge
 * @param data2 The second vector to merge
 * @param result The vector to store the merged data in
 */
void merge(vector<int> &data1, vector<int> &data2, vector<int> &result);

#endif
