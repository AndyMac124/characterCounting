#ifndef A1_RING_PROCESS_H
#define A1_RING_PROCESS_H

#include <stdbool.h>

#define NUM_BYTES (26 * sizeof(long)) // Number of bytes passed between procs

int make_trivial_ring(void);
int add_new_node(int *pid);
void send_subtotal(long charCounts[]);
void read_subtotal(long charCounts[], bool toAdd);

#endif //A1_RING_PROCESS_H
