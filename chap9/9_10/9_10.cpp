#include <bits/stdc++.h>
#include "mpi.h"
#define ll long long int

using namespace std;


bool isPrime(ll n){
    for(ll i = 2; i <= (ll)(sqrt(n)); i++){
        if(n % i == 0){
            return false;
        }
    }
    return true;
}

int main (int argc, char *argv[]) {
    int id;               /* Process rank */
    int p;                /* Number of processes */
    double elapsed_time;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    /* Start timer */
    MPI_Barrier (MPI_COMM_WORLD);
    elapsed_time = - MPI_Wtime();

    for (int i = id + 2; i <= 50; i += p){
        ll a = (ll)(pow(2, i)) - 1;
        ll b = a * (ll)(pow(2, i - 1));
        if(isPrime(a)){
            printf("%lld\n", b);
            fflush (stdout);
        }
    }

    MPI_Barrier (MPI_COMM_WORLD);
    /* Stop timer */
    elapsed_time += MPI_Wtime();
    // printf ("Process %d is done\n", id);
    if (!id) {
        printf ("Execution time %8.6f\n", elapsed_time);
        fflush (stdout);
    }
    MPI_Finalize();
    return 0;
}