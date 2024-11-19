#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

/*
In questo esempio, i singoli processi si creano la matrice A e il vettore x con il
quale eseguire le moltiplicazioni:
- ogni processo si crea il proprio numero di righe della matrice A
- ogni processo si crea il proprio numero di elementi di x
- dato che x deve essere comune per tutti i processi, con la Allgather riuniscono
le varie parti e le ricondividono a ogni processo
- effettuano i calcoli in parallelo per la loro porzione di matrice e mettono i
risultati nella loro porzione di vettore finale, che stampano
EDIT:
- effettuano i calcoli in parallelo per la loro porzione di matrice e convogliano i
risultati in un vettore nel processo root
*/



void Mat_vect_mult(
        double   local_A[],
        double   local_x[],
        double   local_y[],
        int      local_m,
        int      n,
        int      local_n,
        MPI_Comm comm) {

    double* x;
    int local_i, j;

    x = malloc(n * sizeof(double));
    MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, comm);
    
    for (local_i = 0; local_i < local_m; local_i++) {
        local_y[local_i] = 0.0;
        for (j = 0; j < n; j++) {
            local_y[local_i] += local_A[local_i * n + j] * x[j];
        }
    }
    free(x);
}

int main(int argc, char* argv[]) {
    int rank, size;
    int m = 4, n = 4; // Matrice 4x4 come esempio
    int local_m, local_n;
    double *local_A, *local_x, *local_y;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_m = m / size; // Numero di righe della matrice per ogni processo
    local_n = n / size; // Dimensione della porzione del vettore per ogni processo

    // Allocazione della porzione locale della matrice e del vettore
    local_A = malloc(local_m * n * sizeof(double)); // Ogni processo ha local_m righe e n colonne
    local_x = malloc(local_n * sizeof(double));    // Ogni processo ha una porzione del vettore
    local_y = malloc(local_m * sizeof(double));    // Risultato locale

    // Inizializzazione dei dati
    for (int i = 0; i < local_m * n; i++) {
        local_A[i] = rank + 1; // Ogni processo inizializza le proprie righe con valori diversi
    }
    for (int i = 0; i < local_n; i++) {
        local_x[i] = rank + 1; // Ogni processo inizializza il proprio pezzo di vettore
    }

    // Moltiplicazione matrice-vettore distribuita
    Mat_vect_mult(local_A, local_x, local_y, local_m, n, local_n, MPI_COMM_WORLD);

    // Raccolta del risultato globale nel processo root
    double* y = NULL; // Risultato globale (solo nel root)
    if (rank == 0) {
        y = malloc(m * sizeof(double)); // Allocare spazio per il risultato totale
    }

    MPI_Gather(local_y, local_m, MPI_DOUBLE, y, local_m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Stampa del risultato globale (solo nel root)
    if (rank == 0) {
        printf("Risultato della moltiplicazione matrice-vettore:\n");
        for (int i = 0; i < m; i++) {
            printf("y[%d] = %f\n", i, y[i]);
        }
        free(y); // Liberare la memoria nel root
    }
    
    /*
    // Stampa del risultato locale
    for (int i = 0; i < local_m; i++) {
        printf("Processo %d, local_y[%d] = %f\n", rank, i, local_y[i]);
    }
    */


    // Libera memoria e termina MPI
    free(local_A);
    free(local_x);
    free(local_y);
    MPI_Finalize();
    return 0;
}