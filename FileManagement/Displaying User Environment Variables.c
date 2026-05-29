/* Program Number: 15
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program displays the environmental variables
   of the user using the extern char **environ variable.
*/

#include <stdio.h>
#include <stdlib.h>

/* environ is a global variable maintained by the C library.
   It is a NULL-terminated array of strings, where each string
   is in the format "KEY=VALUE". It contains all environment
   variables inherited by this process from its parent (the shell). */
extern char **environ;

int main() {
    int i = 0;

    printf("===== User Environmental Variables =====\n\n");

    /* Loop through the environ array until we hit NULL.
       Each entry is a string like "HOME=/home/user" or "PATH=/usr/bin:..." */
    while (environ[i] != NULL) {
        printf("[%3d] %s\n", i + 1, environ[i]);
        i++;
    }

    printf("\n--- Total environment variables: %d ---\n", i);

    /* We can also access specific variables using getenv() */
    printf("\n--- Some Common Variables ---\n");
    printf("HOME = %s\n", getenv("HOME") ? getenv("HOME") : "not set");
    printf("PATH = %s\n", getenv("PATH") ? getenv("PATH") : "not set");
    printf("USER = %s\n", getenv("USER") ? getenv("USER") : "not set");
    printf("SHELL= %s\n", getenv("SHELL") ? getenv("SHELL") : "not set");

    return 0;
}
