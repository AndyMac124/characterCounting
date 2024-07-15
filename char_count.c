#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>

#include "ring_process.h"
#include "file_handling.h"
#include "char_count_utils.h"


/**
 * File: char_count.c
 *
 * Author: Andrew McKenzie
 * UNE Email: amcken33@myune.edu.au
 * Student Number: 220263507
 *
 * Purpose: This program creates a ring of processes and a directory containing
 * text files and divides them between the ring of processes. The processes
 * count the frequency of the basic A-Z characters and pass them around the
 * ring until the original mother process gets them and prints their totals
 * and a histogram of their frequencies.
 *
 * Compiling: The include makefile can be run with the 'make' command.
 *
 * Running: The program is run by the following:
 *          ./char_count <int - number of processes> <directory>
 *
 * For more detailed instructions, please see the README.md
 */



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
        int nProcs;     // Total number of processes in ring
        long charCounts[NUM_CHARS] = {0}; // Holds count of the 26 characters
        int numFiles;   // To store the number of files in directory
        int filesEach; // Number of files for each process to handle

        // Checking if the given args were correct
        if (parse_args(argc, argv, &nProcs) < 0) {
                exit(EXIT_FAILURE);
        }

        // Assigning the directory for the text files
        const char* TEXT_DIR = valid_directory(argv[2]);

        // Calculating the number of files
        numFiles = get_num_files(TEXT_DIR);

        // If more processes than files, adjust processes to equal files
        if (nProcs > numFiles) {
                nProcs = numFiles;
        }

        // Number of files for each process to handle
        filesEach = numFiles / nProcs;

        // Allocating array to store each file name
        char *files[numFiles];

        // Adding file names to the array
        get_file_names(files, TEXT_DIR);

        /* RING PROCESSING */
        // Making trivial ring with 1 process
        if (make_trivial_ring() < 0) {
                perror("Could not add new node to ring");
                exit(EXIT_FAILURE);
        }

        // Adding more processes based on requested amount or number of files
        for (i = 1; i < nProcs; i++) {
                if (add_new_node(&childpid) < 0) {
                        perror("Could not add new node to ring");
                        exit(EXIT_FAILURE);
                }
                if (childpid) {
                        break;
                }
        }

        // Number of files to process for this node
        int startFile = filesEach * (i - 1);

        // The final node takes on the remainder from the division
        if (i == nProcs) {
                filesEach += numFiles % nProcs;
        }

        // Each process must calculate its files frequencies.
        process_files(files, filesEach, startFile,
                      charCounts, TEXT_DIR);

        // If not the Mother process read and send counts
        if (i > 1) {
                read_subtotal(charCounts, true, NUM_CHARS);
                send_subtotal(charCounts, NUM_CHARS);
        }

        /* If is the Mother process send counts, wait for counts to get back,
         * then wait for all processes to finish and report the totals.
         */
        if (i == 1) {
                send_subtotal(charCounts, NUM_CHARS);
                read_subtotal(charCounts, false, NUM_CHARS);
                for (int k = 1; k < nProcs; k++) {
                        wait(NULL);
                }
                report_totals(charCounts, NUM_CHARS);
        }

        // Freeing allocated memory
        free_files(files, numFiles);

        exit(EXIT_SUCCESS);
}
