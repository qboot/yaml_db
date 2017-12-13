//
//  Filename: main.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Main File
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/main.h"
#include "../headers/manage_file.h"
#include "../headers/manage_array.h"
#include "../headers/manage_entry.h"
#include "gtk/gtk.h"

static int isExitCommand(char *command);
void gtk_widget_show(GtkWidget *widget);

int main(int argc, const char* argv[])
{
    
    GtkWidget * MainWindow = NULL;
    
    /* Initialisation de GTK+ */
    gtk_init(&argc, &argv);
    
    /* Création de la fenêtre */
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    
    /* Affichage et boucle évènementielle */
    gtk_widget_show(MainWindow);
    gtk_main();
    
    /* On quitte.. */
    return EXIT_SUCCESS;
    
//    printf("Welcome to yaml_db project! \n");
//    printf("\n");
//    printf("----------------------------\n");
//    printf("   -                    -   \n");
//    printf("  -                      -  \n");
//    printf("  -       v.1.0.0        -  \n");
//    printf("  -                      -  \n");
//    printf("   -                    -   \n");
//    printf("----------------------------\n");
//    
//    entryLoop();
//    
//    printf("\nClosing yaml_db project... Bye!\n");
    
}

//
// Entry point : will loop on user input and provide entry to parser
//
void entryLoop()
{
    Manager manager = {DB_FILENAME, createFile(DB_FILENAME)};
    char *currentDatabase = malloc(sizeof(char)*STRING_SIZE);
    strcpy(currentDatabase, "");
    
    char entry[STRING_SIZE] = "";
    
    do {
        printf("\nyaml_db > ");
        fgets(entry, sizeof(entry), stdin);
        
        if (isExitCommand(entry)) {
            break;
        }
        
        parseEntry(manager, currentDatabase, entry);
    } while (1);
    
    free(currentDatabase);
    free(manager.path);
}

//
// Check if user has typed `exit` command
//
int isExitCommand(char *command)
{
    char cleanCommand[STRING_SIZE] = "";
    
    for (int i = 0; i < strlen(command)-1; ++i) {
        if (command[i] == ';') {
            continue;
        }
        
        char charToString[] = {tolower(command[i]), '\0'};
        strcat(cleanCommand, charToString);
    }
    
    trimTrailingSpaces(cleanCommand);
    
    if (strcmp(cleanCommand, "exit") == 0 || strcmp(cleanCommand, "quit") == 0) {
        return 1;
    }
    
    return 0;
}
