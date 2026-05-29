/* Program Number: 2
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program runs indefinitely in the background and
   traverses the /proc directory to extract relevant process information
   from the corresponding proc directories.
*/

#include <stdio.h>      /* For printf, fopen, fgets, fclose */
#include <stdlib.h>     /* For exit, atoi */
#include <unistd.h>     /* For getpid, sleep */
#include <string.h>     /* For strlen, strstr */
#include <dirent.h>     /* For opendir, readdir, closedir */
#include <ctype.h>      /* For isdigit */

int main() {
    /* Get our own process ID so we can find our info in /proc */
    pid_t my_pid = getpid();
    printf("My PID is: %d\n", my_pid);
    printf("Run this program in background using: ./a.out &\n");
    printf("Then check /proc/%d/ directory for process info.\n\n", my_pid);

    /* Buffer to hold file paths and content */
    char path[256];
    char buffer[1024];
    FILE *fp;

    /* Read and display our own process information from /proc/self
       /proc/self is a symbolic link to /proc/<our_pid> */

    /* 1. Read the process status file - contains detailed process info */
    printf("===== /proc/%d/status =====\n", my_pid);
    snprintf(path, sizeof(path), "/proc/%d/status", my_pid);
    fp = fopen(path, "r");
    if (fp != NULL) {
        /* Read the status file line by line and print each line */
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(fp);
    } else {
        perror("fopen status");
    }

    /* 2. Read the command line that started this process */
    printf("\n===== /proc/%d/cmdline =====\n", my_pid);
    snprintf(path, sizeof(path), "/proc/%d/cmdline", my_pid);
    fp = fopen(path, "r");
    if (fp != NULL) {
        /* cmdline uses null bytes as separators, so we read byte by byte */
        int ch;
        while ((ch = fgetc(fp)) != EOF) {
            if (ch == '\0')
                printf(" ");  /* Replace null bytes with spaces for readability */
            else
                printf("%c", ch);
        }
        printf("\n");
        fclose(fp);
    }

    /* 3. Read the stat file - contains process statistics */
    printf("\n===== /proc/%d/stat =====\n", my_pid);
    snprintf(path, sizeof(path), "/proc/%d/stat", my_pid);
    fp = fopen(path, "r");
    if (fp != NULL) {
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        printf("\n");
        fclose(fp);
    }

    /* 4. List all running processes by traversing /proc directory */
    printf("\n===== All Running Processes =====\n");
    printf("%-10s %-20s %-10s\n", "PID", "Name", "State");
    printf("----------------------------------------------\n");

    DIR *proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("opendir /proc");
        exit(1);
    }

    struct dirent *entry;
    /* Read each entry in /proc directory */
    while ((entry = readdir(proc_dir)) != NULL) {
        /* Process directories in /proc are named with numbers (PIDs).
           Skip entries that are not numbers (like 'net', 'sys', etc.) */
        int is_pid = 1;
        int i;
        for (i = 0; entry->d_name[i] != '\0'; i++) {
            if (!isdigit(entry->d_name[i])) {
                is_pid = 0;
                break;
            }
        }

        if (is_pid && strlen(entry->d_name) > 0) {
            /* Read the status file for this process */
            char proc_name[256] = "unknown";
            char proc_state[64] = "unknown";

            snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
            fp = fopen(path, "r");
            if (fp != NULL) {
                /* Parse the status file to extract process name and state */
                while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                    if (strncmp(buffer, "Name:", 5) == 0) {
                        sscanf(buffer, "Name:\t%s", proc_name);
                    } else if (strncmp(buffer, "State:", 6) == 0) {
                        /* Extract state info after the tab character */
                        sscanf(buffer, "State:\t%[^\n]", proc_state);
                    }
                }
                fclose(fp);
            }
            printf("%-10s %-20s %-10s\n", entry->d_name, proc_name, proc_state);
        }
    }
    closedir(proc_dir);

    /* Now run indefinitely in the background so the user can inspect
       our /proc/<pid> directory at any time */
    printf("\n--- Program now running indefinitely. Press Ctrl+C to stop. ---\n");
    printf("Inspect /proc/%d/ while the program is running.\n", my_pid);

    while (1) {
        sleep(60);  /* Sleep for 60 seconds in each iteration to save CPU */
    }

    return 0;
}
