#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#define MAX_LENGTH 256

int main()
{

    FILE* cmd = _popen("cmd", "w");

    //Entering path to the solution

    char dir[MAX_LENGTH];
    printf("Enter the path to the solution (for example: C:\\Users\\Shine\\source\\repos\\Git Auto Uploader\nIf your path have spaces, take the way in double quotes\n\n");
    fgets(dir, MAX_LENGTH, stdin);
    dir[strlen(dir) - 1] = '\0';
    fprintf(cmd, "cd %s\n", dir);

    //Git initialization and adding files to the repository (ignoring .vs and x64 folders)

    fprintf(cmd, "git init %s\n", dir);
    fprintf(cmd, "echo **/.vs/**>> %s\\.gitignore\n", dir);
    fprintf(cmd, "echo **/x64/**>> %s\\.gitignore\n", dir);
    fprintf(cmd, "git add %s\n", dir);

    //Commit name input

    char commit[MAX_LENGTH];
    printf("Enter the commit name\n");
    fgets(commit, MAX_LENGTH, stdin);
    commit[strlen(commit) - 1] = '\0';
    fprintf(cmd, "git commit -m \"%s\"\n", commit);

    //Entering the link to the repository

    char remote[MAX_LENGTH];
    printf("Enter the link to the repository (for example: https://github.com/MatteMate/Git-Auto-Uploader)\n");
    scanf_s("%s", remote, MAX_LENGTH);
    fprintf(cmd, "git remote add origin %s\n", remote);

    //Push to the repository and pause

    fprintf(cmd, "git push -u origin master\n");
    fprintf(cmd, "pause");

    return 0;
}