/*
 * File: char_count.c
 * Author: Andrew McKenzie
 * UNE Email: amcken33@myune.edu.au
 * Student Number: 220263507
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "ring_process.h"
#include "file_handling.h"
#include "char_count_utils.h"

/**
 * main() - Main function for character frequency counting program.
 * @arg1: Number of args from the terminal.
 * @arg2: Array of the args from the terminal.
 *
 * This program relies on three core components:
 *      1. File handling - Accessing a given directory and storing file names.
 *      2. Ring of processes - Creates a ring of processes to process files.
 *      3. Character Frequency counting - The processes count the frequency
 *      of the basic characters (A-Z) and pass the totals back around to the
 *      mother process to print out.
 *
 * Return: Int, zero on success, non-zero on failure.
 */
int main (int argc, char *argv[])
{
        int i;          // Number of this process, starting with 1
        int childpid;   // Indicates process should spawn another
        int nprocs;     // Total number of processes in ring
        long char_counts[26] = {0}; // Holds count of the 26 characters

        // Checking if the given args were correct
        if (parse_args(argc, argv, &nprocs) < 0) {
                exit(EXIT_FAILURE);
        }

        // Setting the number of processes in the ring
        nprocs = atoi(argv[1]);

        // Assigning the directory for the text files
        const char* TEXT_DIR = argv[2];

        // Calculating the number of files
        int numFiles = get_num_files(TEXT_DIR);

        // If we have less files than process,
        // reassign processes to number of files.
        if (numFiles < nprocs) {
                nprocs = numFiles;
        }

        // Number of files each process will handle
        int numFilesEach = numFiles / nprocs;
        // The last child in ring, must also do the remainder.
        int remainder = numFiles % nprocs;

        // Allocating array to store each file name
        char *files[numFiles];
        // Setting initial array values
        memset(files, 0, sizeof(files));
        // Adding file names to the array
        get_file_names(files, TEXT_DIR);

        // Making trivial ring with 1 process
        if (make_trivial_ring() < 0) {
                perror("Could not add new node to ring");
                exit(EXIT_FAILURE);
        }

        // Adding more processes based on requested amount
        for (i = 1; i < nprocs; i++) {
                if (add_new_node(&childpid) < 0) {
                        perror("Could not add new node to ring");
                        exit(EXIT_FAILURE);
                }
                if (childpid) {
                        break;
                }
        }

        /* RING PROCESS */
        // Number of files to process for this node
        int startFile = numFilesEach * (i - 1);

        // The final node takes on the remainder from the division
        if (i == nprocs) {
                numFilesEach += remainder;
        }

        // Each process must calculate its files, then wait for parent to
        // pipe through.
        for (int j = 0; j < numFilesEach; j++) {
                calc_file_counts(files[startFile + j],
                               char_counts, TEXT_DIR);
        }

        /* all processes */
        // If not the Mother process read and send counts
        if (i > 1) {
                read_subtotal(char_counts, true);
                send_subtotal(char_counts);
        }

        /* If is the Mother process send counts, wait for counts to get back,
         * then wait for all processes to finish and report the totals.
         */
        if (i == 1) {
                send_subtotal(char_counts);
                read_subtotal(char_counts, false);
                for (int k = 1; k < nprocs; k++) {
                        wait(NULL);
                }
                report_totals(char_counts);
        }

        // Freeing allocated memory
        for (int l = 0; l < numFiles; l++) {
                if (files[l] != NULL) {
                        free(files[l]);
                }
        }

        exit(EXIT_SUCCESS);
}
