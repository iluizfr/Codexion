#include "codexion.h"

int compile(t_coder *coder_ptr)
{
    printf("%d is compiling\n", coder_ptr->id);
    coder_ptr->compile_count++;
    pthread_mutex_unlock(&coder_ptr->left_dongle->mutex);
    pthread_mutex_unlock(&coder_ptr->right_dongle->mutex);

    return 1;
}

void	*routine(void *arg)
{
    t_coder	*coder_ptr = (t_coder *)arg;
    int     is_right_lock;
    int     is_left_lock;
    int     is_compiled;
    int     is_refactoring;

    if (coder_ptr->compile_count == coder_ptr->data.number_of_compiles_required)
    {
        printf("%d has made the number of compiles required.", coder_ptr->id);
        return NULL;
    }

    is_compiled = 0;
    is_refactoring = 0;
    is_right_lock = pthread_mutex_trylock(&coder_ptr->right_dongle->mutex);

    if (is_right_lock == 0)
    {
        printf("%d has taken a dongle\n", coder_ptr->id);
        is_left_lock = pthread_mutex_trylock(&coder_ptr->left_dongle->mutex);

        if (is_left_lock == 0)
        {
            printf("%d has taken a dongle\n", coder_ptr->id);
            // if we have both dongle, compile
            is_compiled = compile(coder_ptr);

            if (is_compiled)
            {
                printf("%d is debugging\n", coder_ptr->id);
                is_refactoring = 1;
            }
            if (is_refactoring)
            {
                printf("%d is refactoring\n", coder_ptr->id);
            }
        }
        else
        {
            pthread_mutex_unlock(&coder_ptr->right_dongle->mutex);
        }
    }
    return NULL;
}

void    init_dongles(t_dongle *dongles)
{

}

void	*initialize(t_data data)
{
    int			i;
    int         n_coders;
    //int         number_of_comp_required;
    t_coder		coders[data.number_of_coders];
    t_dongle    dongles[data.number_of_coders];
    t_manager   manager;

	i = 0;
    manager.timestamp = 0;
    n_coders = data.number_of_coders;
    //number_of_comp_required = data.number_of_compiles_required * n_coders;

    while (i < n_coders)
    {
        dongles[i].dogle_id = i + 1;
        dongles[i].dogle_cooldown = data.dongle_cooldown;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        i++;
    }

    i = 0;
    while (i < n_coders)
    {
        if (i == 0)
            coders[i].left_dongle = &dongles[data.number_of_coders - 1];
        else
            coders[i].left_dongle = &dongles[i - 1];

        coders[i].id = i + 1;
        coders[i].compile_count = 0;
        coders[i].data = data;
        coders[i].right_dongle = &dongles[i];
        coders[i].last_compile = 0;
        coders[i].manager = &manager;
		i++;
    }

    i = 0;
    while (i < n_coders)
    {
		pthread_create(&coders[i].thread, NULL, routine, &coders[i]);
		i++;
    }

	i = 0;
    while (i < n_coders)
    {
		pthread_join(coders[i++].thread, NULL);
    }

    i = 0;
    while (i < n_coders)
    {
        pthread_mutex_destroy(&dongles[i++].mutex);
    }

	return NULL;
}
