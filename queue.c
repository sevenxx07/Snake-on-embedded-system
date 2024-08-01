#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

//When using pop on a single-member queue, it causes segmentation fault. 
//This could be resolved by a few if statements, however we will never need it.
//  (Our snake always pushes one new field to the queue BEFORE popping the last one, so even
//   if snake was only of length = 1, upon calling the pop function, his body_queue would have 2 members.)

member_t *create_member(spot_t *spot) 
{
    member_t *ret = malloc(sizeof(member_t));
    ret->spot = spot;
    ret->next = NULL;
    ret->prev = NULL;
    return ret;
}

queue_t *create_queue() 
{
    queue_t *ret = malloc(sizeof(queue_t));
    ret->head = NULL;
    ret->tail = NULL;
    return ret;
}

void push(queue_t *queue, spot_t *spot) 
{
    member_t *new_spot = create_member(spot);

    if (queue->head != NULL) {
        queue->head->prev = (void *) new_spot;
        new_spot->next = (void *) queue->head;
    }
    
    queue->head = new_spot;
    if (queue->tail == NULL) {
        queue->tail = new_spot;
    }
}

member_t *pop(queue_t *queue) 
{
    member_t *ret = queue->tail;
    queue->tail = (member_t *) queue->tail->prev;
    queue->tail->next = NULL;
    return ret;
}

void clear_cq(queue_t *queue)
{
    member_t *curr_member = queue->head;
    member_t *tmp_member;
    while (1) {
        tmp_member = curr_member->next;
        free(curr_member);

        if (tmp_member == NULL) {
            break;
        }

        curr_member = (member_t *) tmp_member;
    }
    free(queue);
}