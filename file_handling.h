#ifndef A1_FILE_HANDLING_H
#define A1_FILE_HANDLING_H

#define MAX_FILENAME 256
#define NUM_CHARS 26

int get_num_files(const char* directory);
void get_file_names(char** filesArray, const char* directory);
long calc_file_counts(char inFile[], long char_stats[], const char* directory);
void process_files(char *files[], int numFiles, int startFile, long char_counts[], const char* dir);
void free_files(char *files[], int numFiles);

#endif //A1_FILE_HANDLING_H
