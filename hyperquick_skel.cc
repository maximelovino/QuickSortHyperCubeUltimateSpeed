#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

void reunion(int* data1,int taille1,
             int* data2,int taille2,int* result) {  }

void partition(int pivot, int* data, int taille,
               int* dataInf,int& taille1,
               int* dataSup,int& taille2) {  }

void exchange(int* data,int& taille,int etape) {  }

void diffusion(int pivot,int etape) {  }

void quickSort(int* data,int& taille) {  }

void printAll(int* data,int taille) {  
   int p,myPE;
   MPI_Comm_size(MPI_COMM_WORLD,&p);
   MPI_Comm_rank(MPI_COMM_WORLD,&myPE);
   int *recvcounts, *displs, *recvbuf;
   if (myPE == 0) recvcounts = new int[p];
   MPI_Gather(&taille,1,MPI_INT,recvcounts,1,MPI_INT,0,MPI_COMM_WORLD);
   if (myPE == 0) {
      displs = new int[p];
      displs[0] = 0;
      for (int pe=1;pe<p;pe++) displs[pe] = displs[pe-1]+recvcounts[pe-1];
      recvbuf = new int[displs[p-1]+recvcounts[p-1]];
   }
   MPI_Gatherv(data,taille,MPI_INT,recvbuf,recvcounts,displs,MPI_INT,0,MPI_COMM_WORLD);
   if (myPE == 0)
      for (int k=0;k<displs[p-1]+recvcounts[p-1];k++) cout << recvbuf[k] << endl;
   if (myPE == 0) delete recvbuf,recvcounts,displs;
}

int main(int argc,char** argv) {
  MPI_Init(&argc,&argv);
  int myPE;
  MPI_Comm_rank(MPI_COMM_WORLD,&myPE);
  int seed = atoi(argv[1])+myPE; 
  srand(seed);
  int tailleLoc = atoi(argv[2]); // tailleLoc vaut n/p
  int* dataLoc = new int[tailleLoc];
  for (int k=0;k<tailleLoc;k++) dataLoc[k] = rand()%1000;
  printAll(dataLoc,tailleLoc);
  quickSort(dataLoc,tailleLoc);
  printAll(dataLoc,tailleLoc);
  MPI_Finalize();
  return 0;
}
