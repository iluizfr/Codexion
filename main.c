#include "codexion.h"

int main(int argc, char **argv)
{
    t_data  data;

    if (!parser(argc, argv))
    {
        printf("Invalid input.\n");
        return (1);
    }
    data = create_data(argv);
    initialize(data);

    return 0;
}
