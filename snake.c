#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "snake.h"
#include "queue.h"
#include "playing_board.h"
#include "input_handler.h"
#include "led.h"

snake_t *create_snake(bool is_player, int color, bool snake_nr) 
{
    snake_t *ret = malloc(sizeof(snake_t));

    if (ret == NULL) {
        fprintf(stderr, "Memory allocation fault.");
        exit(-1);
    }

    ret->snake_body = create_queue();
    ret->is_alive = true;
    ret->is_player = is_player;
    ret->color = color;
    ret->direction = DIR_DOWN;
    ret->has_eaten = false;
    ret->score = 0;
    ret->snake_nr = snake_nr;

    return ret;
}

void move_general(playing_board_t *playing_board, snake_t *snake) 
{
    if(snake->color == 0x0000ff){
        set_left_led_RGB(playing_board->game->led_mem_base, 0x0000ff);
    }else{
        set_right_led_RGB(playing_board->game->led_mem_base, 0x00ff00);
    }
    playing_board->score_strip = playing_board->score_strip>>1;
    set_led_strip(playing_board->game->led_mem_base, ~playing_board->score_strip);
    if(playing_board->score_strip == 0){
        playing_board->score_strip = 4294967295;
    }
    member_t *popped_member;
    spot_t *next_spot = get_next(playing_board, snake->direction, snake->snake_body->head->spot->total_index);

    if (next_spot == NULL) {
        snake->is_alive = false;
        kill_snake(snake, playing_board);
        return;
    }

    if (next_spot->contains_point) {
        snake->has_eaten = true;
        next_spot->contains_point = false;
        if(snake->color == 0x0000ff){
            set_left_led_RGB(playing_board->game->led_mem_base, 0xff0000);
        }else{
            set_right_led_RGB(playing_board->game->led_mem_base, 0xff0000);
        }
        generate_point(playing_board);
    }

    next_spot->is_snake = true;
    fill_spot(next_spot, playing_board->game, snake->color);

    push(snake->snake_body, next_spot);

    if (!snake->has_eaten) {
        popped_member = pop(snake->snake_body);
        popped_member->spot->is_snake = false;
        reverse_spot(popped_member->spot, playing_board->game);
        snake->has_eaten = false;
        snake->score++;

        free(popped_member);
    } else {
        snake->has_eaten = false;
    }
}

int input_to_int_direction(char new_direction) 
{
    if (new_direction == 'a') {
        return DIR_LEFT;
    } else if (new_direction == 'd') {
        return DIR_RIGHT;
    } else if (new_direction == 'w') {
        return DIR_UP;
    } else if (new_direction == 's') {
        return DIR_DOWN;
    } else {
        return ILLEGAL_DIRECTION;
    }
}

bool is_legal_move(int direction, int new_direction) 
{
    if (new_direction == ILLEGAL_DIRECTION) {
        return false;
    }

    if ((direction == DIR_UP && new_direction == DIR_DOWN) || (direction == DIR_DOWN && new_direction == DIR_UP)
            || (direction == DIR_LEFT && new_direction == DIR_RIGHT) || (direction == DIR_RIGHT && new_direction == DIR_LEFT)) 
            {
                return false;
            }

    return true;
}

void move_player(playing_board_t *playing_board, snake_t *snake)
{
    char input = read_input(playing_board->mtx, playing_board->thread_data);

    int input_as_int = input_to_int_direction(input);

    if (is_legal_move(snake->direction, input_as_int)) {
        snake->direction = input_as_int;
    }

    move_general(playing_board, snake);
}

int simple_to_directions(int simple) 
{
    if (simple == 0) {
        return DIR_DOWN;
    } else if (simple == 1) {
        return DIR_UP;
    } else if (simple == 2) {
        return DIR_RIGHT;
    } else if (simple == 3) {
        return DIR_LEFT;
    } else {
        return DIR_LEFT;
    }
}

int complex_to_directions(playing_board_t *playing_board, snake_t *snake, int importance)
{
    int dir_1, dir_2;
    int up = abs((snake->snake_body->head->spot->total_index + DIR_UP) - playing_board->point->total_index);
    int down = abs((snake->snake_body->head->spot->total_index + DIR_DOWN) - playing_board->point->total_index);
    int left = abs((snake->snake_body->head->spot->total_index + DIR_LEFT) - playing_board->point->total_index);
    int right = abs((snake->snake_body->head->spot->total_index + DIR_RIGHT) - playing_board->point->total_index);
    dir_1 = (up < down) ? DIR_UP : DIR_DOWN;
    dir_2 = (left < right) ? DIR_LEFT : DIR_RIGHT;

    int dir1 = abs((snake->snake_body->head->spot->total_index + dir_1) - playing_board->point->total_index);
    int dir2 = abs((snake->snake_body->head->spot->total_index + dir_2) - playing_board->point->total_index);

    if((dir1 < dir2) && importance == 1){
        return dir_1;
    }else{
        return dir_2;
    }

}

void move_ai_easy(playing_board_t *playing_board, snake_t *snake)
{
    int move = rand() % NUMBER_OF_DIRECTIONS;
    move = simple_to_directions(move);

    if (is_legal_move(snake->direction, move) && (get_next(playing_board, move, snake->snake_body->head->spot->total_index)!= NULL)) {
        snake->direction = move;
    }
    move_general(playing_board, snake);
}

void move_ai_hard(playing_board_t *playing_board, snake_t *snake)
{
    int direction = complex_to_directions(playing_board, snake, 1);

    if (is_legal_move(snake->direction, direction) && (get_next(playing_board, direction, snake->snake_body->head->spot->total_index)!=NULL)) {
        snake->direction = direction;
    }else{
        direction = complex_to_directions(playing_board, snake, 2);
        if (is_legal_move(snake->direction, direction) && (get_next(playing_board, direction, snake->snake_body->head->spot->total_index)!=NULL)) {
            snake->direction = direction;
        }
    }
    int i = 0;
    while(get_next(playing_board, snake->direction, snake->snake_body->head->spot->total_index) == NULL){
        int move = rand() % NUMBER_OF_DIRECTIONS;
        move = simple_to_directions(move);
        if(is_legal_move(snake->direction, move)){
            snake->direction = move;
        }
        i++;
        if(i == MAX_DIR_SWITCH_TRIES){
            break;
        }
    }

    move_general(playing_board, snake);
}

void move(snake_t *snake, playing_board_t *playing_board) 
{
    if(playing_board->game->type == false){
        if (snake->is_alive) {
            if (snake->is_player) {
                move_player(playing_board, snake);
            } else {
                if(playing_board->game->difficulty == false){
                    move_ai_easy(playing_board, snake);
                }else{
                    move_ai_hard(playing_board, snake);
                }
            }
        }
    }else{
        if (snake->is_alive) {
            if(playing_board->game->difficulty == false){
                move_ai_easy(playing_board, snake);
            }else{
                move_ai_hard(playing_board, snake);
            }
        }
    }
}

void kill_snake(snake_t *snake, playing_board_t *playing_board)
{
    member_t *curr_member = snake->snake_body->head;

    while (1) {
        if (curr_member == NULL) {
            break;
        }

        curr_member->spot->is_snake = false;
        reverse_spot(curr_member->spot, playing_board->game);
        curr_member = (member_t *) curr_member->next;
    }
    set_left_led_RGB(playing_board->game->led_mem_base, 0);
    set_right_led_RGB(playing_board->game->led_mem_base, 0);
    set_led_strip(playing_board->game->led_mem_base, 0);

    clear_cq(snake->snake_body);
}

void innit_snakes(playing_board_t *playing_board) 
{
    snake_t *snake = playing_board->first_snake;
    int spot_index = SNAKE_1_START_POINT;

    for (int i = 0; i < DEFAULT_BODY_SIZE; i++) {
        push(snake->snake_body, &playing_board->play_spots[spot_index]);
        fill_spot(&playing_board->play_spots[spot_index], playing_board->game, snake->color);
        spot_index += ONE_ROW_STEP;
    }

    spot_index = SNAKE_2_START_POINT;
    snake = playing_board->second_snake;

    for (int i = 0; i < DEFAULT_BODY_SIZE; i++) {
        push(snake->snake_body, &playing_board->play_spots[spot_index]);
        fill_spot(&playing_board->play_spots[spot_index], playing_board->game, snake->color);
        spot_index += ONE_ROW_STEP;
    }

    playing_board->score_strip = 4294967295;
    
    set_left_led_RGB(playing_board->game->led_mem_base, 0x0000ff);
    set_right_led_RGB(playing_board->game->led_mem_base, 0x00ff00);
    set_led_strip(playing_board->game->led_mem_base, ~playing_board->score_strip);
    
    generate_point(playing_board);
}
