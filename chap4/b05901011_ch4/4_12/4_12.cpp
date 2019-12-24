#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <algorithm>

using namespace std;

double a = 0, b = 1, n = 50;

double f(double x){
   return (4 / (1 + x * x));
}

double xi(double i){
   return (a + (b - a) / n * i);
}

int main (int argc, char *argv[])
{
   double elapsed_time;
   int id;
   int p;
   double global_sum = 0;
   double integral = 0;

   MPI_Init (&argc, &argv);
   /* Start the timer */

   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   MPI_Comm_size (MPI_COMM_WORLD, &p);
   MPI_Barrier(MPI_COMM_WORLD);
   elapsed_time = -MPI_Wtime();

   int lower_bound = (id * (n / 2) / p) + 1;
   int upper_bound = ((id + 1) * (n / 2) / p - 1) + 1;
   double local_sum = 0;
   
   for(int i = lower_bound; i <= upper_bound; i++){
      local_sum += (4 * f(xi(2 * i - 1)) + 2 * f(xi(2 * i)));
   }

   MPI_Reduce (&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   
   if(!id){
      integral = (1 / (3 * n)) * (f(0) - f(1) + global_sum);
   }

   elapsed_time += MPI_Wtime();
   
   /* Print the results */

   if (!id) {
      printf ("PI is estimated as %.10f\n", integral);
      printf ("Time (%d) %.6f\n", p, elapsed_time);
   }

   MPI_Finalize ();
   return 0;
}
