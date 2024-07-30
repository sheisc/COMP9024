/********************************************************************************
            How to Use



                                                            COMP9024

 ***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define N_LOOPS   (1000 * 1000)

#define N_THREADS   8

// mutually exclusive
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// ticket counter
long counter = 0;

//#define USE_OUR_MUTEX_LOCK

/*
    Increase the counter.
 */
static void *run(void *arg) {  
    for (long i = 0; i < N_LOOPS; i++) {
#ifdef USE_OUR_MUTEX_LOCK          
        pthread_mutex_lock(&mutex);
        counter = counter + 1;
        pthread_mutex_unlock(&mutex);
#else
        counter = counter + 1;
#endif
    }

    return NULL;
}


int main(void) {
    pthread_t workers[N_THREADS];

    for (long k = 0; k < N_THREADS; k++) {
        /*
            int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);

            On  success,  pthread_create() returns 0; 
            on error, it returns an error number, and the contents of *thread are undefined.
         */   
        if (pthread_create(&workers[k], NULL, run, (void *) ((long) k)) != 0) {
            exit(-1);
        }
    }
    // The main thread waits until all sub-threads have finished.
    for (long k = 0; k < N_THREADS; k++) {
        pthread_join(workers[k], NULL);
    }
    //
    printf("counter == %ld\n", counter);
    return 0;
}



