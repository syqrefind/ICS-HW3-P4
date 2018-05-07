// interface for chopsticks routines, i.e., the `Monitor` ADT that includes methods (public "member functions" & private internal funcitons
// Number of philosophers
#define I 5
// Initializor of Monitor
extern void chopsticks_init(void);
// pickup()
extern void chopsticks_pickup(int i);
// putdown()
extern void chopsticks_putdown(int i);
// End the monitor
extern void chopsticks_finalize(void);

  
