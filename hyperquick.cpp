#include "hyperquick.h"

using namespace std;

void printAll(int* data, int size, int myPE, int p) {
	int* recvcounts, * displs, * recvbuf;

	if (myPE == 0)
		recvcounts = new int[p];

	MPI_Gather(&size, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (myPE == 0) {
		displs = new int[p];
		displs[0] = 0;
		for (int pe = 1; pe < p; pe++)
			displs[pe] = displs[pe - 1] +
						 recvcounts[pe - 1];
		recvbuf = new int[displs[p - 1] + recvcounts[p - 1]];
	}

	MPI_Gatherv(data, size, MPI_INT, recvbuf, recvcounts, displs, MPI_INT, 0,
				MPI_COMM_WORLD);

	if (myPE == 0) {
		for (int k = 0; k < displs[p - 1] + recvcounts[p - 1]; k++)
			cout << recvbuf[k] << endl;

		delete recvbuf;
		delete recvcounts;
		delete displs;
	}
}

void quickSort(int*&data, int &size, int myPE, int p) {
	sort(data, data + size);


	int d = (int) log2(p);

	int* dataInf = new int;
	int size1 = 0;
	int* dataSup = new int;
	int size2 = 0;

	for (int i = d; i > 0; i--) {
		int pivot = data[size / 2];
		broadcast(pivot, i, p, myPE);
		partition(pivot, data, size, dataInf, size1, dataSup, size2);

		if (!(myPE >> (i - 1) & 0x1))
			exchange(dataSup, size2, i, myPE);
		else
			exchange(dataInf, size1, i, myPE);

		size = size1 + size2;
		mergeArrays(dataInf, size1, dataSup, size2, data);
	}

}

void broadcast(int &pivot, int step, int p, int myPE) {
	int me;
	if (step == log2(p))
		me = 0;
	else
		me = ((myPE >> step) << step);

	int indexInHypercube = myPE - me;

	for (int i = 0; i < step; i++) {
		if (indexInHypercube < (0x1 << i)) {
			MPI_Send(&pivot, 1, MPI_INT, myPE + (0x1 << i), 42, MPI_COMM_WORLD);
		} else if (indexInHypercube < (0x1 << (i + 1))) {
			MPI_Recv(&pivot, 1, MPI_INT, myPE - (0x1 << i), 42, MPI_COMM_WORLD,
					 MPI_STATUS_IGNORE);
		}
	}
}

void exchange(int*&data, int &size, int step, int myPE) {
	int neighbor = myPE ^(0x1 << (step - 1));

	MPI_Send(&size, 1, MPI_INT, neighbor, 42, MPI_COMM_WORLD);

	if (size > 0) {
		MPI_Send(data, size, MPI_INT, neighbor, 42, MPI_COMM_WORLD);
	}

	//TODO We can replace with MPI_Probe
	MPI_Recv(&size, 1, MPI_INT, neighbor, 42, MPI_COMM_WORLD,
			 MPI_STATUS_IGNORE);

	if (size > 0) {
		// Clear the data and set the new size
		delete[] data;
		data = new int[size];
		// Receive array from the neighbor
		MPI_Recv(data, size, MPI_INT, neighbor, 42, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
	}
}

//split in two arrays using pivot, pivot goes in sup
void partition(int pivot, int* data, int size, int*&dataInf, int &size1,
			   int*&dataSup, int &size2) {
	//We have to get the sizes first, otherwise we can't allocate
	size1 = 0;
	size2 = 0;
	for (int j = 0; j < size; j++) {
		if (data[j] < pivot) {
			size1++;
		} else {
			size2++;
		}
	}

	delete dataInf;
	delete dataSup;
	dataInf = new int[size1];
	dataSup = new int[size2];

	int k = 0;
	int j = 0;

	for (int i = 0; i < size; i++) {
		if (data[i] < pivot) {
			dataInf[k++] = data[i];
		} else {
			dataSup[j++] = data[i];
		}
	}
}

void mergeArrays(int* data1, int size1, int* data2, int size2, int*&result) {
	int size = size1 + size2;
	delete result;
	result = new int[size];

	int finalIndex = 0;
	int firstIndex = 0;
	int secondIndex = 0;

	while (finalIndex < size) {
		if (firstIndex < size1 && secondIndex >= size2) {
			result[finalIndex++] = data1[firstIndex++];
		} else if (firstIndex >= size1 && secondIndex < size2) {
			result[finalIndex++] = data2[secondIndex++];
		} else {
			result[finalIndex++] =
					data1[firstIndex] < data2[secondIndex] ? data1[firstIndex++]
														   : data2[secondIndex++];
		}
	}
}

int main(int argc, char** argv) {

	if (argc < 3) {
		cout << "Usage: ./hyperquick <seed> <size>" << endl;
		return 1;
	}
	MPI_Init(&argc, &argv);
	int myPE, p;
	MPI_Comm_rank(MPI_COMM_WORLD, &myPE);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	uint seed = (uint) atoi(argv[1]) + myPE;
	srand(seed);
	int size = atoi(argv[2]); //this is n/p
	int* data = new int[size];

	for (int i = 0; i < size; ++i) {
		data[i] = rand() % MAX_VALUE;
	}

	//remove the text output and just leave number before handing out
	cout << "DATA BEFORE" << endl;
	printAll(data, size, myPE, p);
	quickSort(data, size, myPE, p);
	//remove the text output and just leave number before handing out
	cout << endl;
	cout << "DATA AFTER" << endl;
	printAll(data, size, myPE, p);
	MPI_Finalize();
	return 0;
}