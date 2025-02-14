#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "soma.h"


// Global variables for the bounding box of the varsham text.
static double varsha_bbox_x = 0.0;
static double varsha_bbox_y = 0.0;
static double varsha_bbox_width = 0.0;
static double varsha_bbox_height = 0.0;

// Global variables for the bounding box of the masam text.
static double masa_bbox_x = 0.0;
static double masa_bbox_y = 0.0;
static double masa_bbox_width = 0.0;
static double masa_bbox_height = 0.0;

// Global variables for the bounding box of the masam text.
static double ruthu_bbox_x = 0.0;
static double ruthu_bbox_y = 0.0;
static double ruthu_bbox_width = 0.0;
static double ruthu_bbox_height = 0.0;

int selected_phase = -1;  // -1 means no selection
int hovered_phase = -1;
gboolean show_adjacent_years = FALSE;
gboolean show_adjacent_masa = FALSE;
gboolean show_adjacent_ruthuvu = FALSE;

// Helper: Draw text with a semi-transparent background.
void draw_text_with_bg(cairo_t *cr, const char *text, double x, double y,
                       double r, double g, double b)
{
    cairo_text_extents_t extents;
    cairo_set_font_size(cr, 12);
    cairo_text_extents(cr, text, &extents);

    int padding = 4;
    double bg_width = extents.width + 2 * padding;
    double bg_height = extents.height + 2 * padding;

    // Draw background rectangle.
    cairo_set_source_rgba(cr, 0, 0, 0, 0.6);
    cairo_rectangle(cr, x - bg_width/2, y - bg_height/2, bg_width, bg_height);
    cairo_fill(cr);

    // Draw text.
    cairo_set_source_rgb(cr, r, g, b);
    cairo_move_to(cr, x - extents.width/2, y + extents.height/2 - padding);
    cairo_show_text(cr, text);
}

// show_60_years
void show_60_year_calendar()
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

// on_draw: Renders the moon phases in a circle, draws the central year text,
// and (if enabled) draws the adjacent years.
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    (void)data;  // Unused

    cairo_paint(cr);// Set black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    int center_x = width / 2;
    int center_y = height / 2;
    int radius = MIN(center_x, center_y) - 20;

    for (int i = 0; i < THITHI; i++) {
        double angle = i * (2 * M_PI / THITHI) - (M_PI / 2);
        int img_width = gdk_pixbuf_get_width(pixbuf[i]);
        int img_height = gdk_pixbuf_get_height(pixbuf[i]);

        int x = center_x + (int)(radius * cos(angle)) - img_width / 2;
        int y = center_y + (int)(radius * sin(angle)) - img_height / 2;

        // Draw the moon phase image
        gdk_cairo_set_source_pixbuf(cr, pixbuf[i], x, y);
        cairo_paint(cr);

        // If this is the selected phase, draw a highlight
        if (i == selected_phase) {
            cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);  // Red highlight
            cairo_set_line_width(cr, 1.6);
            cairo_arc(cr, x + img_width / 2, y + img_height / 2, img_width / 2 + 5, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_new_path(cr);  // Clear the current path
        }

        // If this is the hovered phase, draw the phase name below it
        if (i == hovered_phase) {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // White text for visibility
            cairo_set_font_size(cr, 12);
            cairo_move_to(cr, x + img_width / 2 - 30, y + img_height + 20);  // Position the text below the image
            cairo_show_text(cr, thithulu[i]);
            cairo_new_path(cr);  // Clear the current path
        }
    }

    // Draw the central year text.
    cairo_set_source_rgb(cr, 1.0, 1.0, 0.6);  // White text
    cairo_set_font_size(cr, 36);
    cairo_text_extents_t extents;
    cairo_text_extents(cr, prasthutha_varsham, &extents);

    double text_x = center_x - extents.width/2;
    double text_y = center_y + extents.height/2;

    // Update bounding box (used for hover, scroll, and click detection).
    varsha_bbox_x = text_x;
    varsha_bbox_y = center_y - extents.height/2;
    varsha_bbox_width = extents.width;
    varsha_bbox_height = extents.height;

    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, prasthutha_varsham);
    cairo_new_path(cr);

    // Only show additional years if hovering over the year text.
    if (show_adjacent_years) {
        int num_years = 5; // Number of previous/next years to display
        // Base settings for the central year.
        double base_font_size = 36;
        double base_alpha = 1.0;
        double font_decrement = 4;      // How much the font size reduces per step
        double alpha_decrement = 0.20;    // How much the alpha reduces per step
        double vertical_offset = 40;      // Vertical spacing between years

        // Draw next years above the central year.
        for (int i = 1; i <= num_years; i++) {
            double font_size = base_font_size - (i * font_decrement);
            double alpha = base_alpha - (i * alpha_decrement);
            if (alpha < 0) alpha = 0;
            cairo_set_font_size(cr, font_size);
            cairo_set_source_rgba(cr, 1.0, 1.0, 0.8, alpha);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, varsham[prasthutha_varsha_soochika+i], &ext);
            double x = center_x - ext.width / 2;
            double y = center_y - vertical_offset * i;  // Move up for next years

            cairo_move_to(cr, x, y);
            cairo_show_text(cr, varsham[prasthutha_varsha_soochika+i]);
            cairo_new_path(cr);
        }

        vertical_offset = 40;      // Vertical spacing between years
        // Draw previous years below the central year.
        for (int i = 1; i <= num_years; i++) {
            double font_size = base_font_size - (i * font_decrement);
            double alpha = base_alpha - (i * alpha_decrement);
            if (alpha < 0) alpha = 0;
            cairo_set_font_size(cr, font_size);
            cairo_set_source_rgba(cr, 1.0, 1.0, 0.4, alpha);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, varsham[prasthutha_varsha_soochika-i], &ext);
            double x = center_x - ext.width / 2;
            double y = center_y + vertical_offset * i + 20;  // Move down for previous years
            cairo_move_to(cr, x, y);
            cairo_show_text(cr, varsham[prasthutha_varsha_soochika-i]);
            cairo_new_path(cr);
        }
    }

    // show masalu
    cairo_set_source_rgb(cr, 1.0, 1.0, 0.8);  // White text
    cairo_set_font_size(cr, 24);
    cairo_text_extents_t extents_m;
    cairo_text_extents(cr, prasthutha_masam, &extents_m);

    text_x = center_x - extents_m.width/2;
    text_y = center_y + extents_m.height/2 + 40;

    // Update bounding box for masalu (used for hover, scroll, and click detection).
    masa_bbox_x = text_x;
    masa_bbox_y = center_y - extents_m.height/2 + 40;
    masa_bbox_width = extents_m.width;
    masa_bbox_height = extents_m.height;

    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, prasthutha_masam);
    cairo_new_path(cr);

    // Only show additional masalu if hovering over the masa text.
    if (show_adjacent_masa) {
        int num_years = 4; // Number of previous/next masalu to display
        // Base settings for the central masam.
        double base_font_size = 20;
        double base_alpha = 1.0;
        double font_decrement = 2;      // How much the font size reduces per step
        double alpha_decrement = 0.20;    // How much the alpha reduces per step
        double vertical_offset = 40;      // Vertical spacing between years

        // Draw next masalu above the central masa.
        for (int i = 1; i <= num_years; i++) {
            double font_size = base_font_size - (i * font_decrement);
            double alpha = base_alpha - (i * alpha_decrement);
            if (alpha < 0) alpha = 0;
            cairo_set_font_size(cr, font_size);
            cairo_set_source_rgba(cr, 1.0, 1.0, 0.8, alpha);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, masam[prasthutha_masa_soochika+i], &ext);
            double x = center_x - ext.width / 2;
            double y = center_y - vertical_offset * i + 40;  // Move up for next masalu
            cairo_move_to(cr, x, y);
            cairo_show_text(cr, masam[prasthutha_masa_soochika+i]);
            cairo_new_path(cr);
        }

        vertical_offset = 40;      // Vertical spacing between masalu
        // Draw previous masalu below the central masa.
        for (int i = 1; i <= num_years; i++) {
            double font_size = base_font_size - (i * font_decrement);
            double alpha = base_alpha - (i * alpha_decrement);
            if (alpha < 0) alpha = 0;
            cairo_set_font_size(cr, font_size);
            cairo_set_source_rgba(cr, 1.0, 1.0, 0.4, alpha);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, masam[prasthutha_masa_soochika-i], &ext);
            double x = center_x - ext.width / 2;
            double y = center_y + vertical_offset * i + 20 + 40;  // Move down for previous masalu
            cairo_move_to(cr, x, y);
            cairo_show_text(cr, masam[prasthutha_masa_soochika-i]);
            cairo_new_path(cr);
        }
    }

    // show ruthuvulu
    cairo_set_source_rgb(cr, 1.0, 1.0, 0.8);  // White text
    cairo_set_font_size(cr, 24);
    cairo_text_extents_t extents_ruthuvulu;
    cairo_text_extents(cr, prasthutha_ruthuvu, &extents_ruthuvulu);

    text_x = center_x - extents_ruthuvulu.width/2;
    text_y = center_y + extents_ruthuvulu.height/2 - 40;

    // Update bounding box for ruhtuvu (used for hover, scroll, and click detection).
    ruthu_bbox_x = text_x;
    ruthu_bbox_y = center_y - extents_ruthuvulu.height/2 - 40;
    ruthu_bbox_width = extents_ruthuvulu.width;
    ruthu_bbox_height = extents_ruthuvulu.height;

    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, prasthutha_ruthuvu);
    cairo_new_path(cr);

    // Only show additional ruthuvulu if hovering over the masa text.
    if (show_adjacent_ruthuvu) {
        int ruthuv_n = 2; // Number of previous/next ruthuvulu to display
        // Base settings for the central ruthuvu.
        double base_font_size = 20;
        double base_alpha = 1.0;
        double font_decrement = 2;      // How much the font size reduces per step
        double alpha_decrement = 0.20;    // How much the alpha reduces per step
        double vertical_offset = 40;      // Vertical spacing between years

        // Draw next ruthuvulu above the central masa.
        for (int i = 1; i <= ruthuv_n; i++) {
            double font_size = base_font_size - (i * font_decrement);
            double alpha = base_alpha - (i * alpha_decrement);
            if (alpha < 0) alpha = 0;
            cairo_set_font_size(cr, font_size);
            cairo_set_source_rgba(cr, 1.0, 1.0, 0.8, alpha);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, ruthuvu[prasthutha_ruthu_soochika+i], &ext);
            double x = center_x - ext.width / 2;
            double y = center_y - vertical_offset * i - 40;  // Move up for next ruthuvulu
            cairo_move_to(cr, x, y);
            cairo_show_text(cr, ruthuvu[prasthutha_ruthu_soochika+i]);
            cairo_new_path(cr);
        }

        vertical_offset = 40;      // Vertical spacing between ruthuvulu
        // Draw previous ruthuvulu below the central masa.
        for (int i = 1; i <= ruthuv_n; i++) {
            double font_size = base_font_size - (i * font_decrement);
            double alpha = base_alpha - (i * alpha_decrement);
            if (alpha < 0) alpha = 0;
            cairo_set_font_size(cr, font_size);
            cairo_set_source_rgba(cr, 1.0, 1.0, 0.4, alpha);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, ruthuvu[prasthutha_ruthu_soochika-i], &ext);
            double x = center_x - ext.width / 2;
            double y = center_y + vertical_offset * i - 40;  // Move down for previous ruthuvulu
            cairo_move_to(cr, x, y);
            cairo_show_text(cr, ruthuvu[prasthutha_ruthu_soochika-i]);
            cairo_new_path(cr);
        }
    }

    return FALSE;
}

// on_mouse_click: If the user clicks on the central year text, open the 60‑year calendar window.
static gboolean on_mouse_click(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    (void)data;    // Mark data as unused, if needed

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    int center_x = width / 2;
    int center_y = height / 2;
    int radius = MIN(center_x, center_y) - 20;
    
    for (int i = 0; i < THITHI; i++) {
        double angle = i * (2 * M_PI / THITHI) - (M_PI / 2);
        int img_width = gdk_pixbuf_get_width(pixbuf[i]);
        int img_height = gdk_pixbuf_get_height(pixbuf[i]);
        int x = center_x + (int)(radius * cos(angle)) - img_width / 2;
        int y = center_y + (int)(radius * sin(angle)) - img_height / 2;
        if (event->x >= x && event->x <= (x + img_width) &&
            event->y >= y && event->y <= (y + img_height)) {
            selected_phase = i;
            gtk_widget_queue_draw(widget);  // Redraw the widget
            break;
        }
    }

    if (event->x >= varsha_bbox_x && event->x <= (varsha_bbox_x + varsha_bbox_width) &&
        event->y >= varsha_bbox_y && event->y <= (varsha_bbox_y + varsha_bbox_height)) {
        show_60_year_calendar();
        return TRUE;
    }

    if (event->x >= masa_bbox_x && event->x <= (masa_bbox_x + masa_bbox_width) &&
        event->y >= masa_bbox_y && event->y <= (masa_bbox_y + masa_bbox_height)) {
        masalu();
        return TRUE;
    }

    if (event->x >= ruthu_bbox_x && event->x <= (ruthu_bbox_x + ruthu_bbox_width) &&
        event->y >= ruthu_bbox_y && event->y <= (ruthu_bbox_y + ruthu_bbox_height)) {
        ruthuvulu();
        return TRUE;
    }

    return TRUE;
}

// on_mouse_motion: When the mouse pointer hovers over the central year text,
// set a flag to show adjacent years; when it leaves, hide them.
static gboolean on_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
    (void)data;

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    int center_x = width / 2;
    int center_y = height / 2;
    int radius = MIN(center_x, center_y) - 20;
    
    int new_hovered_phase = -1;  // Track the new hovered phase

    for (int i = 0; i < THITHI; i++) {
        double angle = i * (2 * M_PI / THITHI) - (M_PI / 2);
        int img_width = gdk_pixbuf_get_width(pixbuf[i]);
        int img_height = gdk_pixbuf_get_height(pixbuf[i]);
        int x = center_x + (int)(radius * cos(angle)) - img_width / 2;
        int y = center_y + (int)(radius * sin(angle)) - img_height / 2;

        if (event->x >= x && event->x <= (x + img_width) &&
            event->y >= y && event->y <= (y + img_height)) {            
            new_hovered_phase = i;  // Set the hovered phase
            // Only redraw if the hovered phase has changed
            if (new_hovered_phase != hovered_phase) {
                hovered_phase = new_hovered_phase;
                gtk_widget_queue_draw(widget);  // Redraw the widget
            }
            break;
        }
    }

    gboolean year_hovered = (event->x >= varsha_bbox_x && event->x <= (varsha_bbox_x + varsha_bbox_width) &&
                              event->y >= varsha_bbox_y && event->y <= (varsha_bbox_y + varsha_bbox_height));
    if (year_hovered && !show_adjacent_years) {
        show_adjacent_years = TRUE;
        gtk_widget_queue_draw(widget);
    } else if (!year_hovered && show_adjacent_years) {
        show_adjacent_years = FALSE;
        gtk_widget_queue_draw(widget);
    }

    gboolean masa_hovered = (event->x >= masa_bbox_x && event->x <= (masa_bbox_x + masa_bbox_width) &&
                              event->y >= masa_bbox_y && event->y <= (masa_bbox_y + masa_bbox_height));
    if (masa_hovered && !show_adjacent_masa) {
        show_adjacent_masa = TRUE;
        gtk_widget_queue_draw(widget);
    } else if (!masa_hovered && show_adjacent_masa) {
        show_adjacent_masa = FALSE;
        gtk_widget_queue_draw(widget);
    }

    gboolean ruthu_hovered = (event->x >= ruthu_bbox_x && event->x <= (ruthu_bbox_x + ruthu_bbox_width) &&
                              event->y >= ruthu_bbox_y && event->y <= (ruthu_bbox_y + ruthu_bbox_height));
    if (ruthu_hovered && !show_adjacent_ruthuvu) {
        show_adjacent_ruthuvu = TRUE;
        gtk_widget_queue_draw(widget);
    } else if (!ruthu_hovered && show_adjacent_ruthuvu) {
        show_adjacent_ruthuvu = FALSE;
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}

// on_scroll_event: If the user scrolls over the central year text, update the
// central year (scroll up to subtract a year, scroll down to add a year).
static gboolean on_scroll_event(GtkWidget *widget, GdkEventScroll *event, gpointer data)
{
    (void)data;
    (void)widget;
    if (event->x >= varsha_bbox_x && event->x <= (varsha_bbox_x + varsha_bbox_width) &&
        event->y >= varsha_bbox_y && event->y <= (varsha_bbox_y + varsha_bbox_height))
    {
        if (event->direction == GDK_SCROLL_UP)
            move_year_up();     //move the current year up
        else if (event->direction == GDK_SCROLL_DOWN)
            move_year_down();
        gtk_widget_queue_draw(widget);
        return TRUE;
    }
    if (event->x >= masa_bbox_x && event->x <= (masa_bbox_x + masa_bbox_width) &&
        event->y >= masa_bbox_y && event->y <= (masa_bbox_y + masa_bbox_height))
    {
        if (event->direction == GDK_SCROLL_UP)
            move_masa_up();     //move the current year up
        else if (event->direction == GDK_SCROLL_DOWN)
            move_masa_down();
        gtk_widget_queue_draw(widget);
        return TRUE;
    }
    if (event->x >= ruthu_bbox_x && event->x <= (ruthu_bbox_x + ruthu_bbox_width) &&
        event->y >= ruthu_bbox_y && event->y <= (ruthu_bbox_y + ruthu_bbox_height))
    {
        if (event->direction == GDK_SCROLL_UP)
            move_ruthu_up();     //move the current year up
        else if (event->direction == GDK_SCROLL_DOWN)
            move_ruthu_down();
        gtk_widget_queue_draw(widget);
        return TRUE;
    }
    return FALSE;
}

// create_main_window: Creates the main window, a drawing area, and connects
// the required event signals.
GtkWidget* create_main_window(void)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Moon Phases");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Enable events: mouse clicks, mouse motion, and scrolling.
    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK |
                                          GDK_POINTER_MOTION_MASK |
                                          GDK_SCROLL_MASK);

    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(on_mouse_click), NULL);
    g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(on_mouse_motion), NULL);
    g_signal_connect(drawing_area, "scroll-event", G_CALLBACK(on_scroll_event), NULL);

    return window;
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    load_moon_phase_images();   //load images

    get_prasthutha_varsham();
    get_prasthutha_masam();
    get_prasthutha_ruthuvu();

    GtkWidget *window = create_main_window();
    gtk_widget_show_all(window);
    gtk_main();

    // Cleanup: unreference images.
    for (int i = 0; i < THITHI; i++) {
        g_object_unref(pixbuf[i]);
    }

    return 0;
}
