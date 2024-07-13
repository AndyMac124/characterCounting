/*
 * File: char_count_utils.c
 * Author: Andrew McKenzie
 * UNE Email: amcken33@myune.edu.au
 * Student Number: 220263507
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define BARLENGTH 60

/**
 * valid_directory() - Checks if argument is a valid directroy.
 * @arg1: directory name to check.
 *
 * Simply checks it can open the given directory.
 *
 * Return: Int, zero on success, negative on failure.
 */
int valid_directory(char *dir_name)
{
        DIR *dir = opendir(dir_name);
        if (dir) {
                closedir(dir);
                return (0);
        }
        return (-1);
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
                fprintf (stderr, "Usage: %s nprocs\n", argv[0]);
                return(-1);
        }

        if (valid_directory(argv[2]) < 0) {
                perror("Could not access directory");
                exit(EXIT_FAILURE);
        }

        return(0);
}

/**
 * report_totals() - prints character counts to terminal.
 * @arg1: Array of character frequency counts to report.
 *
 * Finds the character with the highest count and uses it to
 * create a ratio of all other characters in the histogram.
 *
 * Return: void
 */
void report_totals(long char_counts[])
{
        // Getting the character with highest count
        long max_count = 0;
        for (int i = 0; i < 26; i++) {
                if (char_counts[i] > max_count) {
                        max_count = char_counts[i];
                }
        }

        for (int j = 0; j < 26; j++) {
                fprintf(stderr, "Process 1 got char %c: %ld \t| ", 'a' + j,
                        char_counts[j]);
                // Char counts to double for increased accuracy.
                int histogram_bar = (int)(((double)char_counts[j]/max_count) *
                                          BARLENGTH);
                for (int k = 0; k < histogram_bar; k++) {
                        fprintf(stderr, "*");
                }
                fprintf(stderr, "\n");
        }
}
