#ifndef A1_FILES_H
#define A1_FILES_H

int get_num_files(const char* directory);
void get_file_names(char** filesArray, const char* directory);
long calc_file_counts(char inFile[], long char_stats[], const char* directory);

#endif //A1_FILES_H
