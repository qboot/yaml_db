//
//  main.c
//  yaml_db
//
//  Created by Quentin Brunet on 14/12/2017.
//  Copyright © 2017 Quentin Brunet. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

static void on_window_closed(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget *window, *label;
    
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Create new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    // Associate `destroy` event to `on_window_close` callback function
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);
    
    // Add text label
    label = gtk_label_new("Hello world!");
    
    // Add label to window
    gtk_container_add(GTK_CONTAINER(window), label);
    
    // Set window title
    gtk_window_set_title(GTK_WINDOW(window),  "yaml_db");
    
    // Set window default size and disallow resize
    gtk_window_set_default_size(GTK_WINDOW(window), 640 , 480);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
    // Show all generated widgets
    gtk_widget_show(label);
    gtk_widget_show(window);
    
    // Event loop
    gtk_main();
    
    return 0;
}

