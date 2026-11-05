/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 13:43:04 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/05 02:33:45 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Checks which request has higher priority*/
static int	higher_priority(t_priority_queue *queue, int a, int b)
{
	if (queue->edf)
	{
		if (queue->heap[a].deadline != queue->heap[b].deadline)
			return (queue->heap[a].deadline < queue->heap[b].deadline);
	}
	return (queue->heap[a].order < queue->heap[b].order);
}
/* Swaps two requests*/
static void	swap(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
/* Moves an element up the heap*/
static void	heapify_up(t_priority_queue *queue, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!higher_priority(queue, index, parent))
			break ;
		swap(&queue->heap[index], &queue->heap[parent]);
		index = parent;
	}
}
/* Moves an element down the heap*/
static void	heapify_down(t_priority_queue *queue, int index)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		smallest = index;
		if (left < queue->size && higher_priority(queue, left, smallest))
			smallest = left;
		if (right < queue->size && higher_priority(queue, right, smallest))
			smallest = right;
		if (smallest == index)
			break ;
		swap(&queue->heap[index], &queue->heap[smallest]);
		index = smallest;
	}
}
/* Initializes the priority queue*/
int	init_priority_queue(t_priority_queue *queue, int capacity, char *scheduler)
{
	if (capacity <= 0)
		return (0);
	queue->heap = malloc(sizeof(t_request) * capacity);
	if (!queue->heap)
		return (0);
	queue->size = 0;
	queue->acquiring = NULL;
	queue->capacity = capacity;
	queue->next_order = 0;
	queue->edf = (strcmp(scheduler, "edf") == 0);
	if (pthread_mutex_init(&queue->mutex, NULL) != 0)
	{
		free(queue->heap);
		return (0);
	}
	if (pthread_cond_init(&queue->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&queue->mutex);
		free(queue->heap);
		return (0);
	}
	return (1);
}
/* Adds a request to the priority queue*/
int	enqueue(t_priority_queue *queue, t_coder *coder, long deadline)
{
	int	index;

	if (queue->size >= queue->capacity)
		return (0);
	index = queue->size;
	queue->heap[index].coder = coder;
	queue->heap[index].deadline = deadline;
	queue->heap[index].order = queue->next_order++;
	queue->size++;
	heapify_up(queue, index);
	pthread_cond_broadcast(&queue->cond);
	return (1);
}
/* Removes and returns the request with highest priority*/
t_coder	*dequeue(t_priority_queue *queue)
{
	t_coder	*coder;

	if (queue->size == 0)
		return (NULL);
	coder = queue->heap[0].coder;
	queue->size--;
	if (queue->size > 0)
	{
		queue->heap[0] = queue->heap[queue->size];
		heapify_down(queue, 0);
	}
	pthread_cond_broadcast(&queue->cond);
	return (coder);
}
/* Removes a specific coder from the priority queue*/
int	remove_from_queue(t_priority_queue *queue, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < queue->size && queue->heap[i].coder != coder)
		i++;
	if (i == queue->size)
		return (0);
	queue->size--;
	if (i < queue->size)
	{
		queue->heap[i] = queue->heap[queue->size];
		if (i > 0 && higher_priority(queue, i, (i - 1) / 2))
			heapify_up(queue, i);
		else
			heapify_down(queue, i);
	}
	pthread_cond_broadcast(&queue->cond);
	return (1);
}
/* Destroys the priority queue*/
void	destroy_priority_queue(t_priority_queue *queue)
{
	pthread_cond_destroy(&queue->cond);
	pthread_mutex_destroy(&queue->mutex);
	free(queue->heap);
	queue->heap = NULL;
	queue->size = 0;
	queue->capacity = 0;
}
