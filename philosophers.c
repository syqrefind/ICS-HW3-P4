// This is to test the output of the solution

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "chopsticks.h"

pthread_mutex_t mutex_for_output;
// # of steps and max # of steps
int n_steps, max_steps = 0;
// Counter of eating for each philosopher
int eat_count[I];

// This test function prints out information for tracing: philosopher_id as i, message as s.
void review(int i, char *s) {
    
    pthread_mutex_lock(&mutex_for_output);
    
    
    if (strcmp (s, "eating") == 0)
        eat_count [i]++;
    
    
    if (n_steps++ > max_steps) {
        
    // Keep the program running while holding chopsticks
        if (strcmp(s,"thinking") == 0) {
            
            pthread_mutex_unlock(&mutex_for_output);
            pthread_exit(0);
        }
    }
    pthread_mutex_unlock(&mutex_for_output);
}

// Worker function for philosopher
void * philosopher(void *arg) {
  int self = *(int *) arg;
  for (;;) {
     review(self,"thinking");
     chopsticks_pickup(self);
     review(self,"eating");
     chopsticks_putdown(self);
  }
}

int main() {
    int i;
    // Philosopher number
    pthread_t t_id[I];
    // Table position number
    int no[I];
    pthread_attr_t attr;

    // Initialize counter of EAT for each philosopher to be 0
    for (i = 0; i < I; i++)
        eat_count [i] = 0;

    
    pthread_mutex_init(&mutex_for_output, NULL);
    // Initialize the Monitor `chopsticks`
    chopsticks_init();
    fprintf(stdout,"Please type in the total # of steps to run the test: "); fflush(stdout);
    fscanf(stdin,"%d",&max_steps);
    pthread_attr_init (&attr);
    // Use 4 kernel threads for this process for test purpose
    pthread_setconcurrency (4);
    // Set system contention for UNIX
    pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
    

    // Start the philosopher threads
    for (i = 0; i < I; i++) {
        no[i] = i;
        pthread_create(&t_id[i], NULL, philosopher, (int *) &no[i]);
    }

    // Wait for the threads to shutdown
    for (i = 0; i < I; i++)
        pthread_join(t_id[i], NULL);

    
    for (i = 0; i < I; i++) {
        fprintf (stderr, "Philospher %d ate %d times\n", i, eat_count [i]);
    }

    
    return 0;
}



