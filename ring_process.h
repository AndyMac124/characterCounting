#ifndef A1_RING_PROCESS_H
#define A1_RING_PROCESS_H

#include <stdbool.h>

int make_trivial_ring(void);
int add_new_node(int *pid);
void send_subtotal(long charCounts[], int numChars);
void read_subtotal(long charCounts[], bool toAdd, int numChars);

#endif //A1_RING_PROCESS_H
