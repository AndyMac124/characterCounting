#ifndef A1_CHAR_COUNT_UTILS_H
#define A1_CHAR_COUNT_UTILS_H

#define BARLENGTH 40

int valid_directory(char *dir_name);
int parse_args(int argc, char *argv[], int *nprocs);
void report_totals(long char_counts[]);

#endif //A1_CHAR_COUNT_UTILS_H
