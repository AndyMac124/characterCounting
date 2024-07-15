#ifndef A1_CHAR_COUNT_UTILS_H
#define A1_CHAR_COUNT_UTILS_H

#define BARLENGTH 40

char* valid_directory(char *dirName);
int parse_args(int argc, char *argv[], int *nprocs);
void report_totals(long charCounts[], int numChars);

#endif //A1_CHAR_COUNT_UTILS_H
