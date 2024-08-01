/**
 * Module of a simple queue implementation to represent the snake.
 *
 * @file queue.h
 * @author Michal Vaverka (vavermic@fel.cvut.cz)
 * @date 2021-31-05
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef QUEUE_H
#define QUEUE_H
#include "utils.h"

/*
* Creates a single member of a given spot.
*/
member_t *create_member(spot_t *spot);

/*
* Creates new queue_t variable.
*/
queue_t *create_queue();

/*
* Pushes a new member on the beginning of the queue.
*/
void push(queue_t *queue, spot_t *spot);

/*
* Pops the last member of the queue.
*/
member_t *pop(queue_t *queue);

/*
* Clears the whole queue
*/
void clear_cq(queue_t *queue);

#endif
