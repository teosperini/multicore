#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){
    MPI_Init(NULL, NULL);
    int rank, size;
    //int global_buff[10] = {0};
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int elements_in_array = argc-1;
    int elements_per_process;

    if (rank == 0) {
        for (int i = 1; i< argc; i++){
            printf("numero %d\n", atoi(argv[i]));
        }

        if (elements_in_array % size != 0) {
            printf("Errore: il numero totale di elementi (%d) non è divisibile per il numero di processi (%d).\n", elements_in_array, size);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        elements_per_process = elements_in_array / size;
        printf("elementi per processo: %d\n", elements_per_process);

    }
    //condivido la grandezza degli array a tutti i processi
    MPI_Bcast(&elements_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //una volta dentro il processo, creo un array 

    //int local_buff[10] = {0};
    
    MPI_Finalize();
    return 0;
}