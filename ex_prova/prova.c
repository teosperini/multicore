#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int my_rank, comm_sz;

    // Inizializzazione di MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    printf("Processo %d di %d processi.\n", my_rank, comm_sz);

    // Finalizzazione di MPI
    MPI_Finalize();
    return 0;
}
