//
// Created by Andrew Mckenzie on 7/7/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define BARLENGTH 60

int valid_directory(char *dir_name) {
        DIR *dir = opendir(dir_name);
        if (dir) {
                closedir(dir);
                return (0);
        }
        return (-1);
}

int parse_args(int argc, char *argv[], int *nprocs) {
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

void report_totals(long char_counts[]) {
        long max_count = 0;
        for (int i = 0; i < 26; i++) {
                if (char_counts[i] > max_count) {
                        max_count = char_counts[i];
                }
        }

        for (int j = 0; j < 26; j++) {
                fprintf(stderr, "Process 1 got char %c: %ld \t| ", 'a' + j,
                        char_counts[j]);
                int histogram_bar = (int)(((double)char_counts[j]/max_count) *
                                          BARLENGTH);
                for (int k = 0; k < histogram_bar; k++) {
                        fprintf(stderr, "*");
                }
                fprintf(stderr, "\n");
        }
}
