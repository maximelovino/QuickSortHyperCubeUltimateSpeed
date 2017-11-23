/**
 * Sequential functions source file
 *
 * @brief	This file contains the sequential functions of our program, that don't require MPI
 * @file 	sequential_functions.cpp
 * @project	Hyperquick
 * @author	Thomas Ibanez, Maxime Lovino
 * @date	November 23, 2017
 */

#include "sequential_functions.h"

void split(int pivot, vector<int> &data, vector<int> &dataInf,
		   vector<int> &dataSup) {
	dataInf.clear();
	dataSup.clear();

	for (int i : data) {
		if (i < pivot) {
			dataInf.push_back(i);
		} else {
			dataSup.push_back(i);
		}
	}
}

void merge(vector<int> &data1, vector<int> &data2, vector<int> &result) {
	result.clear();

	auto finalIndex = 0;
	auto firstIndex = 0;
	auto secondIndex = 0;

	while (finalIndex < (data1.size() + data2.size())) {
		if (firstIndex < data1.size() && secondIndex >= data2.size()) {
			result.push_back(data1[firstIndex++]);
		} else if (firstIndex >= data1.size() && secondIndex < data2.size()) {
			result.push_back(data2[secondIndex++]);
		} else {
			if (data1[firstIndex] < data2[secondIndex])
				result.push_back(data1[firstIndex++]);
			else
				result.push_back(data2[secondIndex++]);
		}
		finalIndex++;
	}
}