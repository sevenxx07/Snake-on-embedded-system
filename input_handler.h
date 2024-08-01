/**
 * Module with functions to receive and get input from the terminal.
 *
 * @file input_handler.h
 * @author Michal Vaverka (vavermic@fel.cvut.cz)
 * @date 2021-02-06
 * 
 * @copyright Copyright (c) 2021
 */
#include "utils.h"

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <pthread.h>

/*
* Function which reads the input from terminal.
* It runs in its own thread.
* (Terminal -> Thread data).
*/
void *input_thread(void *data);

/*
* Function which then returns the last read data;
* This one is called everytime from the main thread.
* (Thread data -> Running program).
*/
char read_input(pthread_mutex_t *mtx, thread_data_t *read_thread_data);

#endif
