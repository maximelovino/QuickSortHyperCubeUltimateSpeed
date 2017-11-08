#include "hyperquick.h"

using namespace std;

void printAll(int* data, int size) {
	int p, myPE;
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myPE);

	int* recvcounts, * displs, * recvbuf;

	if (myPE == 0)
		recvcounts = new int[p];

	MPI_Gather(&size, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (myPE == 0) {
		displs = new int[p];
		displs[0] = 0;
		for (int pe = 1; pe < p; pe++) displs[pe] = displs[pe - 1] + recvcounts[pe - 1];
		recvbuf = new int[displs[p - 1] + recvcounts[p - 1]];
	}

	MPI_Gatherv(data, size, MPI_INT, recvbuf, recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

	if (myPE == 0) {
		for (int k = 0; k < displs[p - 1] + recvcounts[p - 1]; k++)
			cout << recvbuf[k] << endl;

		delete recvbuf, recvcounts, displs;
	}
}

void quickSort(int* data, int &size) {
	int p, myPE;
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myPE);

	sort(begin(data), end(data));


	int d = (int) (log(p) / log(2));

	for (int i = d; i > 0; i--) {
		int pivot = data[size / 2];
		broadcast(pivot, 1);
	}

}

void broadcast(int pivot, int size) {

}

void exchange(int* data, int &size, int step) {

}

void partition(int pivot, int* data, int size, int* dataInf, int &size1, int* dataSup, int &size2) {
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

void reunion(int* data1, int size1, int* data2, int size2, int* result) {

}

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "Usage: ./hyperquick <seed> <size>" << endl;
		return 1;
	}
	MPI_Init(&argc, &argv);
	int myPE;
	MPI_Comm_rank(MPI_COMM_WORLD, &myPE);
	uint seed = (uint) atoi(argv[1]) + myPE;
	srand(seed);
	int size = atoi(argv[2]); //this is n/p
	int* data = new int[size];

	for (int i = 0; i < size; ++i) {
		data[i] = rand() % MAX_VALUE;
	}

	printAll(data, size);
	quickSort(data, size);
	printAll(data, size);

	MPI_Finalize();
	return 0;
}