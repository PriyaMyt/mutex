#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void sigint_handler(int signum) {
    printf("Interrupt signal received. Exiting...\n");
    exit(1);
}

int main() {
    struct timespec req, rem;
    
    // Registering signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    // Define the time to sleep (2 seconds and 500 milliseconds)
    req.tv_sec = 2;
    req.tv_nsec = 500000000;

    printf("Sleeping for 2 seconds and 500 milliseconds...\n");

    // Call nanosleep() with the defined time
    int result = nanosleep(&req, &rem);

    // Check if nanosleep() encountered an error
    if (result == -1) {
        printf("nanosleep() failed.\n");
        return 1;
    }

    // If nanosleep() was interrupted, print remaining time
    if (result == 0 && rem.tv_sec > 0) {
        printf("Sleep interrupted. Remaining time: %ld seconds %ld nanoseconds.\n", rem.tv_sec, rem.tv_nsec);
    } else {
        printf("Sleep completed.\n");
    }

    return 0;
}
