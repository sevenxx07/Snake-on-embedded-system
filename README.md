# Important credits
I worked on this project with one of my colleagues from school. Every head file has properly marked who is the author.
We also used some templates that are also signed by author. 
My work is in: game.h, lcd.h, led.h, main.c

# Introduction
The goal of this project was to create a 2D Snake game that can be run on a device MicroZed APO. The project was created as part of the 
semester work for the subject B35APO – Computer Architecture.

# Project structure
The game as such is divided into 3 basic parts, which are then divided into files.
a) Menu
b) Game
c) End of the game
As such, the files are then structured like this:<br />
![obrazek](https://github.com/user-attachments/assets/223341d5-0624-461f-afe2-c0f8fa395c81)<br />
All files then use the utils.h file. It contains the definition of all necessary structures in program. More detailed information with description of functions, structures
and variables can be found in individual files as block comments. The main.c function is the startup function of the entire program. In the first part starts the opening scene 
from game.h with then the menu from menu.h. It reacts for user input via SSH. It will then be filled in the settings in the menu game structure.

The menu returns the filled game if the user confirms the start. Returns NULL when shutting down the game. After starting the game, we create a playing field and initialize both 
snakes in it as well, based on the settings. 

We will also start two threads - one for user input and the other for the game itself. A game as such is then actually an infinite while loop that updates the playing field and
loads the movement at each step of the game the player or makes a computer move. These moves are then checked if they can be done and if
they do not lead to the end of the game. If so, the update_game method from game.h returns 1 and thus exits infinity loop. Only the final scene appears, which lists the winner. The game then returns back to the menu.

# The game and its rules
Snake is an arcade game where the player controls the movements of a snake on the playing field. Our implementation includes computer vs computer and computer vs player. The playing surface is 
bounded, so the game is lost if you collide with it ends The snake is extended by one tile using bites that appear randomly on the playing field (in one
moment there is only one bite on the board). The aim of the game is to stay alive longer than your opponent. Game it ends prematurely when the snake hits a wall or the body 
of a snake, both its own and its opponent's.

# Used peripherals
In the program, we use RGB diodes, an LED line, writing to a graphic display and standard input via SSH without waiting for a newline. The display shows the initial inscription, 
then the menu, the playing surface and then renders the end of the game. The RGB LEDs light up with the color of the snake on each side if a particular snake wins the RGB
LEDs light up with its color. The LED bar lights up at the same speed as the game speed. Standard login via SSH works both in the menu and in the game without waiting for a new line.

# User manual
After starting the game, a menu will appear. This is controllable with W - up, S - down, E - select. START starts the game. QUIT the game on the contrary
turns off. Using SETTINGS we can set the game. After selection, another menu will appear.
Here you can choose:<br />
a) type – TYPE (PVC – player vs computer, CVC – computer vs computer)<br />
b) difficulty - DIFF (EASY - simple, HRD - difficult)<br />
c) and speed - SPEED (choose between numbers 1 - 3 from slowest to fastest)<br />
And that using the same controls as in the main menu. The settings are saved after selecting CONFIRM. That's it we move back to the main menu and can start the set game.
The game itself can be controlled with W - up, A - left, S - down, D - right.
After the end of the game according to the rules described in the third section, the winner will appear and the game will end. After that the main menu appears again.
