#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

void initFileWithFilename(int fd, const char *filename) {
    char message[256];
    int message_len = snprintf(message,sizeof(message),"\n%s:\n",filename);

    if(write(fd,message,message_len) < message_len) {
	perror("write");
	exit(1);}
}


void validateAndAmendFilename(char **filename) {
	const char *extension = ".out";
	const char *dot = strrchr(*filename, '.'); // Search for the last occurrence of '.'

    // Checking if the dot is present and is not the first character in the name
    if (dot && dot != *filename) {
        // If the part after the last dot is not "out", do nothing
        if (strcmp(dot, extension) == 0) {
            return;
        }
    } else {
        // Reserve enough space to append ".out"
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
    char message[256];
    int message_len = snprintf(message,sizeof(message),"[%s] getpid() = %d, getppid() = %d\n", prefix, pid, ppid);
	if(write(fd,message,message_len)<message_len){
		perror("write");
			exit(1);}

}



int main(int argc, char *argv[]) {
    pid_t pid;
    int fd;
    
    char *filename = argc > 1 ? argv[1] : "output.out";
    
    
    
        if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("Usage: ./lab1 filename\n");
        exit(1);
    }
    
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

