#include "types.h"
#include "stat.h"
#include "user.h"

char buff[512];

void head(int fd, int line)
{
    int i, n, l;
    l = 0;

    while ((n = read(fd, buff, sizeof(buff))) > 0)
    {
        for (i = 0; i <= n; i++)
        {
            // Not end of line
            if (buff[i] != '\n')
            {
                printf(0, "%c", buff[i]);
            }
            // Hit 10 lines
            else if (l == (line - 1))
            {
                printf(1, "\n");
                exit();
            }
            // End of line
            else
            {
                printf(1, "\n");
                l++;
            }
        }
    }
    if (n < 0)
    {
        printf(1, "head: read error\n");
        exit();
    }
}

int main(int argc, char *argv[])
{
    // File descriptor
    int fd = 0;

    // Handles case whereby no filename is provided, so head reads 10 lines from standard input.
    if (argc != 2)
    {
        head(0, 10);
    }
    else
    {
        if ((fd = open(argv[1], 0)) < 0)
        {
            // Error if can't open file, more than likely it does not exist
            printf(1, "head: cannot open '%s' for reading: No such file or directory\n", argv[1]);
            exit();
        }

        head(fd, 10);
        close(fd);
    }

    exit();
}
