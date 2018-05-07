//  NOT_USING_MONITOR.c
//  HW3P4
//  Yunqi Shen


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Define 3 states for each philosophter
enum{THINKING, HUNGRY, EATING} state[5];


// Define Condition Variables and the Mutex
pthread_cond_t self[5];
static  pthread_mutex_t  mutex;


void initializor(){ // initialize monitor

    
    // Initialize the Mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init() error");
        exit(1);
    }
    
    
    for(int i = 1; i < 4; i++){
        // Initialize all philosophers to be thinking
        state[i] = THINKING;
    }
}


void test(int i){
 
    pthread_mutex_lock(&mutex);
    // If my left and right neighbors are not eating
    if ((state[(i - 1) % 5] != EATING) && (state[(i + 1) %5] != EATING) && (state[i] == HUNGRY)){
        // `i`'m gonna eat
        state[i] = EATING;
        printf("The %d philosopher is currently eating.\n", i);
        // Signal `HUNGRY` philosophers during `putdown()`; no effect during `pickup()`
        //self[i].signal();
        pthread_cond_signal(&self[i]);
    }
    pthread_mutex_lock(&mutex);
}


// Functions for philosophters
void pickup(int i)
{
    // Ok so I'm hungry now
    state[i] = HUNGRY;
    // Imma try to see if I can eat
    test(i);
    // If I'm unable to eat, wait to be signalled (maybe before that I need to signal my eating neighbors to signal me)
    if(state[i] != EATING){
        //self[i].wait;
        pthread_cond_wait(&self[i], &mutex);
    }
    return;
}


void putdown(int i)
{
    // Now that I'm done eating, I'm start thinking
    state[i] = THINKING;
    // If right neighbor is `HUNGRY` && both of her neighbors are `not EATING`, set her state as `EATING` && wake her up by signaling her `CV`
    test((i+1)%5);
    // Do the same thing for the left neighbor
    
    return;
}


// The above is everything for the monitor.
// The following is to make philosopher randomly feel hungry.
int main(int argc, char ** argv){
    
    srand((int)time(0));
    initializor();
    int a[10];
    // A simple senario to test the problem
    for (int i = 1; i < 10; i++){
        a[i] = rand() % 2;
        if(a[i]){
            state[i] = HUNGRY;
        }
        printf("%d\n", a[i]);
    }
    
    return 0;
}




