#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // Include for malloc and free

/*
The following structure contains the necessary information
to allow the function "dotprod" to access its input data and
place its output into the structure. This structure is
unchanged from the sequential version.
*/
typedef struct {
    double *a;
    double *b;
    double sum;
    int veclen;
} DOTDATA;

/* Define globally accessible variables and a mutex */
#define NUMTHRDS 4
#define VECLEN 100
 
DOTDATA dotstr;
pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;

void *dotprod(void *arg) {
    /* Define and use local variables for convenience */
    int i, start, end, offset, len;
    double mysum, *x, *y;
    offset = (int)arg;

    /* Calculate start and end indices for this thread */
    len = dotstr.veclen;
    start = offset * len;
    end = start + len;

    /* Perform the dot product */
    mysum = 0;
    x = dotstr.a;
    y = dotstr.b;
    for (i = start; i < end; i++) {
        mysum += (x[i] * y[i]);
    }

    /* Update the sum in the global structure */
    pthread_mutex_lock(&mutexsum);
    dotstr.sum += mysum;
    pthread_mutex_unlock(&mutexsum);
   
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int i;
    double *a, *b;
    int status;
    pthread_attr_t attr;

    /* Assign storage and initialize values */
    a = (double*)malloc(NUMTHRDS * VECLEN * sizeof(double));
    b = (double*)malloc(NUMTHRDS * VECLEN * sizeof(double));
    for (i = 0; i < NUMTHRDS * VECLEN; i++) {
        a[i] = 1;
        b[i] = a[i];
    }
   
    dotstr.veclen = VECLEN;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;
    pthread_mutex_init(&mutexsum, NULL);

    /* Create threads to perform the dot product */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (i = 0; i < NUMTHRDS; i++) {
        pthread_create(&callThd[i], &attr, dotprod, (void*)i);
    }
    pthread_attr_destroy(&attr);

    /* Wait for threads to finish */
    for (i = 0; i < NUMTHRDS; i++) {
        pthread_join(callThd[i], NULL);
    }

    /* Print out the results and cleanup */
    printf("Sum = %f \n", dotstr.sum);

    free(a);
    free(b);
    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
}
