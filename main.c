#include "codexion.h"

int main(int argc, char **argv)
{
    t_data  data;

    if (!parser(argc, argv))
    {
        printf("Invalid input.\n");
        return (1);
    }
    data = init_data(argv);
    run(data);

    return 0;
}
