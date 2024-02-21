#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

void initFileWithFilename(int fd, const char *filename) {
    // Start with an additional line break for a better display of the file (for cat *.out)
    dprintf(fd, "\n%s:\n", filename);
}


void validateAndAmendFilename(char **filename) {
    /*
    This function is to check if the input was given as a file
    if not, then make it a .out file
    */
	const char *extension = ".out";
	const char *dot = strrchr(*filename, '.'); 
    
    
    if (dot && dot != *filename) {
        
        if (strcmp(dot, extension) == 0) {
            return;
        }
    } else {
        
        char *newFilename = malloc(strlen(*filename) + strlen(extension) + 1); // Reserve space for the null terminator
        if (!newFilename) {
            perror("Failed to allocate new filename");
            exit(EXIT_FAILURE);
        }
        strcpy(newFilename, *filename);
        strcat(newFilename, extension); // Append ".out"
        *filename = newFilename; // Update the filename with the extended name
    }
}


void writeProcessInfo(const char *prefix, int fd) {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    //getting the process id and the parent process id for each process
    dprintf(fd, "[%s] getpid() = %d, getppid() = %d\n", prefix, pid, ppid);

}


int main(int argc, char *argv[]) {
    pid_t pid;
    int fd;
    char *filename = argc > 1 ? argv[1] : "output.out";
    validateAndAmendFilename(&filename);
    
    if (argc < 2) {
        printf("No file specified. Default filename used: %s\n", filename);
    } else {
        printf("Created file: %s\n", filename);
    }

    // O_TRUNC to truncate the existing content if re-created.
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Adding the name of the data and the end time.
    initFileWithFilename(fd, filename);

    if ((pid = fork()) == -1) {
        perror("Failed to fork");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        writeProcessInfo("CHILD", fd);
    } else {  // Parent process
        writeProcessInfo("PARENT", fd);
        wait(NULL); // Wait for the child process to finish
    }

    close(fd); // Make sure the file descriptor is closed properly

    return EXIT_SUCCESS;
}

