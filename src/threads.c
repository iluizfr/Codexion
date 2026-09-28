#include "codexion.h"

int compile(t_coder *coder_ptr)
{
    pthread_mutex_t mutex;
    if (coder_ptr->left_dongle != NULL && coder_ptr->right_dongle != NULL)
    {
        printf("%d is compiling\n", coder_ptr->id);
        coder_ptr->left_dongle->is_usable = 1;
        coder_ptr->right_dongle->is_usable = 1;

        pthread_mutex_unlock(coder_ptr->left_dongle);
        pthread_mutex_unlock(coder_ptr->right_dongle);
        coder_ptr->left_dongle = NULL;
        coder_ptr->right_dongle = NULL;
    }
}

void	*routine(void *arg)
{
    t_coder	*coder_ptr = (t_coder *)arg;
    t_dongle *dongle_ptr;
    int i;

    i = 0;
    while (i < coder_ptr->data.number_of_coders)
    {
        if (coder_ptr->dongles_ptr[i].is_usable)
        {
            dongle_ptr = &coder_ptr->dongles_ptr[i];

            if (coder_ptr->left_dongle == NULL)
            {
                pthread_mutex_lock(dongle_ptr);
                coder_ptr->left_dongle = dongle_ptr;
                dongle_ptr->is_usable = 0;
                printf("%d has taken a dongle 'left'\n", coder_ptr->id);
                i++;
            }

            if (coder_ptr->right_dongle == NULL && dongle_ptr->is_usable)
            {
                pthread_mutex_lock(dongle_ptr);
                coder_ptr->right_dongle = dongle_ptr;
                dongle_ptr->is_usable = 0;
                printf("%d has taken a dongle 'right'\n", coder_ptr->id);
            }
        }
        i++;
    }
    compile(coder_ptr);

	return NULL;
}

void	*initialize_coders(t_data data)
{
    int			i;
    t_coder		coders[data.number_of_coders];
    t_dongle     dongles[data.number_of_coders];

	i = 0;
    while (i < data.number_of_coders)
    {
        dongles[i].dogle_id = i + 1;
        dongles[i].is_usable = 1;
        dongles[i].dogle_cooldown = data.dongle_cooldown;
        i++;
    }
    i = 0;
    while (i < data.number_of_coders)
    {
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].data = data;
        coders[i].dongles_ptr = dongles;
        coders[i].left_dongle = NULL;
        coders[i].right_dongle = NULL;

		pthread_create(&coders[i].thread, NULL, routine, &coders[i]);
		i++;
    }

	i = 0;
    while (i < data.number_of_coders)
		pthread_join(coders[i++].thread, NULL);

	return NULL;
}

/*void *initialize(s_data data)
{
    return;
}*/