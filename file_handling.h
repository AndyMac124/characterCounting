#ifndef A1_FILE_HANDLING_H
#define A1_FILE_HANDLING_H

int get_num_files(const char* directory);
void get_file_names(char** filesArray, const char* directory);
long calc_file_counts(char inFile[], long char_stats[], const char* directory);

#endif //A1_FILE_HANDLING_H
