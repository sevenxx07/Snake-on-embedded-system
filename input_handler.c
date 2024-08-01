#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include "input_handler.h"
#include "utils.h"

#define TERMINAL_TIMEOUT_MS 100

void *input_thread(void *d)
{
    extern pthread_mutex_t mtx_c;
    extern pthread_cond_t character_read_c;

    thread_data_t *data = (thread_data_t *)d;
    bool end = false;

    struct termios tio, tioOld;

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    tcgetattr(STDIN_FILENO, &tio);
    tioOld = tio; // backup
    cfmakeraw(&tio);
    tio.c_oflag |= OPOST;
    tcsetattr(STDIN_FILENO, TCSANOW, &tio);

    // start reading terminal
    char input;
    while (!end)
    {
        read(STDIN_FILENO, &input, 1);
            
        pthread_mutex_lock(&mtx_c);
        data->last_input = data->last_read;
        data->last_read = input;
        pthread_mutex_unlock(&mtx_c);
        pthread_cond_broadcast(&character_read_c);

        pthread_mutex_lock(&mtx_c);
        end = end || data->quit; // check for quit
        pthread_mutex_unlock(&mtx_c);
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
    return NULL;
}

char read_input(pthread_mutex_t *mtx_v, thread_data_t *read_thread_data) 
{
    extern pthread_cond_t character_read_c;

    char input = ' ';
    pthread_mutex_lock(mtx_v);
    input = read_thread_data->last_read;
    pthread_mutex_unlock(mtx_v);

    return input;
}
