#include <ncurses.h>

typedef struct Task {
    int id;
    char taskName[50];
    int parentID;
    char deadline[11];  // YYYY-MM-DD
    int state;
    struct Task *next;
} Task;

// Create CSV file if not exist
void checkIfFileExist(char *filename);

// Read tasks from CSV file
void readTasksFromFile(char *filename, Task **head);

// Get last task ID
int findLastTaskID(const char *filename);

// Write task to CSV file
/* Create temporary file to copy original file and insert the new subtask */
void writeTaskToFile(const char *filename, const Task *task);

// Print task list (debug)
void printTaskList(Task *head, char *dateFormat, WINDOW *win, int currentTask);

// Add task from file to list
void addTaskToList(Task **head, Task *newTask);

// Remove task from file
void removeTask(const char *filename, int taskID);

// Get a task with ID
void getTask(const char *filename, int taskID, const Task *task);

// Modify an existing task
void modifyTask(const char *filename, int taskID, Task *newTask);

// Remove all tasks completed
void deleteTasksCompleted(const char *filename);

// Convert date in international format (yyyy-mm-dd) to the given format
void convertDateFormat(char *formattedDate, char *inputDate, char *outputFormat);