#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

#include "files.h"

#define MAX_FILENAME 256

/**
 * get_num_files() - Calculates the number of files in given directory
 * @arg1: pathname of directory to search inside
 *
 * Return: Integer number representing the total number of files
 */
int get_num_files(const char* directory)
{
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

/**
 * get_file_names() - Extracts filenames from inside directory
 * @arg1: Pointer to start of char array for holding the names
 * @arg2: Pointer to the directory pathname to get the filenames from
 *
 *  Function goes through the given directory, any file that is not hidden
 *  or the special '.' and '..' files will have its name recorded into the
 *  given array.
 *
 * Return: void (pass by reference)
 */
void get_file_names(char** filesArray, const char* directory)
{
        DIR* dp;
        struct dirent* dirp;
        dp = opendir(directory);
        int index = 0;

        while((dirp = readdir(dp)) != NULL) {
                if (dirp->d_type == DT_REG && dirp->d_name[0] != '.') {
                        filesArray[index] = (char*)malloc((strlen(dirp->d_name) + 1) * sizeof(char));
                        strcpy(filesArray[index], dirp->d_name);
                        index++;
                }
        }

        closedir(dp);
}

/**
 * calc_file_counts() - Counts frequency of each char in given file
 * @arg1: File name to access
 * @arg2: Array for the numeric count of each character
 * @arg3: Directory the file is inside
 *
 * Function accesses given file, reads the file while checking if each
 * character is one of the 26 between A and Z and increments that count
 * in the given char_stats array.
 *
 * Reference: Function slightly modified from assignment description
 *
 * Return: The array of numeric count of each character
 */
long calc_file_counts(char inFile[], long char_stats[], const char* directory)
{
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
