#include "codexion.h"

t_coder   *list_new(t_data data, int coder_id)
{
    t_coder    *new;

    new = (t_coder *)malloc(sizeof(t_coder));
    if (!new)
        return (NULL);

    return (new);
}
