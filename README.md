# Operating Systems - NTUA course

Semester: 6th (flow Y)

Professor: Panagiotis Tsanakas


# Lab Summaries

This document provides an overview of various lab exercises, focusing on different aspects of programming in C, including file operations, process handling, signals, and network programming.

## [Lab 1: File and Process Operations](./lab1)

**Objective:** Demonstrate file creation, process forking, and writing process-related information into a file.

- Implements functions to ensure filenames have a specific extension and to write process IDs (both parent and child) into a file.
- The program forks a child process, where both parent and child write their process IDs to a file specified by the user or a default file.

## [Lab 2: Signal Handling](./lab2)

**Objective:** Explore UNIX signal handling across multiple processes using `sigaction`.

- Parent and child processes communicate state changes through signals (`SIGUSR1`, `SIGUSR2`, and `SIGTERM`).
- Child processes handle toggling between two states ("gates open" and "gates closed") upon receiving specific signals.
- The parent process manages child processes based on external commands and monitors their status.

## [Lab 3: Inter-process Communication (IPC) with Pipes and Select](./lab3)

**Objective:** Use pipes for IPC and `select()` for handling multiple file descriptors.

- The parent process sends integers to child processes via pipes based on user input or round-robin/random selection.
- Child processes receive integers, modify them, and send them back to the parent.
- Demonstrates robust handling of user commands and child process responses using non-blocking I/O multiplexing with `select()` - (instead the lab's instructions to use `poll()` which is following the same logic).

## [Lab 4: Network Socket Programming](./lab4)

**Objective:** Establish a TCP connection to a server, handle input/output operations, and implement debugging options.

- The application connects to a specified host and port, sends and receives data through the network.
- Provides a mechanism for debugging that shows detailed information about the data being sent and received.
- Implements utility functions to check the presence of spaces in strings and to validate received data.

---

For each lab, compile the code using a C compiler (e.g., `gcc`) and run the executable with appropriate arguments as demonstrated in the provided source files. Each lab is designed to enhance understanding of system-level programming in C by dealing with real-world programming scenarios involving files, processes, signals, and network operations.
