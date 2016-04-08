//main.cpp
//CSC330 project 2
//Sean Wagner


#include <iostream>
#include <mpi.h>

//the main method which runs the program
int main(int argc, char * argv[]) {
	int ierr;
	//the storage array, each location corresponds to a node
	//int retVals[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//number of trials to execute
	int numTrials = 10000;
	ierr = MPI_Init(&argc, &argv);
	//the data that will be sent out
	int newArr[5];
	newArr[0] = 5;
	newArr[1] = 10;
	newArr[2] = 20;
	newArr[3] = 35;
	newArr[4] = 40;
	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int retVals[size+1];
	for(int j = 0;j<=size;j++) {
		retVals[j] = 0;
	}

	MPI_Status status;
	int tag = 1234;	
	
	//main loop to execute the trail a number of times
	for(int z = 0; z<numTrials;z++) {	
	int result = MPI_Bcast(newArr,5,MPI_INT,0,MPI_COMM_WORLD);
	int k = 1;
	//checks if head node and then executes the response
	if(rank != 0) {
		//std::cout << "I am a baby node";
		MPI_Send(newArr,5,MPI_INT,0,tag,MPI_COMM_WORLD);
	}
	//carries out processing from head node
	else {
		//looks for responses from each node
		for(int i = 1; i<size; i++) {
			MPI_Recv(newArr,5, MPI_INT,MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			int currNode = status.MPI_SOURCE;
			int prevVal = retVals[currNode];
			prevVal = prevVal + k;
			retVals[currNode] = prevVal;
			k++;
		}
	}
	}
	
	//prints the results for analysis
	if(rank==0) {
		for(int m = 0; m < size; m++) {
			int printVal = retVals[m]/numTrials;
			if(m>=10) {
				std::cout << " n" <<m<<"  "<<retVals[m]<<"    avg = " << printVal << "\n";
			}
			else {
				std::cout << "  n" <<m<<"  "<<retVals[m]<<"    avg = " << printVal << "\n";
			}
		}
	}
	//finalizes mpi and exits
	MPI_Finalize();
	return 0;
}
