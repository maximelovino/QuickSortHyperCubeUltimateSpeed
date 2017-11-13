#include "hyperquick.h"

void printAll(vector<int> &data, int myPE, int p) {
	int* recvcounts, * displs, * recvbuf;

	if (myPE == 0)
		recvcounts = new int[p];
	int size = data.size();

	MPI_Gather(&size, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (myPE == 0) {
		displs = new int[p];
		displs[0] = 0;
		for (int pe = 1; pe < p; pe++)
			displs[pe] = displs[pe - 1] +
						 recvcounts[pe - 1];
		recvbuf = new int[displs[p - 1] + recvcounts[p - 1]];
	}

	MPI_Gatherv(&(data.front()), data.size(), MPI_INT, recvbuf, recvcounts,
				displs, MPI_INT, 0,
				MPI_COMM_WORLD);

	if (myPE == 0) {
		for (int k = 0; k < displs[p - 1] + recvcounts[p - 1]; k++)
			cout << recvbuf[k] << endl;

		delete recvbuf;
		delete recvcounts;
		delete displs;
	}
}

void quickSort(vector<int> &data, int myPE, int p) {
	sort(begin(data), end(data));


	auto d = (int) log2(p);

	vector<int> dataInf, dataSup;

	for (int i = d; i > 0; i--) {
		int pivot = data[data.size() / 2];

		broadcast(pivot, i, p, myPE);

		split(pivot, data, dataInf, dataSup);

		if (!(myPE >> (i - 1) & 0x1))
			exchange(dataSup, i, myPE);
		else
			exchange(dataInf, i, myPE);

		merge(dataInf, dataSup, data);
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

void exchange(vector<int> &data, int step, int myPE) {
	int neighbor = myPE ^(0x1 << (step - 1));

	if (!data.empty()) {
		MPI_Send(&data.front(), data.size(), MPI_INT, neighbor, 42,
				 MPI_COMM_WORLD);
	}
	//this is get the size without needing to send it
	MPI_Status status;
	MPI_Probe(neighbor, 42, MPI_COMM_WORLD, &status);
	int size;
	MPI_Get_count(&status, MPI_INT, &size);


	if (size > 0) {
		// Receive array from the neighbor
		data.clear();
		data.resize(size);
		MPI_Recv(&data.front(), size, MPI_INT, neighbor, 42, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
	}
}

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

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "Usage: ./hyperquick <seed> <size>" << endl;
		return 1;
	}

	MPI_Init(&argc, &argv);
	int myPE, p;
	MPI_Comm_rank(MPI_COMM_WORLD, &myPE);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	auto seed = (uint) atoi(argv[1]) + myPE;
	srand(seed);
	vector<int> data;
	auto size = atoi(argv[2]); //this is n/p
	data.reserve(size);
	for (int i = 0; i < size; ++i) {
		data.push_back(rand() % MAX_VALUE);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printAll(data, myPE, p);
	quickSort(data, myPE, p);
	printAll(data, myPE, p);
	MPI_Finalize();
	return 0;
}