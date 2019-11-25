#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    int myrank;

    double elapsed_time;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    for(int i = 100; i <= 10000000; i += 100){
        char message[i];
        int size = i;
        // printf("%d\n", strlen(message));

        MPI_Barrier (MPI_COMM_WORLD);
        if (myrank == 0){
            elapsed_time = - MPI_Wtime();

            MPI_Send(message, size, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
            // printf("Send message\n");
            // fflush(stdout);

            // get recv
            MPI_Recv(message, size, MPI_CHAR, 1, 99, MPI_COMM_WORLD, &status);
            // printf("Received message\n");
            // fflush(stdout);

            /* Stop timer */
            elapsed_time += MPI_Wtime();
            // printf("Size = %d, Execution time %8.6f\n", size, elapsed_time);
            printf("%d, %8.6f\n", size, elapsed_time);
            fflush(stdout);
        }else if (myrank == 1){
            MPI_Recv(message, size, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
            MPI_Send(message, size, MPI_CHAR, 0, 99, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}