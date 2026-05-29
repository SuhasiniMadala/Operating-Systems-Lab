/* Program Number: 40
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program prints the maximum number of files that
   can be opened within a process and the size of a pipe (circular buffer).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>  /* For getrlimit */
#include <fcntl.h>
#include <errno.h>

int main() {
    printf("===== Process File Limit and Pipe Size =====\n\n");

    /* Method 1: Using sysconf() to get max open files.
       _SC_OPEN_MAX returns the maximum number of files a process can open.
       This value is also called RLIMIT_NOFILE. */
    long max_files = sysconf(_SC_OPEN_MAX);
    printf("Maximum files per process (sysconf): %ld\n", max_files);

    /* Method 2: Using getrlimit() for more detailed info.
       getrlimit(RLIMIT_NOFILE, ...) returns both the soft and hard limits.
       - Soft limit: current effective limit (can be increased up to hard limit)
       - Hard limit: absolute maximum (only root can increase it) */
    struct rlimit rl;
    if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
        printf("  Soft limit: %lu\n", (unsigned long)rl.rlim_cur);
        printf("  Hard limit: %lu\n", (unsigned long)rl.rlim_max);
    }

    /* Determine pipe size by creating a pipe and checking its capacity.
       We use fcntl with F_GETPIPE_SZ to get the pipe buffer size.
       On Linux, the default pipe size is typically 65536 bytes (64 KB). */
    printf("\n--- Pipe Size ---\n");
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

#ifdef F_GETPIPE_SZ
    /* F_GETPIPE_SZ is Linux-specific (available since kernel 2.6.35) */
    int pipe_size = fcntl(pipefd[0], F_GETPIPE_SZ);
    if (pipe_size != -1) {
        printf("Pipe buffer size (F_GETPIPE_SZ): %d bytes (%d KB)\n",
               pipe_size, pipe_size / 1024);
    } else {
        perror("fcntl F_GETPIPE_SZ");
    }
#else
    /* Alternative: determine pipe size by writing until it blocks.
       We set the pipe to non-blocking mode and count bytes written. */
    printf("F_GETPIPE_SZ not available. Measuring by writing...\n");
    int flags = fcntl(pipefd[1], F_GETFL);
    fcntl(pipefd[1], F_SETFL, flags | O_NONBLOCK);

    char byte = 'A';
    long bytes_written = 0;
    while (write(pipefd[1], &byte, 1) == 1) {
        bytes_written++;
    }
    printf("Pipe buffer size (measured): %ld bytes (%ld KB)\n",
           bytes_written, bytes_written / 1024);
#endif

    close(pipefd[0]);
    close(pipefd[1]);

    /* Also print PIPE_BUF - the max atomic write size for pipes */
    printf("PIPE_BUF (max atomic write): %ld bytes\n", (long)_PC_PIPE_BUF);
    long pipe_buf = fpathconf(pipefd[0] >= 0 ? pipefd[0] : 0, _PC_PIPE_BUF);
    /* Note: pipe already closed, use pathconf instead */
    pipe_buf = pathconf("/", _PC_PIPE_BUF);
    printf("PIPE_BUF (pathconf): %ld bytes\n", pipe_buf);

    return 0;
}
