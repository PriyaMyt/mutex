#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

volatile sig_atomic_t interrupted = 0;

void sigint_handler(int signum) {
    interrupted = 1;
}

int main() {
    struct timespec req, rem;
    
    // Registering signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    // Define the time to sleep (2 seconds and 500 milliseconds)
    req.tv_sec = 2;
    req.tv_nsec = 500000000;

    printf("Sleeping for 2 seconds and 500 milliseconds...\n");

    // Loop until the sleep time is completed or interrupted
    while (nanosleep(&req, &rem) && !interrupted) {
        // If nanosleep is interrupted by a signal, print remaining time and retry
        printf("Sleep interrupted. Remaining time: %ld seconds %ld nanoseconds.\n", rem.tv_sec, rem.tv_nsec);
        req = rem; // Adjust the sleep time to the remaining time
    }

    if (!interrupted) {
        printf("Sleep completed.\n");
    }

    return 0;
}
