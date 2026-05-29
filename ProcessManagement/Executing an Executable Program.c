/* Program Number: 25
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: (a) Execute a program using the exec system call.
   (b) Pass input to an executable program via command-line arguments.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    /* ============================================ */
    /* (a) Execute a program using exec system call.
       exec replaces the current process image with a new program.
       The calling process is completely replaced - it does not return
       to the original code after exec (unless exec fails). */

    printf("===== Part (a): Executing a program using exec =====\n\n");

    pid = fork();
    if (pid == 0) {
        /* Child: replace itself with the 'ls' command.
           execl() takes the path, followed by arguments (argv[0], argv[1], ..., NULL).
           The first argument after the path is conventionally the program name. */
        printf("Child: About to exec 'ls -l'...\n");
        execl("/bin/ls", "ls", "-l", NULL);

        /* If exec succeeds, this line will NEVER execute.
           If we reach here, exec failed. */
        perror("execl failed");
        exit(1);
    }
    wait(NULL);  /* Parent waits for child to finish */

    /* ============================================ */
    /* (b) Pass input to an executable program.
       We demonstrate passing command-line arguments to an exec'd program.
       Usage: ./a.out <name>
       The name argument will be passed to a child that uses it. */

    printf("\n===== Part (b): Passing arguments to exec'd program =====\n\n");

    if (argc < 2) {
        printf("Usage: %s <name>\n", argv[0]);
        printf("Example: %s John\n", argv[0]);
        printf("Skipping part (b) since no name was provided.\n");
    } else {
        pid = fork();
        if (pid == 0) {
            /* Pass the user-provided name to 'echo' command.
               This simulates passing input to an executable. */
            printf("Child: Executing 'echo' with argument: %s\n", argv[1]);
            execl("/bin/echo", "echo", "Hello,", argv[1], "! Welcome to OS Lab.", NULL);
            perror("execl failed");
            exit(1);
        }
        wait(NULL);
    }

    printf("\nParent: All exec demonstrations complete.\n");
    return 0;
}
