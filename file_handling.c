/*H*
 * FILENAME: file_handling.c
 *
 * AUTHOR: Andrew McKenzie
 * UNE EMAIL: amcken33@myune.edu.au
 * STUDENT NUMBER: 220263507
 *
 * PURPOSE: This file supports the char_count.c program by containing
 * the necessary file handling utilities.
 *
 *H*/

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

#include "file_handling.h"


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

        if (!dp) {
                perror("Failed to open directory");
                exit(EXIT_FAILURE);
        }

        while ((dirp = readdir(dp)) != NULL) {
                /* Confirming file type and ignoring hidden files.
                 * This was an issue in MacOS but not Linux, so I've
                 * left it in as a precaution.
                 */
                if (dirp->d_type == DT_REG && dirp->d_name[0] != '.') {
                        count++;
                }
        }

        if (closedir(dp) == -1) {
                perror("Failed to close directory");
                exit(EXIT_FAILURE);
        }

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

        if (!dp) {
                perror("Failed to open directory");
                exit(EXIT_FAILURE);
        }

        while((dirp = readdir(dp)) != NULL) {
                /* Confirming file type and ignoring hidden files.
                 * This was an issue in MacOS but not Linux, so I've
                 * left it in as a precaution.
                 */
                if (dirp->d_type == DT_REG && dirp->d_name[0] != '.') {
                        // Allocating the space for the name
                        filesArray[index] = (char*)malloc((strlen
                                (dirp->d_name) + 1) * sizeof(char));
                        // Checking memory was allocated
                        if (filesArray[index] == NULL) {
                                perror("Couldn't allocate memory for files");
                                closedir(dp);
                                exit(EXIT_FAILURE);
                        }
                        // Copying the name into the index
                        if (strcpy(filesArray[index], dirp->d_name) == NULL) {
                                perror("Failed to copy string");
                                free(filesArray[index]);
                                closedir(dp);
                                exit(EXIT_FAILURE);
                        }
                        index++;
                }
        }

        if (closedir(dp) == -1) {
                perror("Failed to close directory");
                exit(EXIT_FAILURE);
        }
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
long calc_file_counts(char inFile[], long charStats[], const char* directory)
{
        char fileName[MAX_FILENAME] = ""; // Allocating array space
        strcpy(fileName, directory);  // Copying directory name into array
        strcat(fileName, "/"); // Adding slash for subdirectory
        strcat(fileName, inFile); // Adding file name

        FILE* file = fopen(fileName, "r");
        if (file == NULL) {
                perror("Failed to open specified file");
                exit(EXIT_FAILURE);
        }

        char c;
        long charCount = 0;

        while((c = fgetc(file))) {
                if (c == EOF) {
                        break;
                } else if ((tolower(c)-'a') >= 0
                        && (tolower(c)-'a') < NUM_CHARS) {
                        charStats[tolower(c) - 'a'] += 1;
                } else {
                        continue;
                }
                charCount += 1;
        }

        if (fclose(file) == EOF) {
                perror("Failed to close file");
                exit(EXIT_FAILURE);
        }

        return charCount;
}


/**
 * process_files() - Processes all files allocated to given process
 * @arg1: Array of file names
 * @arg2: Number of files for this process
 * @arg3: First file index for this process
 * @arg4: Array to store frequencies in
 * @arg5: Directory name to go inside
 *
 * Function will start from the index given by start file and progress
 * through the given number of files calling the calc_file_counts and
 * store the frequencies in the given array.
 *
 * Return: void
 */
void process_files(char *files[], int numFiles, int startFile,
                   long charCounts[], const char* dir)
{
        for (int j = 0; j < numFiles; j++) {
                calc_file_counts(files[startFile + j],
                                 charCounts, dir);
        }
}


/**
 * free_files() - Frees allocated memory for the given files array
 * @arg1: Files array to free
 * @arg2: length of array
 *
 * Function to free all memory allocated for the files array.
 *
 * Return: Void
 */
void free_files(char *files[], int numFiles)
{
        for (int l = 0; l < numFiles; l++) {
                if (files[l] != NULL) {
                        free(files[l]);
                }
        }
}
