#include "soma.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdio.h>

// show_60_years
void samvathsaralu()
{
    GtkWidget *cal_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(cal_win), "samvathsaralu");
    gtk_window_set_default_size(GTK_WINDOW(cal_win), 400, 400);
    g_signal_connect(cal_win, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    // Create a grid to arrange the year labels.
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(cal_win), grid);

    int cols = 12; // 10 columns (10 x 6 = 60 cells)
    for (int i = 0; i < 60; i++) {
        GtkWidget *label = gtk_label_new(varsham[i]);
        int col = i % cols;
        int row = i / cols;
        gtk_grid_attach(GTK_GRID(grid), label, col, row, 1, 1);
    }
    gtk_widget_show_all(cal_win);
}

// show masalu
void masalu()
{
    GtkWidget *cal_masa = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(cal_masa), "masalu");
    gtk_window_set_default_size(GTK_WINDOW(cal_masa), 200, 200);
    g_signal_connect(cal_masa, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    // Create a grid to arrange the masa labels.
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(cal_masa), grid);

    int cols = 4; // 4 columns (4 x 3 = 12 cells)
    for (int i = 0; i < 12; i++) {
        GtkWidget *label = gtk_label_new(masam[i]);
        int col = i % cols;
        int row = i / cols;
        gtk_grid_attach(GTK_GRID(grid), label, col, row, 1, 1);
    }
    gtk_widget_show_all(cal_masa);
}

// show ruthuvulu
void ruthuvulu()
{
    GtkWidget *cal_ruthuvu = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(cal_ruthuvu), "ruthuvulu");
    gtk_window_set_default_size(GTK_WINDOW(cal_ruthuvu), 100, 200);
    g_signal_connect(cal_ruthuvu, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    // Create a grid to arrange the masa labels.
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(cal_ruthuvu), grid);

    int cols = 2; // 4 columns (4 x 3 = 12 cells)
    for (int i = 0; i < 6; i++) {
        GtkWidget *label = gtk_label_new(ruthuvu[i]);
        int col = i % cols;
        int row = i / cols;
        gtk_grid_attach(GTK_GRID(grid), label, col, row, 1, 1);
    }
    gtk_widget_show_all(cal_ruthuvu);
}