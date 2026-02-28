#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#include <string.h> // Include for strlen()

#define MAX_LENGTH 256

int trim_newline(char* text) {
    size_t len = strlen(text);
    if (len == 0) {
        return 0;
    }

    if (text[len - 1] == '\n') {
        text[len - 1] = '\0';
        len--;
    }

    return len > 0;
}

int has_unsafe_shell_chars(const char* text) {
    return strpbrk(text, "\"&|<>^") != NULL;
}

int is_valid_remote_url(const char* remote) {
    return strncmp(remote, "https://github.com/", 19) == 0 ||
           strncmp(remote, "git@github.com:", 15) == 0;
}

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
    if (!trim_newline(dir)) {
        fprintf(stderr, "Path cannot be empty.\n");
        _pclose(cmd);
        return 1;
    }
    if (has_unsafe_shell_chars(dir)) {
        fprintf(stderr, "Path contains unsafe shell characters.\n");
        _pclose(cmd);
        return 1;
    }

    // Change directory
    if (fprintf(cmd, "cd \"%s\"\n", dir) < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }

    // Git initialization and adding files to the repository (ignoring .vs and x64 folders)
    if (fprintf(cmd, "if not exist .git\\ (git init)\n") < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }
    if (fprintf(cmd, "if exist .gitignore (findstr /x /c:\".vs/\" .gitignore >nul || echo .vs/>>.gitignore) else (echo .vs/>.gitignore)\n") < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }
    if (fprintf(cmd, "if exist .gitignore (findstr /x /c:\"x64/\" .gitignore >nul || echo x64/>>.gitignore) else (echo x64/>>.gitignore)\n") < 0) {
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
    if (!trim_newline(commit)) {
        fprintf(stderr, "Commit message cannot be empty.\n");
        _pclose(cmd);
        return 1;
    }
    if (has_unsafe_shell_chars(commit)) {
        fprintf(stderr, "Commit message contains unsafe shell characters.\n");
        _pclose(cmd);
        return 1;
    }

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
    if (!trim_newline(remote)) {
        fprintf(stderr, "Remote URL cannot be empty.\n");
        _pclose(cmd);
        return 1;
    }
    if (has_unsafe_shell_chars(remote)) {
        fprintf(stderr, "Remote URL contains unsafe shell characters.\n");
        _pclose(cmd);
        return 1;
    }
    if (!is_valid_remote_url(remote)) {
        fprintf(stderr, "Remote URL must be a GitHub URL (https://github.com/... or git@github.com:...).\n");
        _pclose(cmd);
        return 1;
    }

    if (fprintf(cmd, "git remote get-url origin >nul 2>nul || git remote add origin %s\n", remote) < 0) {
        perror("Failed to write to command processor");
        _pclose(cmd);
        return 1;
    }

    // Push to the repository
    if (fprintf(cmd, "git push -u origin HEAD\n") < 0) {
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