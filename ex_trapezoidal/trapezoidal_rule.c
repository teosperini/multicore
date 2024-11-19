#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

// Definizione della funzione da integrare
double f(double x) {
    return x * x; // esempio di funzione, f(x) = x^2
}

/**
 * Questa funzione calcola l'area della parte di funzione tra left_endpt e right_endpt,
 * dividendo l'intervallo in trap_count trapezi larghi base_len
 */
double Trap (
    double left_endpt,      // bordo sinistro intervallo 
    double right_endpt,     // bordo destro intervallo
    int trap_count,         // numero di trapezi
    double base_len){       // larghezza dei trapezi
    
    double estimate,x;
    int i;

    estimate = (f(left_endpt) + f(right_endpt))/2.0;    // inizialmente calcola la media ai lati (f(l) + f(r))/2
    for(i=1; i<=trap_count-1;i++){                      // poi calcola la media di ogni trapezio, prendendo il valore centrale
        x = left_endpt + i*base_len;
        estimate += f(x);
    }
    estimate=estimate*base_len;
    return estimate;
}

/**
 * Il metodo dei trapezi approssima l'area sotto una curva come somma di tanti piccoli trapezi,
 * ognuno dei quali rappresenta una parte della funzione sull’intervallo scelto
 */
int main(void) {
    int my_rank, comm_sz, n = 1024, local_n;
    double a = 0.0, b = 3.0, h, local_a, local_b;
    double local_int, total_int;
    int source;
    MPI_Init (NULL, NULL);
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size (MPI_COMM_WORLD, &comm_sz);
    h = (b-a)/n;
    printf("%d\n",n);
    local_n = n/comm_sz;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a, local_b, local_n, h);
    if (my_rank != 0){
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0,MPI_COMM_WORLD);
    }else{
        total_int = local_int;
        for (source = 1; source < comm_sz; source++){
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
    }
    if (my_rank == 0){
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
    }
    MPI_Finalize();
    return 0;
}