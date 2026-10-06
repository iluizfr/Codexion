#include "codexion.h"

int compile(t_coder *coder_ptr)
{
    printf("%d is compiling\n\n", coder_ptr->id);

    pthread_mutex_unlock(&coder_ptr->left_dongle->mutex);
    pthread_mutex_unlock(&coder_ptr->right_dongle->mutex);

    return 0;
}

void	*routine(void *arg)
{
    t_coder	*coder_ptr = (t_coder *)arg;
    int     is_right_lock;
    int     is_left_lock;

    is_right_lock = pthread_mutex_trylock(&coder_ptr->right_dongle->mutex);

    if (!is_right_lock) {
        is_left_lock = pthread_mutex_trylock(&coder_ptr->left_dongle->mutex);

        if (!is_left_lock) {
            printf("%d has taken a dongle\n", coder_ptr->id);
            printf("%d has taken a dongle\n", coder_ptr->id);
            compile(coder_ptr);
        }
        else {
            pthread_mutex_unlock(&coder_ptr->right_dongle->mutex);
        }
    }

    return NULL;
}

void	*initialize_coders(t_data data)
{
    int			i;
    int         n_coders;
    t_coder		coders[data.number_of_coders];
    t_dongle    dongles[data.number_of_coders];
    t_manager   manager;

	i = 0;
    n_coders = data.number_of_coders;
    manager.timestamp = 0;

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

		pthread_create(&coders[i].thread, NULL, routine, &coders[i]);
		i++;
    }

	i = 0;
    while (i < n_coders)
		pthread_join(coders[i++].thread, NULL);
    i = 0;
    while (i < n_coders)
    {
        pthread_mutex_destroy(&dongles[i++].mutex);
    }

	return NULL;
}
