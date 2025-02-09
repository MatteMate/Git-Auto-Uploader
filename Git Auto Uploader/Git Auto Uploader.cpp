#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#include <string.h> // Include for strlen()

#define MAX_LENGTH 256

int main() {
    // Open a pipe to the command processor
    FILE* cmd = _popen("cmd", "w");
    if (!cmd) {
        perror("Failed to open command processor");
        return 1; // Exit with error code
    }

    // Entering path to the solution
    char dir[MAX_LENGTH];
    printf("Enter the path to the solution (for example: C:\\Users\\Shine\\source\\repos\\Git Auto Uploader\n"
           "If your path has spaces, enclose it in double quotes:\n\n");

    if (!fgets(dir, MAX_LENGTH, stdin)) {
        perror("Failed to read path input");
        _pclose(cmd); // Close the pipe before exiting
        return 1;
    }
    dir[strlen(dir) - 1] = '\0'; // Remove newline character

    // Change directory
    if (fprintf(cmd, "cd \"%s\"\n", dir) < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }

    // Git initialization and adding files to the repository (ignoring .vs and x64 folders)
    if (fprintf(cmd, "git init\n") < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }
    if (fprintf(cmd, "echo .vs/ >> %s\\.gitignore\n", dir) < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }
    if (fprintf(cmd, "echo x64/ >> %s\\.gitignore\n", dir) < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }
    if (fprintf(cmd, "git add .\n") < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }

    // Commit name input
    char commit[MAX_LENGTH];
    printf("Enter the commit name:\n");
    if (!fgets(commit, MAX_LENGTH, stdin)) {
        perror("Failed to read commit input");
        _pclose(cmd);
        return 1;
    }
    commit[strlen(commit) - 1] = '\0'; // Remove newline character

    if (fprintf(cmd, "git commit -m \"%s\"\n", commit) < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }

    // Entering the link to the repository
    char remote[MAX_LENGTH];
    printf("Enter the link to the repository (for example: https://github.com/MatteMate/Git-Auto-Uploader):\n");
    if (!fgets(remote, MAX_LENGTH, stdin)) {
        perror("Failed to read remote input");
        _pclose(cmd);
        return 1;
    }
    remote[strlen(remote) - 1] = '\0'; // Remove newline character

    if (fprintf(cmd, "git remote add origin %s\n", remote) < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }

    // Push to the repository
    if (fprintf(cmd, "git push -u origin main\n") < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }

    // Close the pipe
    if (_pclose(cmd) == -1) {
        perror("Failed to close command processor");
        return 1;
    }

    printf("Git operations completed successfully!\n");
    return 0;
}