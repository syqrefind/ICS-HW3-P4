#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "chopsticks.h"


// Define the left and and right philosopher index for convenience
#define LEFT (i-1)%I
#define RIGHT (i+1)%I


// Define the three states
#define THINKING 1
#define HUNGRY 2
#define EATING 3

// Defind mutex, CV, and state of each philosopher

pthread_cond_t self[I]; // CV for each philosopher
pthread_mutex_t mutex;  // mutex
int state[I];   // state for each philosopher, either 1, 2 or 3


// "Private Functions" not included in header file
int update_state (int i) {
  if (state[i] == HUNGRY
   && state[LEFT] != EATING
   && state[RIGHT] != EATING) {
     state[i] = EATING;
     pthread_cond_signal (&self[i]);
  }
  return 0;
}

// "Public Functions"
void chopsticks_init () {
    int i;
    pthread_mutex_init (&mutex, NULL);
    for (i = 0; i < I; i++) {
        pthread_cond_init (&self[i], NULL);
        state[i] = THINKING;
  }
}

void chopsticks_pickup (int i) {
    pthread_mutex_lock (&mutex);      /* enter cs, lock mutex */
    state[i] = HUNGRY;            /* set philosopher's state to HUNGRY */
    update_state(i);              /* update_state philosopher */
    while (state[i] == HUNGRY)    /* loop while philosopher is hungry */
        pthread_cond_wait (&self[i],&mutex);
    pthread_mutex_unlock (&mutex); /* exit cs, unlock mutex */
}

void chopsticks_putdown (int i) {
    pthread_mutex_lock (&mutex);       /* enter cs, lock mutex */
    state[i] = THINKING;
    update_state (LEFT);           /* update_state neighbors */
    update_state (RIGHT);
    pthread_mutex_unlock (&mutex);     /* exit cs, unlock mutex */
}

