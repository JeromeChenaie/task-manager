#include <ncurses.h>
#include <string.h>

// Update the task list window
void updateTaskListWin(WINDOW *win) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, (COLS - 10) / 2, "To-do list");
}

// Update the command window
void updateCommandsWin(WINDOW *win) {
    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 0, (COLS - 8) / 2, "Commands");

    mvwprintw(win, 1, 2, "a : add a task");
    mvwprintw(win, 2, 2, "s : add a subtask");
    mvwprintw(win, 3, 2, "r : delete a task");
    mvwprintw(win, 4, 2, "x / space : validate a task");
    mvwprintw(win, 5, 2, "q : quit application");
}

// Update action window
void updateActionWin(WINDOW *win) {
    box(win, 0, 0);
    mvwprintw(win, 0, (COLS - 8) / 2, "Create a new task");
    mvwprintw(win, 2, 2, "Task name: ");
    wrefresh(win);
}

// Initialize ncurses variables
void initGui() {
    initscr(); // Init ncurses
    cbreak(); // Disable line buffering
    noecho(); // Disables display of typed characters
    keypad(stdscr, TRUE); // Enables support for special keys

    curs_set(0); // Remove blinking cursor

    if (has_colors() != FALSE) {
        start_color();
    }
}

// Set up terminal colors
void setColor(char *color) {
    if (strcmp(color, "BLACK") == 0) {
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
    } else if (strcmp(color, "RED") == 0) {
        init_pair(1, COLOR_RED, COLOR_BLACK);
    } else if (strcmp(color, "GREEN") == 0) {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
    } else if (strcmp(color, "YELLOW") == 0) {
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    } else if (strcmp(color, "BLUE") == 0) {
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
    } else if (strcmp(color, "MAGENTA") == 0) {
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    } else if (strcmp(color, "CYAN") == 0) {
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
    } else {
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
    }

    attron(COLOR_PAIR(1));
}
