#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "task.h"
#include "config.h"
#include "gui.h"


/* Configuration variables with default values, can be changed in config.txt */
char deleteIfCompleted[5] = "true";
char csvFileName[30] = "task.csv";
char interfaceColor[7] = "GREEN";
char dateFormat[11] = "%Y-%m-%d";


int main() {

    configInitialisation(csvFileName, interfaceColor, dateFormat, deleteIfCompleted);
    checkIfFileExist(csvFileName);

    if(strcmp(deleteIfCompleted, "true") == 0) {
        deleteTasksCompleted(csvFileName);
    }

    Task *taskList = NULL;

    readTasksFromFile(csvFileName, &taskList);

    initGui();

    setColor(interfaceColor);
    
    WINDOW *taskListWin = subwin(stdscr, ((2 * LINES) / 3) - 2, COLS - 2, 1, 1);
    WINDOW *commandsWin = subwin(stdscr, (LINES / 3) - 2, COLS - 2, ((2 * LINES) / 3) + 1, 1);
    WINDOW *actionWin = subwin(stdscr, (LINES / 3) - 2, COLS - 2, ((2 * LINES) / 3) + 1, 1);


    updateCommandsWin(commandsWin);
    updateTaskListWin(taskListWin);


    int currentTask = 1;
    int totalTask = findLastTaskID(csvFileName) - 1;

    printTaskList(taskList, dateFormat, taskListWin, currentTask);

    refresh();

    int ch;
    while ((ch = getch()) != 'q') {

        if (ch == KEY_UP) {
            if (currentTask <= 1) {
                currentTask = totalTask;
            } else {
                currentTask -= 1;
            }
            updateCommandsWin(commandsWin);
            updateTaskListWin(taskListWin);

            printTaskList(taskList, dateFormat, taskListWin, currentTask);
            wrefresh(taskListWin);
        }

        if (ch == KEY_DOWN) {
            if (currentTask >= totalTask) {
                currentTask = 1;
            } else {
                currentTask += 1;
            }
            updateCommandsWin(commandsWin);
            updateTaskListWin(taskListWin);

            printTaskList(taskList, dateFormat, taskListWin, currentTask);
            wrefresh(taskListWin);
        }

        // Remove task
        if (ch == 'r') {
            removeTask(csvFileName, currentTask);

            totalTask -= 1;
            wrefresh(commandsWin);

            while (taskList != NULL) {
                Task *temp = taskList;
                taskList = taskList->next;
                free(temp);
            }

            readTasksFromFile(csvFileName, &taskList);

            updateTaskListWin(taskListWin);
            wrefresh(taskListWin);

            printTaskList(taskList, dateFormat, taskListWin, currentTask);
            wrefresh(taskListWin);
        }
        
        // Validate task
        if (ch == 'x' || ch == ' ') {

            Task *task = (Task *)malloc(sizeof(Task));

            getTask(csvFileName, currentTask, task);
            task->state = !task->state;

            mvwprintw(commandsWin, 9, 2, "%s", task->taskName);
            mvwprintw(commandsWin, 10, 2, "%d", task->state);
            wrefresh(commandsWin);
            modifyTask(csvFileName, task->id, task);

            while (taskList != NULL) {
                Task *temp = taskList;
                taskList = taskList->next;
                free(temp);
            }

            readTasksFromFile(csvFileName, &taskList);

            updateTaskListWin(taskListWin);

            printTaskList(taskList, dateFormat, taskListWin, currentTask);
            wrefresh(taskListWin);

        }

        // Add task
        if (ch == 'a') {
            werase(commandsWin);

            wrefresh(commandsWin);

            updateActionWin(actionWin);
            
            Task *newTask = (Task *)malloc(sizeof(Task));

            echo();
            wgetnstr(actionWin, newTask->taskName, 49);
            noecho();

            mvwprintw(actionWin, 4, 2, "Deadline (yyyy-mm-dd): ");
            wrefresh(actionWin);
            echo();
            wgetnstr(actionWin, newTask->deadline, 10);
            noecho();

            newTask->id = findLastTaskID(csvFileName);
            newTask->parentID = 0;
            newTask->state = 0;

            totalTask += 1;

            writeTaskToFile(csvFileName, newTask);

            while (taskList != NULL) {
                Task *temp = taskList;
                taskList = taskList->next;
                free(temp);
            }

            readTasksFromFile(csvFileName, &taskList);

            updateTaskListWin(taskListWin);

            printTaskList(taskList, dateFormat, taskListWin, currentTask);
            wrefresh(taskListWin);

            updateCommandsWin(commandsWin);
            wrefresh(commandsWin);
            
        }

        // Add subtask
        if (ch == 's') {
            werase(commandsWin);
            wrefresh(commandsWin);

            updateActionWin(actionWin);
            
            Task *newTask = (Task *)malloc(sizeof(Task));

            echo();
            wgetnstr(actionWin, newTask->taskName, 49);
            noecho();

            mvwprintw(actionWin, 4, 2, "Deadline (yyyy-mm-dd): ");
            wrefresh(actionWin);
            echo();
            wgetnstr(actionWin, newTask->deadline, 10);
            noecho();

            newTask->id = findLastTaskID(csvFileName);
            newTask->parentID = currentTask;
            newTask->state = 0;

            totalTask += 1;

            writeTaskToFile(csvFileName, newTask);

            while (taskList != NULL) {
                Task *temp = taskList;
                taskList = taskList->next;
                free(temp);
            }
            readTasksFromFile(csvFileName, &taskList);

            updateTaskListWin(taskListWin);
            refresh();

            printTaskList(taskList, dateFormat, taskListWin, currentTask);
            wrefresh(taskListWin);

            updateCommandsWin(commandsWin);
            wrefresh(commandsWin);
            
        }

        // Resizing windows when resizing the terminal
        if (ch == KEY_RESIZE) {

            wresize(taskListWin, ((2 * LINES) / 3) - 2, COLS - 2);
            wresize(commandsWin, (LINES / 3) - 2, COLS - 2);

            updateTaskListWin(taskListWin);
            updateCommandsWin(commandsWin);

            printTaskList(taskList, dateFormat, taskListWin, currentTask);

            wrefresh(taskListWin);
            wrefresh(commandsWin);

            refresh();
        }
    }

    // Free up memory by releasing each list item
    while (taskList != NULL) {
        Task *temp = taskList;
        taskList = taskList->next;
        free(temp);
    }

    endwin();
    return 0;
}
