#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

double rand_number(){
    return 2.0*(rand() / (double)RAND_MAX) - 1.0;
}
int main(int argc, char** argv){
    MPI_Init(NULL,NULL);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(time(NULL) + rank);

    int number_of_tosses = 10000;
    int number_in_circle = 0;

    int tosses_per_process = number_of_tosses / size;

    for (int toss = 0; toss < tosses_per_process; toss++){
        double x = rand_number();
        double y = rand_number();
        double distance_sq = x*x + y*y;
        if (distance_sq <= 1){
            number_in_circle++;
        }
    }
    int global_count = 0;

    MPI_Reduce(&number_in_circle, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0){
        double pi_estimate = 4.0 * global_count / number_of_tosses; // Calcola l'approssimazione di pi
        printf("Estimated value of pi = %f\n", pi_estimate);
    }
    MPI_Finalize();
    return 0;
}
