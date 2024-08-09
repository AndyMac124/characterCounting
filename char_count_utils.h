#ifndef A1_CHAR_COUNT_UTILS_H
#define A1_CHAR_COUNT_UTILS_H

#define BAR_LENGTH 59 // Max length of bar for the histogram

char* valid_directory(char *dirName);
int parse_args(int argc, char *argv[], int *nprocs);
void report_totals(long charCounts[]);

#endif //A1_CHAR_COUNT_UTILS_H
