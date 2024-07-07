#include <unistd.h>
#include <stdbool.h>
#include "ring_process.h"


int make_trivial_ring(void) {
        int fd[2];

        if (pipe(fd) == -1) {
                return(-1);
        }

        if ((dup2(fd[0], STDIN_FILENO) == -1) ||
            (dup2(fd[1], STDOUT_FILENO) == -1)) {
                return(-3);
        }

        return(0);
}

int add_new_node(int *pid) {
        int fd[2];

        if (pipe(fd) == -1) {
                return(-1);
        }

        if ((*pid = fork()) == -1) {
                return(-2);
        }

        if (*pid > 0 && dup2(fd[1], STDOUT_FILENO) < 0) {
                return(-3);
        }

        if (*pid == 0 && dup2(fd[0], STDIN_FILENO) < 0) {
                return(-4);
        }

        if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
                return(-5);
        }

        return(0);
}


void send_subtotal(long char_counts[]){
        write(STDOUT_FILENO, char_counts, 26 * sizeof(long));
}

void read_subtotal(long char_counts[], bool to_add){
        long temp_counts[26] = {0};
        read(STDIN_FILENO, temp_counts, 26 * sizeof(long));
        for (int i = 0; i < 26; i++) {
                if (to_add) {
                        char_counts[i] += temp_counts[i];
                } else {
                        // When final count is received by Mother process
                        char_counts[i] = temp_counts[i];
                }
        }
}
