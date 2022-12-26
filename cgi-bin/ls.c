#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
int main(int argc, char **argv, char **env) {
    if (argc != 2)
        return -1;
    DIR* dir = opendir(argv[1]);
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        return -1;
    } else {
        return -1;
    }
    char *ret;
    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }
    pid_t pid;
    pid = fork();
    if (pid == -1)
        return 1;
    if (pid == 0) {
        fd[1] = open(".ls_cgi", O_CREAT | O_TRUNC | O_RDWR, 0777);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        char *pythonArgs[] = {"ls", argv[1],NULL};
        execve("/bin/ls", pythonArgs, NULL);
        exit(0);
    } else {
        wait(0);
        FILE *stream;
        char *line = NULL;
        size_t len = 0;
        stream = fopen(".ls_cgi", "r");
        if (stream == NULL) {
            return -1;
        }
        printf("<!DOCTYPE html>\n<html>\n<body>\n<h3>Sexadores de pollo directory list</h3>\n<ul>");
        while ((getline(&line, &len, stream)) != -1) {
            for (int i = 0; i < len; ++i) {
                if (line[i] == '\n') {
                    line[i] = '\0';
                    break;
                }
            }
            ret = strchr(argv[1], '/');
            printf("<li><a href=%s/%s>%s</a></li>\n", ret ? ret : "", line, line);
        }
        printf("</ul>\n</body>\n</html>\n");
        unlink(".ls_cgi");
        free(line);
        fclose(stream);
    }
}