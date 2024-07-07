//
// Created by Andrew Mckenzie on 7/7/2024.
//
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

#include "files.h"

int calc_num_files(const char* directory) {
        DIR* dir;
        struct dirent* file;
        int count = 0;

        dir = opendir(directory);

        while ((file = readdir(dir)) != NULL) {
                if (file->d_type == DT_REG && file->d_name[0] != '.') {
                        count++;
                }
        }

        closedir(dir);

        return count;
}

void get_file_names(char** filesArray, const char* directory){
        DIR* dir;
        struct dirent* file;
        dir = opendir(directory);
        int index = 0;

        while((file = readdir(dir)) != NULL) {
                if (file->d_type == DT_REG && file->d_name[0] != '.') {
                        filesArray[index] = (char*)malloc((strlen
                                                                   (file->d_name) + 1) * sizeof(char));
                        strcpy(filesArray[index], file->d_name);
                        index++;
                }
        }

        closedir(dir);
}

long calc_file_counts(char inFile[], long char_stats[], const char* directory) {
        char cur_fname[MAX_FILENAME] = "";
        strcpy(cur_fname, directory);
        strcat(cur_fname, "/");
        strcat(cur_fname, inFile);

        FILE* file = fopen(cur_fname, "r");
        if (file == NULL) {
                perror("Failed to open specified file");
                exit(1);
        }
        char c;
        long char_count = 0;

        while((c = fgetc(file))) {
                if (c == EOF) {
                        break;
                } else if ((tolower(c)-'a') >= 0 && (tolower(c)-'a') < 26) {
                        char_stats[tolower(c) - 'a'] += 1;
                } else {
                        continue;
                }
                char_count += 1;
        }
        fclose(file);
        return char_count;
}
