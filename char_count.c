#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_FILENAME 256

int make_trivial_ring(void) {
        int fd[2];

        if (pipe(fd) == -1) {
                return(-1);
        }

        if ((dup2(fd[0], STDIN_FILENO) == -1) ||
                (dup2(fd[1], STDOUT_FILENO) == -1)) {
                return(-3);
        }

        return(0);
}

int valid_directory(char *dir_name){
        DIR *dir = opendir(dir_name);
        if (dir) {
                closedir(dir);
                return (0);
        }
        return (-1);
}

int parse_args(int argc, char *argv[], int *nprocs) {
        if ((argc != 3) || ((*nprocs = atoi(argv[1])) <= 0)) {
                fprintf (stderr, "Usage: %s nprocs\n", argv[0]);
                return(-1);
        }

        if (valid_directory(argv[2]) < 0) {
                perror("Could not access directory");
                exit(EXIT_FAILURE);
        }

        return(0);
}

int add_new_node(int *pid) {
        int fd[2];

        if (pipe(fd) == -1) {
                return(-1);
        }

        if ((*pid = fork()) == -1) {
                return(-2);
        }

        if (*pid > 0 && dup2(fd[1], STDOUT_FILENO) < 0) {
                return(-3);
        }

        if (*pid == 0 && dup2(fd[0], STDIN_FILENO) < 0) {
                return(-4);
        }

        if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
                return(-5);
        }

        return(0);
}

int num_files(const char* directory) {
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

void file_names(char** filesArray, const char* directory){
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

long procFileCounts(char inFile[], long char_stats[], const char* directory) {
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

void send_subtotal(long char_counts[]){
        write(STDOUT_FILENO, char_counts, 26 * sizeof(long));
}

void read_subtotal(long char_counts[], bool to_add){
        long temp_counts[26] = {0};
        read(STDIN_FILENO, temp_counts, 26 * sizeof(long));
        for (int i = 0; i < 26; i++) {
                if (to_add) {
                        char_counts[i] += temp_counts[i];
                } else {
                        // When final count is received by Mother process
                        char_counts[i] = temp_counts[i];
                }
        }
}

void report_totals(long char_counts[]) {
        long max_count = 0;
        const int BARLENGTH = 80;
        for (int i = 0; i < 26; i++) {
                if (char_counts[i] > max_count) {
                        max_count = char_counts[i];
                }
        }

        for (int j = 0; j < 26; j++) {
                fprintf(stderr, "Process 1 got char %c: %ld \t| ", 'a' + j,
                       char_counts[j]);
                int histogram_bar = (int)(((double)char_counts[j]/max_count) *
                        BARLENGTH);
                for (int k = 0; k < histogram_bar; k++) {
                        fprintf(stderr, "*");
                }
                fprintf(stderr, "\n");
        }
}

int main(int argc, char *argv[]) {
        int i;          // Number of this process, starting with 1
        int childpid;   // Indicates process should spawn another
        int nprocs;     // Total number of processes in ring
        long char_counts[26] = {0};

        // Checking if args were correct
        if (parse_args(argc, argv, &nprocs) < 0) {
                exit(EXIT_FAILURE);
        }

        // Setting the number of processes in the ring
        nprocs = atoi(argv[1]);

        // Assigning the directory for the text files
        const char* TEXT_DIR = argv[2];

        // Calculating the number of files
        int numFiles = num_files(TEXT_DIR);

        if (numFiles < nprocs) {
                nprocs = numFiles;
        }

        // Number of files each
        int numFilesEach = numFiles / nprocs;

        // Remainder, if last child in ring, must also do the remainder.
        int remainder = numFiles % nprocs;

        // Allocating length of array
        //char **files = malloc(numFiles * sizeof(char*));
        char *files[numFiles];

        // Setting initial array values
        //memset(files, 0, numFiles * sizeof(char*));
        memset(files, 0, sizeof(files));

        // Adding file names to the array
        file_names(files, TEXT_DIR);

        // Making trivial ring with 1 process
        if (make_trivial_ring() < 0) {
                perror("Could not add new node to ring");
                exit(EXIT_FAILURE);
        }

        // Adding more processes based on requested amount
        for (i = 1; i < nprocs; i++) {
                if (add_new_node(&childpid) < 0) {
                        perror("Could not add new node to ring");
                        exit(EXIT_FAILURE);
                }
                if (childpid) {
                        break;
                }
        }

        /* RING PROCESS */
        // Number of files to process for this node
        int startFile = numFilesEach * (i - 1);

        // The final node takes on the remainder from the division
        if (i == nprocs) {
                numFilesEach += remainder;
        }

        // Each process must calculate its files, then wait for parent to
        // pipe through.
        for (int j = 0; j < numFilesEach; j++) {
                procFileCounts(files[startFile + j],
                               char_counts, TEXT_DIR);
        }

        /* all processes */
        if (i > 1) {
                read_subtotal(char_counts, true);
                send_subtotal(char_counts);
        }

        if (i == 1) {
                send_subtotal(char_counts);
                read_subtotal(char_counts, false);
                for (int k = 1; k < nprocs; k++) {
                        wait(NULL);
                }
                report_totals(char_counts);
        }

        for (int l = 0; l < numFiles; l++) {
                if (files[l] != NULL) {
                        free(files[l]);
                }
        }
        //free(files);

        exit(EXIT_SUCCESS);
}
