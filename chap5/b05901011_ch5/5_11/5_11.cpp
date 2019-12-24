#include <bits/stdc++.h>
// #include <math.h>
// #include <stdio.h>
// #include <cstdlib>
// #include <algorithm>
#include <cstring>
using namespace std;

#include "mpi.h"

int base = 1000000;
int len = 100;
int n = 1000000;

int* calSubsum(int p, int id){
    /*
    使用大數法計算第id個processor的和
    */
    int low = (id * n / p) + 1;
    int up = ((id + 1) * n / p - 1) + 1;

    int* val = new int[len];
    memset(val, 0, sizeof(int) * len);

    // {val[0], val[1], val[2]... val[len-1]} 接起來看 代表的是整個商

    for(int i = low; i <= up; i++ ){
        // perform long division
        long long int dividend = 1; // 至少要容納 n * base
        for(int j = 0; j < len; j++){
            val[j] += dividend / i;
            dividend %= i;
            dividend *= base;
        }

        // 將val進位
        for(int j = len - 1; j >= 0; j--){
            val[j - 1] += val[j] / base;
            val[j] %= base;
        }
    }

    return val;
}


void big_sum(void *inP, void *outP, int *len, MPI_Datatype *dptr){
    int* in = (int*)inP;
    int* out = (int*)outP;

    for(int i = *len - 1; i >= 0; i--){
        out[i] += in[i];
        out[i - 1] += out[i] / base;
        out[i] %= base;
    }
}

int main (int argc, char *argv[])
{
    int id;
    int p;
    double elapsed_time;

    MPI_Init (&argc, &argv);
    /* Start the timer */

    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    MPI_Op myOp;
    MPI_Op_create(big_sum, true, &myOp);

    int* sum = new int[len];
    memset(sum, 0, sizeof(int) * len);

    int* sub_sum = calSubsum(p, id);

    MPI_Reduce(sub_sum, sum, len, MPI_INT, myOp, 0, MPI_COMM_WORLD);

    elapsed_time += MPI_Wtime();
    
    /* Print the results */

    if (!id) {
        printf("Time: %.4f\n", elapsed_time);
        printf("Ans: ");
        printf("%d.", sum[0]);
        for(int i = 1; i < len; i++){
            printf("%d", sum[i]);
        }
        printf("\n");
    }

    MPI_Finalize ();
    return 0;
}
