#import <stdio.h>
#import <stdlib.h>
#import <pthread.h>

// abbiamo un n. dobbiamo fare n divisioni

void* thread_sum(void* rank){
	long my_rank = (long) rank;
	double factor;
	long long i;
	long long my_n = n/thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;

	if(my_first_i % 2 == 0){
		factor = 1.0;
	} else {
		factor = -1.0;
	}

	for (i = my_first_i; i < my_last_i; i++, factor = -factor){
		sum += factor/(2*1+1);		// non funziona bene per una possibile race condition
	}

	return NULL;
}

//quando voglio passare un puntatore da dentro il thread a fuori dal thread (dalla funzione del thread al main)
//faccio una malloc e poi fuori dal main faccio la free

int main(int argc, char** argv){


	n_thread = 8

	pthread_t* thread_vector = calloc(n_thread, sizeof(pthread_t));

	for(i = 0; i<n_thread; i++, factor = -factor){
		
		pthread_create();
	}
	j = 0;
	while(j<n_thread){

	}

	return 0;
	//fflush();
}