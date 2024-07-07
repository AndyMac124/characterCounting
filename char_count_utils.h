//
// Created by Andrew Mckenzie on 7/7/2024.
//

#ifndef A1_CHAR_COUNT_UTILS_H
#define A1_CHAR_COUNT_UTILS_H

int valid_directory(char *dir_name);
int parse_args(int argc, char *argv[], int *nprocs);
void report_totals(long char_counts[]);

#endif //A1_CHAR_COUNT_UTILS_H
