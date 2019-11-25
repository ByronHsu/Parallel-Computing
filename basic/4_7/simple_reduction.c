#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    int id, p;
    int global_count = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int num = id + 1;

    MPI_Reduce(&num, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(!id){
        printf("Total %d processors. The sum is %d\n", p, global_count);
        printf("p * (p + 1) / 2 = %d", p * (p + 1) / 2);
        fflush(stdout);
    }
    
    MPI_Finalize();
    return 0;
}