#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char buf[1024];
    char *params[MAXARG];
    int i, n;

    if (argc < 2)
    {
        fprintf(2, "Usage: xargs <command> [args...]\n");
        exit(1);
    }

    // xargs echo bye  params: echo bye
    for (i = 1; i < argc; i++)
    {
        params[i - 1] = argv[i];
    }

    n = 0;
    while (1)
    {
        int cc = read(0, &buf[n], 1);
        if (cc <= 0)
        {
            fprintf(2, "why?\n");
            exit(0);
        }
        if (buf[n] == '\n')
        {
            buf[n] = '\0';
            break;
        }
        n++;
        if (n >= sizeof(buf))
        {
            fprintf(2, "xargs: buf too long\n");
            exit(1);
        }
    }
    params[argc - 1] = buf;
    if (fork() == 0)
    {
        exec(params[0], params);
    }
    else
    {
        wait(0);
    }

    exit(0);
}