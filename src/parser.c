#include "codexion.h"

int verify_arg(char *arg)
{
    int value;

    value = atoi(arg);
    if (value < 0)
        return (0);

    return (1);
}

int verify_args(char **argv)
{
    int i;
    int j;

    j = 1;
    while (j < 8 && argv[j])
    {
        i = 0;
        while (argv[j][i])
        {
            if (!isdigit(argv[j][i]))
            {
                printf("In '%s' the '%c' must be a number.\n", argv[j], argv[j][i]);
                return (-1);
            }
            i++;
        }
        if (!verify_arg(argv[j]))
        {
            printf("'%s' must be positive.", argv[j]);
            return (-1);
        }
        if (j == 1)
        {
            if (atoi(argv[j]) == 1)
            {
                printf("Its necessary at Least 2 coders to compile.");
                return (-1);
            }
        }
        j++;
    }
    if ((strcmp(argv[8], "fifo") != 0) && (strcmp(argv[8], "edf") != 0))
    {
        printf("Invalid argument, unknow schedular '%s'.\n", argv[8]);
        return (-1);
    }
    return 0;
}

int parser(int argc, char **argv)
{
    if (argc != 9)
    {
        printf("The number of arguments given is invalid.\n");
        return (0);
    }
    if (verify_args(argv) < 0)
        return 0;
    return 1;
}
