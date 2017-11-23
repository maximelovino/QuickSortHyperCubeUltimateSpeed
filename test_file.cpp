/**
 * Tests source file
 *
 * @brief	This file implements unit testing for the two sequential
 * functions of the project, we use the Catch Framework for unit testing
 * @file 	test_file.cpp
 * @project	Hyperquick
 * @author	Thomas Ibanez, Maxime Lovino
 * @date	November 23, 2017
 */

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main()

#include "catch.hpp"
#include "sequential_functions.h"
#include <time.h>
#include <algorithm>

using namespace std;

TEST_CASE("Split function", "[split]") {
	vector<int> normalVectorToSplit = {1, 3, 7, 8, 10, 20, 23, 40};
	int pivot = 9;
	vector<int> functionResultInf;
	vector<int> functionResultSup;
	vector<int> expectedResultInf;
	vector<int> expectedResultSup;
	SECTION("Checking with normal split") {
		expectedResultInf = {1, 3, 7, 8};
		expectedResultSup = {10, 20, 23, 40};
		split(pivot, normalVectorToSplit, functionResultInf, functionResultSup);
		REQUIRE(((functionResultInf == expectedResultInf) &&
				 (functionResultSup == expectedResultSup)));
	}
	SECTION("Checking that pivot goes into sup") {
		pivot = 8;
		expectedResultInf = {1, 3, 7};
		expectedResultSup = {8, 10, 20, 23, 40};
		split(pivot, normalVectorToSplit, functionResultInf, functionResultSup);
		REQUIRE(((functionResultInf == expectedResultInf) &&
				 (functionResultSup == expectedResultSup)));
	}

	SECTION("Testing splitting empty array") {
		vector<int> emptyArray = {};
		split(pivot, emptyArray, functionResultInf, functionResultSup);
		REQUIRE(((functionResultInf == emptyArray) &&
				 (functionResultSup == emptyArray)));
	}
}

TEST_CASE("Merge function", "[merge]") {
	SECTION("Testing with hardcoded data") {
		vector<int> firstArray = {1, 3, 4, 6, 7, 10, 20, 30, 50};
		vector<int> secondArray = {2, 2, 4, 5, 9, 12, 14, 22, 25};
		vector<int> functionResult;
		vector<int> expectedResult = {1, 2, 2, 3, 4, 4, 5, 6, 7, 9, 10, 12, 14,
									  20, 22, 25, 30, 50};
		merge(firstArray, secondArray, functionResult);
		REQUIRE((functionResult == expectedResult));
		merge(secondArray, firstArray, functionResult);
		REQUIRE((functionResult == expectedResult));
	}

	SECTION("Testing with randomly generated arrays and comparing to concat and sort") {
		srand(time(NULL));
		for (int i = 0; i < 100; i++) {
			vector<int> first;
			vector<int> second;

			for (int j = 0; j < (rand() % 1000); j++) {
				first.push_back(rand() % 1000);
				second.push_back(rand() % 1000);
			}
			sort(first.begin(), first.end());
			sort(second.begin(), second.end());
			vector<int> expected;
			vector<int> result;

			merge(first, second, result);
			//Concatening the two vectors and sorting them is equivalent to the reunion, but less efficient
			expected.insert(expected.end(), first.begin(), first.end());
			expected.insert(expected.end(), second.begin(), second.end());
			sort(expected.begin(), expected.end());
			REQUIRE((expected == result));
		}
	}

}