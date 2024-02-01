#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "task.h"

// Create CSV file if not exist
void checkIfFileExist(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        FILE *newFile = fopen(filename, "w");
        fprintf(newFile, "ID,taskName,parentID,Deadline,State\n");
        fclose(newFile);
    }
}

// Read tasks from CSV file
void readTasksFromFile(char *filename, Task **head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    fgets(line, sizeof(line), file);  // Ignore first line (headers)

    while (fgets(line, sizeof(line), file) != NULL) {
        Task *newTask = (Task *)malloc(sizeof(Task));
        sscanf(line, "%d,%[^,],%d,%[^,],%d\n", &newTask->id, newTask->taskName, &newTask->parentID, newTask->deadline, &newTask->state);
        newTask->next = NULL;
        addTaskToList(head, newTask);
    }

    fclose(file);
}

// Get last task ID
int findLastTaskID(const char *filename){
    int line = 0;
    char character;

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    while ((character = fgetc(file)) != EOF) {
        if (character == '\n') {
            line++;
        }
    }

    fclose(file);
    return line;
}

// Write task to CSV file
/* Create temporary file to copy original file and insert the new subtask */
void writeTaskToFile(const char *filename, const Task *task) {
    FILE *csvFile = fopen(filename, "a+");
    if (csvFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (task->parentID > 0){
        FILE *tmpFile = fopen("tmp.csv", "w");
        if (tmpFile == NULL) {
            perror("Error opening temporary file");
            exit(EXIT_FAILURE);
        }

        char line[100];
        char idTask[10];
        sprintf(idTask, "%d", task->parentID);

        int i = 1;

        // Ignore headers line
        fgets(line, sizeof(line), csvFile);
        fprintf(tmpFile, "ID,taskName,parentID,Deadline,State\n");

        while (fgets(line, sizeof(line), csvFile) != NULL) {            

            if (strstr(line, idTask) == line) {
                char *element = strtok(line, ",");
                int elementNumber = 0;
                char newLine[100];
                char id[12];
                sprintf(id, "%d", i);
                while (element != NULL) {
                    if (elementNumber == 0) {
                        strcpy(newLine, id);
                        strcat(newLine, ",");
                    } else if (elementNumber == 4) {
                        strcat(newLine, element);
                    } else {
                        strcat(newLine, element);
                        strcat(newLine, ",");
                    }
                    element = strtok(NULL, ",");
                    elementNumber += 1;
                }
                fputs(newLine, tmpFile);
                fprintf(tmpFile, "%d,%s,%d,%s,%d\n", i + 1, task->taskName, task->parentID, task->deadline, task->state);
                i += 1;
            } else {
                char *element = strtok(line, ",");
                int elementNumber = 0;
                char newLine[100];
                char id[12];
                sprintf(id, "%d", i);
                while (element != NULL) {
                    if (elementNumber == 0) {
                        strcpy(newLine, id);
                        strcat(newLine, ",");
                    } else if (elementNumber == 4) {
                        strcat(newLine, element);
                    } else {
                        strcat(newLine, element);
                        strcat(newLine, ",");
                    }
                    element = strtok(NULL, ",");
                    elementNumber += 1;
                }
                
                fputs(newLine, tmpFile);
            }
            i += 1;
        }
        fclose(tmpFile);
        fclose(csvFile);
        remove(filename);
        rename("tmp.csv", filename);
    } else {
        // Add task at the end of file
        fprintf(csvFile, "%d,%s,%d,%s,%d\n", task->id, task->taskName, task->parentID, task->deadline, task->state);
        fclose(csvFile);
    }
}

// Print task list (debug)
void printTaskList(Task *head, char *dateFormat, WINDOW *win, int currentTask) {
    Task *current = head;

    int i = 2;

    while (current != NULL) {
        char outputDate[11];
        convertDateFormat(outputDate, current->deadline, dateFormat);

        char task[200];
        char idString[4];

        sprintf(idString, "%d", current->id);

        if (current->state == 1) {
            strcpy(task, "[X] ");
        }else {
            strcpy(task, "[ ] ");
        }

        strcat(task, current->taskName);
        strcat(task, " (");
        strcat(task, outputDate);
        strcat(task, ") [");
        strcat(task, idString);
        strcat(task, "]");

        if (currentTask == current->id) {
            wattron(win, A_REVERSE);
        }

        if (current->parentID == 0) {
            mvwprintw(win, i + 1, 2, task);
        } else {
            mvwprintw(win, i + 1, 6, task);
        }

        wattroff(win, A_REVERSE);
        i += 1;
        

        current = current->next;
    }

    wrefresh(win);
}

// Add task from file to list
void addTaskToList(Task **head, Task *newTask) {
    if (*head == NULL) {
        *head = newTask;
    } else {
        Task *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newTask;
    }
}

// Remove task from file
void removeTask(const char *filename, int taskID) {
    FILE *csvFile = fopen(filename, "r");
    FILE *tmpFile = fopen("tmp.csv", "w");

    if (csvFile == NULL || tmpFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    fprintf(tmpFile, "ID,taskName,parentID,Deadline,State\n");

    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), csvFile) != NULL) {
        int idInLine;
        sscanf(line, "%d,", &idInLine);
        
        
        if (idInLine != taskID && i != 0) {
            char *element = strtok(line, ",");
            int elementNumber = 0;
            char newLine[100];
            char id[10];
            sprintf(id, "%d", i);
            while (element != NULL) {
                if (elementNumber == 0) {
                    strcpy(newLine, id);
                    strcat(newLine, ",");
                } else if (elementNumber == 4) {
                    strcat(newLine, element);
                } else {
                    strcat(newLine, element);
                    strcat(newLine, ",");
                }
                element = strtok(NULL, ",");
                elementNumber += 1;
            }   
            fputs(newLine, tmpFile);
            i += 1;
        }

        if (i == 0) {
            i += 1;
        }
    }

    fclose(csvFile);
    fclose(tmpFile);

    if (rename("tmp.csv", filename) != 0) {
        printf("Temporary file renaming error");
        exit(EXIT_FAILURE);
    }
}

// Get a task with ID
void getTask(const char *filename, int taskID, const Task *task) {
    FILE *csvFile = fopen(filename, "r");
    if (csvFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    char line[100];

    while (fgets(line, sizeof(line), csvFile) != NULL) {
        int idInLine;
        sscanf(line, "%d,", &idInLine);

        if (idInLine == taskID) {
            sscanf(line, "%d,%[^,],%d,%[^,],%d\n", &task->id, task->taskName, &task->parentID, task->deadline, &task->state);
        }
    }
}

// Modify an existing task
void modifyTask(const char *filename, int taskID, Task *newTask) {
    FILE *csvFile = fopen(filename, "r");
    FILE *tmpFile = fopen("tmp.csv", "w");

    if (csvFile == NULL || tmpFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    char line[100];

    while (fgets(line, sizeof(line), csvFile) != NULL) {
        int idInLine;

        sscanf(line, "%d,", &idInLine);

        if (idInLine != taskID) {
            fputs(line, tmpFile);
        } else {
            fprintf(tmpFile, "%d,%s,%d,%s,%d\n", newTask->id, newTask->taskName, newTask->parentID, newTask->deadline, newTask->state);
        }
    }

    fclose(csvFile);
    fclose(tmpFile);

    if (rename("tmp.csv", filename) != 0) {
        printf("Temporary file renaming error");
        exit(EXIT_FAILURE);
    }
}

// Remove all tasks completed
void deleteTasksCompleted(const char *filename) {
    FILE *csvFile = fopen(filename, "r");
    FILE *tmpFile = fopen("tmp.csv", "w");

    if (csvFile == NULL || tmpFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    char line[100];

    while (fgets(line, sizeof(line), csvFile) != NULL) {
        int state;

        for (int i = strlen(line) - 1; i >= 0; i--) {
            if (line[i] >= '0' && line[i] <= '9') {
                state = line[i] - '0';
                break;
            }
        }

        if (state != 1) {
            fputs(line, tmpFile);
        }
    }

    fclose(csvFile);
    fclose(tmpFile);

    if (rename("tmp.csv", filename) != 0) {
        printf("Temporary file renaming error");
        exit(EXIT_FAILURE);
    }
}

// Convert date in international format (yyyy-mm-dd) to the given format
void convertDateFormat(char *formattedDate, char *inputDate, char *outputFormat){
    for (int i = 0; i < strlen(outputFormat); i++) {
        if (strchr("dmyY%/-", outputFormat[i]) == NULL) {
            fprintf(stderr, "Error : For the date format, only the following characters are accepted: d m y Y %% / -\n");
            exit(EXIT_FAILURE);
        }
    }

    struct tm date_tm;

    if (sscanf(inputDate, "%d-%d-%d", &date_tm.tm_year, &date_tm.tm_mon, &date_tm.tm_mday) != 3) {
        fprintf(stderr, "Error extracting date.\n");
        exit(EXIT_FAILURE);
    }

    date_tm.tm_year -= 1900;
    date_tm.tm_mon -= 1;

    if (strftime(formattedDate, 11, outputFormat, &date_tm) == 0) {
        fprintf(stderr, "Error formatting date.\n");
        exit(EXIT_FAILURE);
    }

}