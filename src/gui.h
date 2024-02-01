#include <ncurses.h>

// Update the task list window
void updateTaskListWin(WINDOW *win);

// Update the command window
void updateCommandsWin(WINDOW *win);

// Update action window
void updateActionWin(WINDOW *win);

// Initialize ncurses variables
void initGui();

// Set up terminal colors
void setColor(char *color);