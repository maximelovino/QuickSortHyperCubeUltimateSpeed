/**
 * Sequential functions header file
 *
 * @brief	This file contains the sequential functions of our program, that don't require MPI
 * @file 	sequential_functions.h
 * @project	Hyperquick
 * @author	Thomas Ibanez, Maxime Lovino
 * @date	November 23, 2017
 */

#ifndef _SEQUENTIAL_FUNCTIONS_H_
#define _SEQUENTIAL_FUNCTIONS_H_

#include <vector>
using namespace std;

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
