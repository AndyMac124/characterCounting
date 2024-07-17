/*H*
 * FILENAME: char_count_utils.c
 *
 * AUTHOR: Andrew McKenzie
 * UNE EMAIL: amcken33@myune.edu.au
 * STUDENT NUMBER: 220263507
 *
 * PURPOSE: This file supports the char_count.c program by containing
 * the various utilities required by the program. Currently, the
 * subdirectory validation, command line argument parsing and reporting
 * the totals back to the terminal.
 *
 *H*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

#include "char_count_utils.h"


/**
 * valid_directory() - Checks if argument is a valid directory.
 * @arg1: directory name to check.
 *
 * Simply checks it can open the given directory.
 * Will remove forward slashes from start or end as an effort
 * to save the program from an error.
 *
 * Return: Int, zero on success, negative on failure.
 */
char* valid_directory(char *dirName)
{
        // If the given directory started with a slash, remove it.
        if (dirName[0] == '/') {
                dirName = dirName + 1;
        }

        int length = strlen(dirName);

        // If the given directory ended with a slash, remove it.
        if (dirName[length-1] == '/') {
                dirName[length - 1] = '\0';
        }

        DIR *dir = opendir(dirName);

        if (dir) {
                if (closedir(dir) == -1) {
                        perror("Failed to close directory");
                        exit(EXIT_FAILURE);
                }
                return dirName;
        } else {
                perror("Could not access directory");
                exit(EXIT_FAILURE);
        }
}


/**
 * parse_args() - Checks args are correct
 * @arg1: Number of arguments in argv
 * @arg2: Array of arguments
 * @arg3: Number of processes requested
 *
 * Checks for correct number of args, number of processes is a valid int,
 * and that the given directory is valid and can be opened.
 *
 * Return: Int, zero on success, negative on failure.
 */
int parse_args(int argc, char *argv[], int *nprocs)
{
        // Checking number of args, and number of processes is int.
        if ((argc != 3) || ((*nprocs = atoi(argv[1])) <= 0)) {
                fprintf(stderr, "Usage: %s <number of processes> "
                                "<directory>\n", argv[0]);
                return(-1);
        }

        return(0);
}


/**
 * report_totals() - prints character counts to terminal.
 * @arg1: Array of character frequency counts to report.
 * @arg2: Number of characters in analysis
 *
 * Finds the character with the highest count and uses it to
 * create a ratio of all other characters in the histogram.
 *
 * Return: void
 */
void report_totals(long charCounts[], int numChars)
{
        // Getting the character with the highest count
        long maxCount = 0;
        for (int i = 0; i < numChars; i++) {
                if (charCounts[i] > maxCount) {
                        maxCount = charCounts[i];
                }
        }

        for (int j = 0; j < numChars; j++) {
                fprintf(stderr, "Process 1 got char %c: %ld \t| ", 'a' + j,
                        charCounts[j]);
                // Char counts to double for increased accuracy.
                double ratio = (double)charCounts[j] / maxCount * BARLENGTH;
                int histogramBar = (int)ceil(ratio);
                for (int k = 0; k < histogramBar; k++) {
                        fprintf(stderr, "*");
                }
                fprintf(stderr, "\n");
        }
}
