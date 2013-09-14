#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>
#include <papi.h>

#define SIZE 1000000

static uint64_t residual_error = 0;

inline uint64_t get_time(void) {
    return (uint64_t) PAPI_get_virt_cyc();
}

void init_time(void) {
	uint64_t t1, t2;
	t1 = get_time();
	t2 = get_time();
	residual_error = t2 - t1;
}

inline uint64_t diff_time(uint64_t t1, uint64_t t2) {
	return t2 - t1 - residual_error;
}

int compare_ints (const void *a, const void *b) {
   const int *ia = (const int *) a;
   const int *ib = (const int *) b; 
   return (*ia > *ib) - (*ia < *ib);
}

int main (int argc, char *argv[]) {
	int i, count;
	int *array = (int*) malloc (SIZE * sizeof(int));
	uint64_t start, end;
    int events[3] = { PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L3_DCM };
    long long misses[3];
    int papilevels = 3;

    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
        exit(1);
    }

	//Initialization
	count = 0;
	srand(time(NULL));
	init_time();
	for (i = 0; i < SIZE; i++)
		array[i] = rand();

	//Measurement
    while (PAPI_start_counters(events, papilevels) != PAPI_OK) {
        papilevels--;
    }
	start = get_time();
	/*
	 * É possível, em um vetor ordenado, fazer a contagem 
	 * em tempo O(lg(n)) em vez de O(n) utilizando busca 
	 * binária. Embora isto diminua sensivelmente o tempo 
	 * este não é o intuito deste exercício.
	 *
	 */
	for (i = 0; i < SIZE; i++)
		if (array[i] < RAND_MAX / 2)
			count++;
	end = get_time();
	uint64_t exec_time = diff_time(start, end);
    if (PAPI_read_counters(misses, papilevels) != PAPI_OK) {
        fprintf(stderr, "Erro em PAPI_read_counters\n");
        exit(1);
    }

	printf("Time: %" PRIu64 " Count %d\n",  exec_time, count);
    for (i = 0; i < papilevels; i++) {
        printf("Cache misses (L%d): %lld\n", i+1, misses[i]);
    }
	free(array);
	return 0;
}
