# Operating Systems Lab Exercise Report

**Student Name:** Suhasini Madala 
**Register Number:** BT2024043  
**Course:** EGC 301P - Operating Systems Lab  
**Total Exercises:** 52

---

## Table of Contents

1. [Section I: File Management (Q1–Q18)](#section-i-file-management)
2. [Section II: Process Management (Q19–Q30)](#section-ii-process-management)
3. [Section III: System V IPC Mechanisms (Q31–Q52)](#section-iii-system-v-ipc-mechanisms)
4. [Compilation Instructions](#compilation-instructions)
5. [File Structure](#file-structure)

---

## Section I: File Management

### Q1 – Creating Different File Types (1.c)
**Concepts:** `symlink()`, `link()`, `mkfifo()`, `mknod()`  
- Creates a soft link (symbolic link) using `symlink()` — a shortcut pointing to a file path.
- Creates a hard link using `link()` — another directory entry pointing to the same inode.
- Creates FIFOs using both `mkfifo()` library function and `mknod()` system call with `S_IFIFO`.
- **Key Difference:** Deleting the original file breaks a soft link but not a hard link.

### Q2 – Background Process & /proc Exploration (2.c)
**Concepts:** `getpid()`, `/proc` filesystem, `opendir()`, `readdir()`  
- Reads process info from `/proc/<pid>/status`, `/proc/<pid>/cmdline`, `/proc/<pid>/stat`.
- Traverses `/proc` directory to list all running processes with PID, name, and state.
- Runs indefinitely in background for user to inspect `/proc/<pid>/` directory.

### Q3 – File Creation with creat() (3.c)
**Concepts:** `creat()` system call, file descriptors  
- Creates a file and prints its file descriptor value (typically 3, since 0=stdin, 1=stdout, 2=stderr).
- `creat()` is equivalent to `open(file, O_WRONLY|O_CREAT|O_TRUNC, mode)`.

### Q4 – Opening File with O_EXCL (4.c)
**Concepts:** `open()`, `O_RDWR`, `O_EXCL`, `O_CREAT`  
- Opens an existing file in read-write mode.
- Demonstrates `O_EXCL|O_CREAT` — fails if file exists (atomic creation check).
- Useful for lock files and preventing race conditions.

### Q5 – Continuous File Creation (5.c)
**Concepts:** `open()`, `/proc/pid/fd`, file descriptor table  
- Creates 5 files per iteration in an infinite loop WITHOUT closing them.
- Demonstrates file descriptor table growth observable at `/proc/<pid>/fd`.
- Shows the per-process open file limit (`ulimit -n`).

### Q6 – I/O with System Calls (6.c)
**Concepts:** `read()`, `write()`, `STDIN_FILENO`, `STDOUT_FILENO`  
- Takes input from STDIN and displays on STDOUT using only `read()`/`write()` — no printf/scanf.
- Demonstrates low-level I/O without C library buffering.

### Q7 – File Copying (cp emulation) (7.c)
**Concepts:** `open()`, `read()`, `write()`, `close()`, `O_TRUNC`  
- Copies file1 to file2 using a 4096-byte buffer loop.
- Usage: `./a.out source_file dest_file`
- Handles partial writes and read errors.

### Q8 – Read File Line by Line (8.c)
**Concepts:** `open()`, `read()`, `O_RDONLY`, character-by-character reading  
- Opens file read-only, reads character by character, accumulates into lines at `\n`.
- Displays each line with its line number.

### Q9 – File Information with stat() (9.c)
**Concepts:** `stat()`, `struct stat`, `S_ISREG()`, `ctime()`  
- Prints: inode, hard links count, UID, GID, size, block size, block count, atime, mtime, ctime.
- Identifies file type using `S_IS*` macros.

### Q10 – File Write and lseek (10.c)
**Concepts:** `lseek()`, `SEEK_CUR`, `SEEK_SET`, file holes, `od` command  
- Writes 10 bytes, seeks forward 10 bytes (creating a hole), writes 10 more bytes.
- The hole contains null bytes (`\0`) — observable with `od -c`.
- Demonstrates sparse file optimization.

### Q11 – File Descriptor Duplication (11.c)
**Concepts:** `dup()`, `dup2()`, `fcntl(F_DUPFD)`, shared file table entry  
- Duplicates fd using all three methods and writes with each.
- All duplicated fds share the same file offset → sequential writes.
- `dup()` = lowest available fd; `dup2()` = specified fd; `fcntl()` = lowest fd ≥ N.

### Q12 – Determining File Opening Mode (12.c)
**Concepts:** `fcntl(F_GETFL)`, `O_ACCMODE`, file status flags  
- Uses `fcntl()` with `F_GETFL` to retrieve file status flags.
- Decodes access mode using `O_ACCMODE` mask: `O_RDONLY`, `O_WRONLY`, `O_RDWR`.
- Also detects `O_APPEND`, `O_NONBLOCK`, `O_SYNC` flags.

### Q13 – Waiting for STDIN with select() (13.c)
**Concepts:** `select()`, `fd_set`, `FD_ZERO`, `FD_SET`, `FD_ISSET`, `struct timeval`  
- Waits for STDIN input for 10 seconds using `select()`.
- Returns immediately when data is available, or prints timeout message.

### Q14 – File Type Identification (14.c)
**Concepts:** `lstat()`, `S_ISREG`, `S_ISDIR`, `S_ISLNK`, `S_ISFIFO`  
- Takes filename from command line, identifies type using `lstat()` (doesn't follow symlinks).
- Recognizes: regular file, directory, symlink, FIFO, char device, block device, socket.

### Q15 – Environmental Variables (15.c)
**Concepts:** `extern char **environ`, `getenv()`  
- Iterates through `environ` array to display all environment variables.
- Also shows specific variables (HOME, PATH, USER, SHELL) via `getenv()`.

### Q16 – Mandatory Locking (16.c)
**Concepts:** `fcntl()`, `struct flock`, `F_WRLCK`, `F_RDLCK`, `F_SETLK`, `F_UNLCK`  
- **(a)** Write lock (exclusive) — prevents all other access to the locked region.
- **(b)** Read lock (shared) — allows concurrent reads but blocks writes.
- Requires `chmod 2644` on the file and filesystem mounted with `mand` option.

### Q17 – Ticket Reservation with Write Lock (17/)
**Files:** `17a.c` (store ticket), `17b.c` (lock, read, increment)  
- `17a.c` initializes a ticket file with number 100.
- `17b.c` acquires write lock with `F_SETLKW` (blocking), reads number, increments, writes back.
- Simulates concurrent ticket booking with race condition prevention.

### Q18 – Record Locking (18.c)
**Concepts:** `fcntl()` with byte-range locking, `struct Record`  
- Creates 3 fixed-size records in a binary file.
- Locks individual records by calculating byte offset: `record_num * sizeof(Record)`.
- **(a)** Write lock on one record for modification; **(b)** Read lock for safe concurrent reading.

---

## Section II: Process Management

### Q19 – Process States (19.c)
**Concepts:** `fork()`, `sleep()`, `kill(SIGSTOP)`, process states R/S/T  
- **(a)** Running (R): child in busy loop (CPU-bound computation).
- **(b)** Sleeping (S): child calls `sleep()` (interruptible sleep).
- **(c)** Stopped (T): parent sends `SIGSTOP` to child.
- Verify with: `ps -o pid,stat,comm`

### Q20 – Fork and Print PIDs (20.c)
**Concepts:** `fork()`, `getpid()`, `getppid()`, `wait()`  
- `fork()` returns child PID to parent, 0 to child, -1 on error.
- Both processes print their PID and the return value of `fork()`.

### Q21 – Parent & Child Write to File (21.c)
**Concepts:** Shared file descriptor after `fork()`, interleaved writes  
- File opened BEFORE `fork()` — both processes share the same file table entry and offset.
- Writes from both are sequential (not overwriting) due to shared offset.

### Q22 – Zombie Process (22.c)
**Concepts:** Zombie state (Z), `wait()`, process table entry  
- Child exits immediately; parent does NOT call `wait()`.
- Child becomes zombie — entry stays in process table until parent reads exit status.
- Observable with `ps -eo pid,stat,comm | grep defunct`.

### Q23 – Orphan Process (23.c)
**Concepts:** Orphan, init/systemd adoption, `getppid()`  
- Parent exits immediately; child sleeps then checks `getppid()`.
- After parent dies, child's PPID changes to 1 (init) or system subreaper.

### Q24 – waitpid for Specific Child (24.c)
**Concepts:** `waitpid()`, `WIFEXITED()`, `WEXITSTATUS()`, multiple children  
- Creates 3 children with different sleep durations and exit codes.
- Parent uses `waitpid(pid, ...)` to wait for each specific child in desired order.

### Q25 – Executing with exec (25.c)
**Concepts:** `execl()`, `fork()`, command-line argument passing  
- **(a)** Child replaces itself with `ls -l` using `execl()`.
- **(b)** Passes user-provided command-line argument to `echo` via `execl()`.
- `exec` never returns on success; original process image is replaced.

### Q26 – All exec Variants for ls -Rl (26.c)
**Concepts:** `execl`, `execlp`, `execle`, `execv`, `execvp`  
- **l** = list args; **v** = vector (array) args; **p** = search PATH; **e** = custom environment.
- Each variant demonstrated in a separate forked child process.

### Q27 – Real-Time Priority Range (27.c)
**Concepts:** `sched_get_priority_max()`, `sched_get_priority_min()`  
- Prints min/max priority for SCHED_FIFO, SCHED_RR, and SCHED_OTHER.
- Linux: FIFO/RR range is typically 1–99; OTHER is always 0.

### Q28 – Nice Value (28.c)
**Concepts:** `nice()`, `getpriority()`, `PRIO_PROCESS`  
- Gets current nice value, then modifies it using `nice(increment)`.
- Nice range: -20 (highest priority) to 19 (lowest). Only root can go negative.

### Q29 – Scheduling Policy (29.c)
**Concepts:** `sched_getscheduler()`, `sched_setscheduler()`, `struct sched_param`  
- Gets current policy (default: SCHED_OTHER/CFS).
- Attempts to change to SCHED_FIFO and SCHED_RR (requires root/sudo).

### Q30 – Daemon Process (30.c)
**Concepts:** `fork()`, `setsid()`, `umask()`, `chdir("/")`, daemon lifecycle  
- Classic daemon creation: fork → setsid → chdir → umask → close std fds.
- Daemon checks time every 30 seconds and executes a task at the target time.

---

## Section III: System V IPC Mechanisms

### Q31 – Pipe Basics (31.c)
**Concepts:** `pipe()`, `pipefd[0]` (read), `pipefd[1]` (write)  
- Creates pipe, writes data through write end, reads from read end, displays content.

### Q32 – Parent to Child via Pipe (32.c)
**Concepts:** `pipe()` + `fork()`, closing unused ends  
- Pipe created BEFORE fork. Parent writes, child reads.
- Each process closes its unused pipe end to prevent deadlocks.

### Q33 – Two-Way Pipe Communication (33.c)
**Concepts:** Two pipes for bidirectional communication  
- Pipe1: parent→child. Pipe2: child→parent.
- Full-duplex communication between parent and child.

### Q34 – ls -l | wc with dup/dup2/fcntl (34.c)
**Concepts:** Pipe + fd redirection for shell pipeline emulation  
- **(a)** `dup()`: close stdout, dup pipe write end to get fd 1.
- **(b)** `dup2()`: directly assign pipe fd to stdout.
- **(c)** `fcntl(F_DUPFD)`: duplicate to lowest fd ≥ 0.

### Q35 – Directory Count with dup2 (35.c)
**Concepts:** Multi-stage pipeline: `ls -l | grep ^d | wc -l`  
- Three child processes connected by two pipes, all using `dup2()` for redirection.

### Q36 – FIFO Creation (36.c + 36.sh)
**Concepts:** `mknod` command, `mkfifo` command, `strace`, `mknod()` syscall, `mkfifo()` function  
- Shell script: creates FIFOs with commands, compares efficiency via strace.
- C program: creates FIFOs with `mknod()` syscall and `mkfifo()` library function.

### Q37 – One-Way FIFO Communication (37/)
**Files:** `37_writer.c`, `37_reader.c`  
- Writer sends messages through a named FIFO; reader receives and displays them.
- Run reader first (blocks until writer connects), then writer.

### Q38 – Two-Way FIFO Communication (38/)
**Files:** `38_program1.c`, `38_program2.c`  
- Two FIFOs for bidirectional chat between two independent processes.
- Program1 sends first, Program2 receives first (alternating turns).

### Q39 – select() on FIFO (39.c)
**Concepts:** `select()` with FIFO fd, `O_RDONLY|O_NONBLOCK`, timeout  
- Opens FIFO non-blocking, uses `select()` to wait 10 seconds for data.

### Q40 – Process File Limit & Pipe Size (40.c)
**Concepts:** `sysconf(_SC_OPEN_MAX)`, `getrlimit(RLIMIT_NOFILE)`, `F_GETPIPE_SZ`  
- Prints max open files (soft/hard limits) and pipe buffer size (typically 64KB on Linux).

### Q41 – Message Queue Creation (41.c)
**Concepts:** `ftok()`, `msgget()`, `IPC_CREAT`  
- Generates IPC key with `ftok()`, creates message queue, prints key and msqid.

### Q42 – Message Queue Info (42.c)
**Concepts:** `msgctl(IPC_STAT)`, `struct msqid_ds`, `struct ipc_perm`  
- Prints: permissions, UID/GID, last send/receive time, change time, queue size, message count, max bytes, PIDs.

### Q43 – Sending Messages (43.c)
**Concepts:** `msgsnd()`, `struct msgbuf`, message types  
- Sends 3 messages with different types (1 and 2) to the queue. Verify with `ipcs -q`.

### Q44 – Receiving Messages (44.c)
**Concepts:** `msgrcv()`, flag=0 (blocking), `IPC_NOWAIT` (non-blocking), `ENOMSG`  
- **(a)** Blocking receive: waits until message available.
- **(b)** Non-blocking: returns immediately with `ENOMSG` if no message.

### Q45 – Change Queue Permissions (45.c)
**Concepts:** `msgctl(IPC_SET)`, modifying `msg_perm.mode`  
- Reads current permissions, changes to 0644, verifies the change.

### Q46 – Remove Message Queue (46.c)
**Concepts:** `msgctl(IPC_RMID)`  
- Removes message queue immediately. Equivalent to `ipcrm -q <msqid>`.

### Q47 – Shared Memory Operations (47.c)
**Concepts:** `shmget()`, `shmat()`, `shmdt()`, `shmctl(IPC_RMID)`, `SHM_RDONLY`  
- **(a)** Creates shared memory, writes data. **(b)** Attaches read-only, verifies write fails.
- **(c)** Detaches with `shmdt()`. **(d)** Removes with `shmctl(IPC_RMID)`.

### Q48 – Semaphore Creation (48.c)
**Concepts:** `semget()`, `semctl(SETVAL)`, `union semun`, binary vs counting  
- **(a)** Binary semaphore initialized to 1 (mutex behavior).
- **(b)** Counting semaphore initialized to 3 (3 concurrent accesses).

### Q49 – Semaphore Implementation (49/)
**Files:** `49a.c` (ticket), `49b.c` (shared memory), `49c.c` (counting), `49d.c` (remove)  
- **(a)** Ticket reservation with semaphore (P/V operations) instead of file locking.
- **(b)** Shared memory protection from concurrent writes using binary semaphore.
- **(c)** 5 processes competing for 2 resources using counting semaphore (value=2).
- **(d)** Semaphore removal with `semctl(IPC_RMID)`.

### Q50 – Deadlock (50.c)
**Concepts:** Circular wait, two semaphores, `fork()`  
- Process 1 locks Sem0, then tries Sem1. Process 2 locks Sem1, then tries Sem0.
- Neither can proceed → classic circular wait deadlock. Must kill with Ctrl+C.

### Q51 – Socket Communication (51/)
**Files:** `51_server.c`, `51_client.c`  
- Server: `socket()` → `bind()` → `listen()` → `accept()` → `read()`/`write()`.
- Client: `socket()` → `connect()` → `write()`/`read()`.
- Interactive chat over TCP (AF_INET, SOCK_STREAM) on port 8080.

### Q52 – Concurrent Server (52/)
**Files:** `52a.c` (fork-based), `52b.c` (pthread-based)  
- **(a)** Fork: each client gets a child process. Parent continues accepting. SIGCHLD handler prevents zombies.
- **(b)** Pthread: each client gets a detached thread. Lighter than fork, shared memory space.
- Both are echo servers on port 9090/9091.

---

## Compilation Instructions

All programs are written for **Linux** and should be compiled with `gcc`:

```bash
# Single-file programs (e.g., Q1-Q16, Q18-Q35, etc.)
gcc -o output_name filename.c

# Q52b requires pthread library
gcc -o 52b 52/52b.c -lpthread

# Shell script (Q36)
chmod +x 36.sh
./36.sh
```

### Batch Compilation Script
```bash
#!/bin/bash
# Compile all programs at once
cd final/

# Single files
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 39 40 41 42 43 44 45 46 47 48 50; do
    gcc -o "prog_$i" "$i.c" && echo "✓ $i.c" || echo "✗ $i.c FAILED"
done

# Multi-file programs
gcc -o prog_17a 17/17a.c && gcc -o prog_17b 17/17b.c
gcc -o prog_37w 37/37_writer.c && gcc -o prog_37r 37/37_reader.c
gcc -o prog_38p1 38/38_program1.c && gcc -o prog_38p2 38/38_program2.c
gcc -o prog_49a 49/49a.c && gcc -o prog_49b 49/49b.c
gcc -o prog_49c 49/49c.c && gcc -o prog_49d 49/49d.c
gcc -o prog_51s 51/51_server.c && gcc -o prog_51c 51/51_client.c
gcc -o prog_52a 52/52a.c
gcc -o prog_52b 52/52b.c -lpthread

echo "Compilation complete!"
```

---

## File Structure

```
final/
├── 1.c  to  16.c          (Q1–Q16: File Management)
├── 17/
│   ├── 17a.c               (Store ticket number)
│   └── 17b.c               (Lock and increment ticket)
├── 18.c to 35.c            (Q18–Q35: Record Locking, Process Mgmt, Pipes)
├── 36.c                    (FIFO creation - system calls)
├── 36.sh                   (FIFO creation - shell commands)
├── 37/
│   ├── 37_writer.c         (FIFO one-way: writer)
│   └── 37_reader.c         (FIFO one-way: reader)
├── 38/
│   ├── 38_program1.c       (FIFO two-way: program 1)
│   └── 38_program2.c       (FIFO two-way: program 2)
├── 39.c to 48.c            (Q39–Q48: IPC - MsgQ, SHM, Semaphores)
├── 49/
│   ├── 49a.c               (Ticket with semaphore)
│   ├── 49b.c               (Shared memory protection)
│   ├── 49c.c               (Counting semaphore)
│   └── 49d.c               (Remove semaphore)
├── 50.c                    (Deadlock demonstration)
├── 51/
│   ├── 51_server.c         (Socket server)
│   └── 51_client.c         (Socket client)
├── 52/
│   ├── 52a.c               (Concurrent server - fork)
│   └── 52b.c               (Concurrent server - pthread)
├── OS_Lab_Report.md         (This report)
├── Lab Exercise Submission Template.pdf
└── List of OS Lab Exercises_EGC 301P (1).pdf
```

**Total: 61 source files (60 .c + 1 .sh) covering all 52 exercises.**

---

## Key System Calls Summary

| Category | System Calls Used |
|----------|------------------|
| **File I/O** | `open`, `close`, `read`, `write`, `creat`, `lseek`, `stat`, `lstat` |
| **File Descriptors** | `dup`, `dup2`, `fcntl` |
| **Links & FIFO** | `symlink`, `link`, `mkfifo`, `mknod` |
| **Process** | `fork`, `exec*`, `wait`, `waitpid`, `getpid`, `getppid`, `exit` |
| **Signals** | `kill`, `signal` |
| **Scheduling** | `nice`, `sched_getscheduler`, `sched_setscheduler`, `sched_get_priority_max/min` |
| **Pipes** | `pipe`, `select` |
| **Message Queues** | `msgget`, `msgsnd`, `msgrcv`, `msgctl` |
| **Shared Memory** | `shmget`, `shmat`, `shmdt`, `shmctl` |
| **Semaphores** | `semget`, `semop`, `semctl` |
| **Sockets** | `socket`, `bind`, `listen`, `accept`, `connect` |
| **Threads** | `pthread_create`, `pthread_detach` |

---

*Report prepared by Suhasini Madala (BT2024043) for EGC 301P Operating Systems Lab.*
