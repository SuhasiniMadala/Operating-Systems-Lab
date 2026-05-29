/* Program Number: 26
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program executes 'ls -Rl' using five different
   exec system calls: execl, execlp, execle, execv, execvp.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    /* All exec functions replace the current process with a new program.
       The differences are in how they specify the program and arguments:
       
       l = arguments as a LIST (individual args)
       v = arguments as a VECTOR (array)
       p = searches PATH for the program
       e = pass custom ENVIRONMENT variables */

    /* (a) execl: path + list of args + NULL */
    printf("===== (a) execl =====\n");
    pid = fork();
    if (pid == 0) {
        /* execl(path, arg0, arg1, ..., NULL)
           - Full path to the executable required
           - Arguments listed one by one
           - Must end with NULL */
        execl("/bin/ls", "ls", "-R", "-l", NULL);
        perror("execl");
        exit(1);
    }
    wait(NULL);

    /* (b) execlp: searches PATH + list of args + NULL */
    printf("\n===== (b) execlp =====\n");
    pid = fork();
    if (pid == 0) {
        /* execlp(file, arg0, arg1, ..., NULL)
           - 'p' means it searches the PATH environment variable
           - No need to give full path, just the command name */
        execlp("ls", "ls", "-R", "-l", NULL);
        perror("execlp");
        exit(1);
    }
    wait(NULL);

    /* (c) execle: path + list of args + NULL + environment */
    printf("\n===== (c) execle =====\n");
    pid = fork();
    if (pid == 0) {
        /* execle(path, arg0, arg1, ..., NULL, envp)
           - 'e' means we pass our OWN environment variables
           - envp is a NULL-terminated array of "KEY=VALUE" strings */
        char *env[] = {"PATH=/bin:/usr/bin", "HOME=/tmp", NULL};
        execle("/bin/ls", "ls", "-R", "-l", NULL, env);
        perror("execle");
        exit(1);
    }
    wait(NULL);

    /* (d) execv: path + args as array */
    printf("\n===== (d) execv =====\n");
    pid = fork();
    if (pid == 0) {
        /* execv(path, argv)
           - 'v' means arguments are in an array (vector)
           - Array must be NULL-terminated */
        char *args[] = {"ls", "-R", "-l", NULL};
        execv("/bin/ls", args);
        perror("execv");
        exit(1);
    }
    wait(NULL);

    /* (e) execvp: searches PATH + args as array */
    printf("\n===== (e) execvp =====\n");
    pid = fork();
    if (pid == 0) {
        /* execvp(file, argv)
           - 'v' = array of arguments
           - 'p' = search PATH for the program */
        char *args[] = {"ls", "-R", "-l", NULL};
        execvp("ls", args);
        perror("execvp");
        exit(1);
    }
    wait(NULL);

    printf("\n===== All exec variants demonstrated =====\n");
    return 0;
}
