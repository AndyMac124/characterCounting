#ifndef A1_RING_PROCESS_H
#define A1_RING_PROCESS_H

#include <stdbool.h>

int make_trivial_ring(void);
int add_new_node(int *pid);
void send_subtotal(long char_counts[], int num_chars);
void read_subtotal(long char_counts[], bool to_add, int num_chars);

#endif //A1_RING_PROCESS_H
