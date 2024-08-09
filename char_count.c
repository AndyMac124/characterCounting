/*H*
 * FILENAME: char_count.c
 *
 * AUTHOR: Andrew McKenzie
 * UNE EMAIL: amcken33@myune.edu.au
 * STUDENT NUMBER: 220263507
 *
 * PURPOSE:
 * Perform frequency analysis on a subdirectory using inter process
 * communication and out the totals and relative histogram.
 *
 * This program creates a ring of processes and accesses a subdirectory
 * containing text files and divides them between the ring of processes.
 * The processes count the frequency of the basic A-Z characters without
 * case sensitivity and pass them around the ring until the original
 * mother process gets them and prints the totals and a histogram
 * of their frequencies.
 *
 * COMPILING: The included makefile can be run with the 'make' command.
 *
 * RUNNING: The program is run by the following:
 *          ./char_count <int - number of processes> <directory>
 *
 * NOTES:
 * The directory should be a subdirectory. If forward or
 * backslashes are added the program will remove these in an attempt
 * to save the situation.
 *
 * As per the Linux Kernel C programming guide:
 * - Function names use snake case for emphasis.
 * - Variables use camel case for brevity.
 * - Global variables, constants and macros use snake case and are upper case.
 * - Everything except function declarations use K&R style braces.
 * - Functions use Allman style braces.
 *
 * For more detailed instructions, please see the README.md
 *H*/

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
int main(int argc, char *argv[])
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

        // Each process must calculate its own files frequencies.
        process_files(files, filesEach, startFile,
                      charCounts, TEXT_DIR);

        // If not the Mother process read and send counts
        if (i > 1) {
                read_subtotal(charCounts, true);
                send_subtotal(charCounts);
        }

        /* If is the Mother process send counts, wait for counts to get back,
         * then wait for all processes to finish and report the totals.
         */
        if (i == 1) {
                send_subtotal(charCounts);
                read_subtotal(charCounts, false);
                for (int j = 1; j < nProcs; j++) {
                        wait(NULL);
                }
                report_totals(charCounts);
        }

        // Freeing allocated memory
        free_files(files, numFiles);

        exit(EXIT_SUCCESS);
}
