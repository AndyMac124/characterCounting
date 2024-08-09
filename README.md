# COSC330 A1

#### Author: Andrew McKenzie
#### Student ID: 220263507

## Overview:
This program is designed to perform a frequency analysis of basic A-Z 
characters in all files within a given directory except for 
the `.` `..` and hidden files.

It will create a ring of processes based on the given user input and then access 
the given directory and divide the files between the number of process. 
Each process will count the frequency of the A-Z characters and pass the totals 
around so the 'mother' process can get the final total and print it to the 
terminal with a histogram of the ratios.

---
### Running the program:
To run this program, you can first use the `make` utility to compile the program
and then `./char_count <number of processes> <directory name>` in 
the command line e.g. `./char_count 4 text_files`

---
### Process creation:
The processes are created in a ring fashion with stdout and stdin being 
duplicated to the pipes file descriptors.

The program will not create more processes than the number of files to not 
waste system resources.

---
### File processing distribution:
Files are divided up among the processes equally with any remainder being 
allocated to the last process.

---
### Error Handling
- I considered asking for correct user input if the number of processes or 
directory were not correct but decided that given this occurs at the initial
program start point it was better to not start the program and just output 
the error message.

- In the situation the user doesn't purely enter the directory name 
e.g. `text_files` but instead enters `/text_files` or `text_files/` the 
program will remove these forward slashes since they aren't valid in 
directory names.

- I have also accounted for the `.` `..` and hidden files. These won't be 
run through the frequency analysis.