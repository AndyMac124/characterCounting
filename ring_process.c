/**
 * File: ring_process.c
 * Author: Andrew McKenzie
 * UNE Email: amcken33@myune.edu.au
 * Student Number: 220263507
 */

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#include "ring_process.h"

/**
 * make_trivial_ring() - Creates pipe
 *
 * Function creates a basic pipe and redirects standard in
 * and standard out.
 *
 * Reference: Function from the assignment description.
 *
 * Return: Integer representing the outcome.
 *         Zero on success, negative on failure.
 */
int make_trivial_ring(void)
{
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

/**
 * add_new_node() - Adds new node to the ring.
 * @arg1: Process ID to fork from
 *
 * Function inserts process into ring and redirects parents standard out
 * into child and childs standard in to the parent.
 *
 * Reference: Function from the assignment description.
 *
 * Return: Integer representing the outcome.
 *         Zero on success, negative on failure.
 */
int add_new_node(int *pid)
{
        int fd[2];

        if (pipe(fd) == -1) {
                return(-1);
        }

        if ((*pid = fork()) == -1) {
                return(-2);
        }

        // Parent process pipe write to standard out
        if (*pid > 0 && dup2(fd[1], STDOUT_FILENO) < 0) {
                return(-3);
        }

        // Child process pipe read to standard in
        if (*pid == 0 && dup2(fd[0], STDIN_FILENO) < 0) {
                return(-4);
        }

        // Closing file descriptors
        if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
                return(-5);
        }

        return(0);
}

/**
 * send_subtotal() - Writes to standard out
 * @arg1: array of character frequency
 * @arg2: Number of characters in analysis
 *
 * Function writes the given array of longs (representing the character
 * frequencies in this program) to standard out.
 *
 * Return: void
 */
void send_subtotal(long charCounts[], int num_chars)
{
        int written;
        written = write(STDOUT_FILENO,
                        charCounts, num_chars * sizeof(long));
        if (written < 0) {
                perror("Error writing to standard out");
        }
}

/**
 * read_subtotal() - Reads from standard in
 * @arg1: This processes contribution to the total character frequency counts
 * @arg2: Boolean of whether this process needs to add its count.
 *        Used for when the 'Mother' process receives the final count.
 * @arg3: Number of characters in analysis
 *
 * Return: void
 */
void read_subtotal(long charCounts[], bool toAdd, int numChars)
{
        int bytes;

        long tempCounts[26] = {0};
        bytes = read(STDIN_FILENO, tempCounts, numChars * sizeof(long));

        if (bytes < 0) {
                perror("Error reading from standard in");
        }

        for (int i = 0; i < numChars; i++) {
                if (toAdd) {
                        charCounts[i] += tempCounts[i];
                } else {
                        // When final count is received by Mother process
                        charCounts[i] = tempCounts[i];
                }
        }
}
