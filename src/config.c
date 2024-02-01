#include <stdio.h>
#include <stdlib.h>

#include "config.h"

// Create configuration file if not exist
void configInitialisation(char *csvFileName, char *interfaceColor, char *dateFormat, char *deleteIfCompleted) {
    FILE *file = fopen("config.txt", "r");
    if (file == NULL) {
        FILE *newFileConfig = fopen("config.txt", "w");
        fprintf(newFileConfig, "csvFileName: %s\ninterfaceColor: %s\ndateFormat: %s\ndeleteIfCompleted: %s\n", csvFileName, interfaceColor, dateFormat, deleteIfCompleted);

        fclose(newFileConfig);
    } else {
        fscanf(file, "csvFileName: %s\ninterfaceColor: %s\ndateFormat: %s\ndeleteIfCompleted: %s", csvFileName, interfaceColor, dateFormat, deleteIfCompleted);
        
        fclose(file);
    }
}