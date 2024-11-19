#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

// Funzione per leggere e distribuire un vettore tra i processi MPI
void read_vector(
            double local_a[],     // Array locale che riceverà la porzione di dati
            int local_n,          // Numero di elementi assegnati a ciascun processo
            int n,                // Numero totale di elementi nel vettore
            char vec_name[],      // Nome del vettore, usato per visualizzare un messaggio di input
            int my_rank,          // Rank (identificatore) del processo corrente
            MPI_Comm comm ) {     // Comunicatore MPI che definisce l'insieme dei processi
    double* a = NULL;             // Puntatore per memorizzare temporaneamente il vettore completo nel processo 0
    int i;

    // Se il processo corrente è il processo radice (rank 0)
    if (my_rank == 0){
        // Alloca memoria per il vettore completo
        a = malloc(n * sizeof(double));
        
        // Messaggio per richiedere l'input del vettore
        printf("Enter the vector %s\n", vec_name);

        // Ciclo per leggere gli elementi del vettore da standard input
        for ( i = 0; i < n; i++){
            scanf("%lf", &a[i]);
        }

        // Distribuisce (scatter) i dati dal vettore `a` ai vari processi
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
        
        // Libera la memoria allocata per il vettore completo, poiché ora i dati sono stati distribuiti
        free(a);
    } else {
        // Gli altri processi chiamano `MPI_Scatter` per ricevere la loro porzione del vettore
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
    }
}


int main(int argc, char* argv[]) {
    int my_rank, comm_sz;
    int n = 8;           // Dimensione totale del vettore
    int local_n;         // Numero di elementi per ciascun processo
    double* local_a;     // Array locale per memorizzare la porzione di vettore di ciascun processo

    // Inizializza l'ambiente MPI
    MPI_Init(&argc, &argv);

    // Ottiene il rank del processo corrente (unico per ciascun processo)
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Ottiene il numero totale di processi nel comunicatore
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Calcola il numero di elementi del vettore per ciascun processo
    local_n = n / comm_sz;

    // Alloca memoria per l'array locale per ciascun processo
    local_a = malloc(local_n * sizeof(double));

    // Chiama la funzione `read_vector` per leggere e distribuire il vettore
    read_vector(local_a, local_n, n, "A", my_rank, MPI_COMM_WORLD);

    // Stampa la porzione di vettore ricevuta da ciascun processo
    printf("Processo %d ha ricevuto: ", my_rank);
    for (int i = 0; i < local_n; i++) {
        printf("%f ", local_a[i]);
    }
    printf("\n");

    // Libera la memoria allocata per l'array locale
    free(local_a);

    // Termina l'ambiente MPI
    MPI_Finalize();

    return 0;
}