#include <bits/stdc++.h>
#include "mpi.h"

using namespace std;

double randSingle(){
    double min = -1;
    double max = 1;
    double r = (max - min) * rand() / RAND_MAX + min;
    return r;
}

double distance(double x, double y, double z){
    double d = sin(acos((x + y + z) / (sqrt(3) * sqrt(x * x + y * y + z * z)))) * sqrt(x * x + y * y + z * z);
    return d;
}

int main (int argc, char *argv[]) {
    int id;               /* Process rank */
    int p;                /* Number of processes */
    int n_samples = 10000000;
    int global_positive = 0;
    double elapsed_time;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    /* Start timer */
    MPI_Barrier (MPI_COMM_WORLD);
    elapsed_time = - MPI_Wtime();
    int local_positive = 0;
    /* Random generation */
    for(int i = 0; i < n_samples / p; i++){
        double x = randSingle();
        double y = randSingle();
        double z = randSingle();
        double d = distance(x, y, z);
        if(d > 0.15) local_positive += 1;
    }
    
    MPI_Reduce (&local_positive, &global_positive, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double prob, base, volume;
    if(!id){
        prob = (double)global_positive / (double)n_samples;
        base = 2 * 2 * 2;
        volume = prob * base;
    }
    MPI_Barrier (MPI_COMM_WORLD);
    /* Stop timer */
    elapsed_time += MPI_Wtime();
    // printf ("Process %d is done\n", id);
    if (!id) {
        printf ("The volume is estimated as %.5f by %d samples\n", volume, n_samples);
        printf ("Execution time %8.6f\n", elapsed_time);
        fflush (stdout);
    }
    MPI_Finalize();
    return 0;
}