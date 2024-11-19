#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

/*
In questo esempio, il processo root crea la matrice A e il vettore x:
- viene condiviso un determinato numero di righe della matrice A ad ogni processo
- viene condiviso l'intero vettore x ad ogni processo, dato che deve essere comune
per tutti
- vengono effettuati i calcoli in parallelo per la loro porzione di matrice
- i risultati vengono convogliati in un vettore finale nel processo root, che li stampa
*/

void Mat_vect_mult(
        double   local_A[],
        double   x[],
        double   local_y[],
        int      local_m,
        int      n) {

    int local_i, j;

    for (local_i = 0; local_i < local_m; local_i++) {
        local_y[local_i] = 0.0;
        for (j = 0; j < n; j++) {
            local_y[local_i] += local_A[local_i * n + j] * x[j];
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, size;
    int m = 4, n = 4; // Dimensioni della matrice (4x4 come esempio)
    int local_m;
    double *A = NULL, *x = NULL; // Matrice e vettore inizializzati nel root
    double *local_A, *local_y;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (m % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "La matrice non può essere distribuita equamente tra i processi.\n");
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    local_m = m / size; // Numero di righe della matrice per ogni processo

    // Allocazione memoria per porzioni locali
    local_A = malloc(local_m * n * sizeof(double));
    local_y = malloc(local_m * sizeof(double));

    // Processo root inizializza la matrice e il vettore
    if (rank == 0) {
        A = malloc(m * n * sizeof(double));
        x = malloc(n * sizeof(double));

        // Inizializzazione esempio
        for (int i = 0; i < m * n; i++) {
            A[i] = i + 1; // Valori incrementali per esempio
        }
        for (int i = 0; i < n; i++) {
            x[i] = i + 1; // Vettore x: [1, 2, 3, ...]
        }
    }

    // Distribuzione della matrice tra i processi
    MPI_Scatter(A, local_m * n, MPI_DOUBLE, local_A, local_m * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Trasmissione del vettore a tutti i processi
    if (rank == 0) {
        free(A); // La matrice completa non serve più nel root
    }
    if (rank != 0) {
        x = malloc(n * sizeof(double)); // Allocazione per processi non-root
    }
    MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Moltiplicazione matrice-vettore distribuita
    Mat_vect_mult(local_A, x, local_y, local_m, n);

    // Raccolta del risultato globale nel processo root
    double* y = NULL;
    if (rank == 0) {
        y = malloc(m * sizeof(double));
    }
    MPI_Gather(local_y, local_m, MPI_DOUBLE, y, local_m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Stampa del risultato globale (solo nel root)
    if (rank == 0) {
        printf("Risultato della moltiplicazione matrice-vettore:\n");
        for (int i = 0; i < m; i++) {
            printf("y[%d] = %f\n", i, y[i]);
        }
        free(y);
    }

    // Libera memoria e termina MPI
    free(local_A);
    free(local_y);
    free(x);
    MPI_Finalize();
    return 0;
}
