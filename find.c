#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

uint type = T_FILE;
char path[14] = {'\0'};

void find(char *start_path)
{
    char full_path[DIRSIZ] = {'\0'}, *p;
    int fd, newfd;
    struct dirent de;
    struct stat st, newst;

    // Catching errors with file descriptor and fstat
    if ((fd = open(start_path, 0)) < 0)
    {
        printf(2, "find: cannot open %s\n", start_path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        printf(2, "find: cannot stat %s\n", start_path);
        close(fd);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        strcpy(full_path, start_path);
        p = full_path + strlen(full_path);
        if (strcmp(start_path, "/") != 0)
        {
            p[0] = '/';
            p++;
        }
        strcpy(p, de.name);
        p = p + strlen(de.name);
        p[0] = '\0';

        // If not a file, continue
        if ((de.inum == 0) || (strcmp(de.name, "..") == 0) || (strcmp(de.name, ".") == 0))
        {
            continue;
        }

        // Catching errors with file descriptor and fstat
        if ((newfd = open(full_path, 0)) < 0)
        {
            printf(2, "find: cannot access %s/%s (1)\n", start_path, de.name);
            continue;
        }
        if (fstat(newfd, &newst) < 0)
        {
            printf(2, "find: cannot stat %s/%s\n", start_path, de.name);
            close(newfd);
            continue;
        }

        switch (newst.type)
        {
        case T_DIR:
            if (!strcmp(path, de.name))
            {
                printf(1, "%s\n", full_path);
            }
            find(full_path);
            break;

        default:
            if (!strcmp(path, de.name))
            {
                printf(1, "%s\n", full_path);
            }
            break;
        }
        close(newfd);
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        goto bad_use;
    }
    else if (argc == 3)
    {
        strcpy(path, argv[2]);
        find(argv[1]);
        exit();
    }

bad_use:
    printf(1, "Usage: find <starting_path> <filename_to_find>\n");
    exit();
}
