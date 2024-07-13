#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

#include "files.h"

#define MAX_FILENAME 256

int calc_num_files(const char* directory) {
        DIR* dp;
        struct dirent* dirp;
        int count = 0;

        dp = opendir(directory);

        while ((dirp = readdir(dp)) != NULL) {
                if (dirp->d_type == DT_REG && dirp->d_name[0] != '.') {
                        count++;
                }
        }

        closedir(dp);

        return count;
}

void get_file_names(char** filesArray, const char* directory){
        DIR* dp;
        struct dirent* dirp;
        dp = opendir(directory);
        int index = 0;

        while((dirp = readdir(dp)) != NULL) {
                if (dirp->d_type == DT_REG && dirp->d_name[0] != '.') {
                        filesArray[index] = (char*)malloc((strlen
                                                                   (dirp->d_name) + 1) * sizeof(char));
                        strcpy(filesArray[index], dirp->d_name);
                        index++;
                }
        }

        closedir(dp);
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
