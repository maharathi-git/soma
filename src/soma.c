#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "soma.h"


// Global variables for the bounding box of the central year text.
static double year_bbox_x = 0.0;
static double year_bbox_y = 0.0;
static double year_bbox_width = 0.0;
static double year_bbox_height = 0.0;

int selected_phase = -1;  // -1 means no selection
int hovered_phase = -1;
gboolean show_adjacent_years = FALSE;

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
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // White text
    cairo_set_font_size(cr, 36);
    cairo_text_extents_t extents;
    cairo_text_extents(cr, prasthutha_varsham, &extents);

    double text_x = center_x - extents.width/2;
    double text_y = center_y + extents.height/2;

    // Update bounding box (used for hover, scroll, and click detection).
    year_bbox_x = text_x;
    year_bbox_y = center_y - extents.height/2;
    year_bbox_width = extents.width;
    year_bbox_height = extents.height;

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
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, alpha);
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
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, alpha);
            cairo_text_extents_t ext;
            cairo_text_extents(cr, varsham[prasthutha_varsha_soochika-i], &ext);
            double x = center_x - ext.width / 2;
            double y = center_y + vertical_offset * i + 20;  // Move down for previous years
            cairo_move_to(cr, x, y);
            cairo_show_text(cr, varsham[prasthutha_varsha_soochika-i]);
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

    if (event->x >= year_bbox_x && event->x <= (year_bbox_x + year_bbox_width) &&
        event->y >= year_bbox_y && event->y <= (year_bbox_y + year_bbox_height)) {
        show_60_year_calendar();
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

    gboolean year_hovered = (event->x >= year_bbox_x && event->x <= (year_bbox_x + year_bbox_width) &&
                              event->y >= year_bbox_y && event->y <= (year_bbox_y + year_bbox_height));
    if (year_hovered && !show_adjacent_years) {
        show_adjacent_years = TRUE;
        gtk_widget_queue_draw(widget);
    } else if (!year_hovered && show_adjacent_years) {
        show_adjacent_years = FALSE;
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
    if (event->x >= year_bbox_x && event->x <= (year_bbox_x + year_bbox_width) &&
        event->y >= year_bbox_y && event->y <= (year_bbox_y + year_bbox_height))
    {
        if (event->direction == GDK_SCROLL_UP)
            move_year_up();     //move the current year up
        else if (event->direction == GDK_SCROLL_DOWN)
            move_year_down();
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

    GtkWidget *window = create_main_window();
    gtk_widget_show_all(window);
    gtk_main();

    // Cleanup: unreference images.
    for (int i = 0; i < THITHI; i++) {
        g_object_unref(pixbuf[i]);
    }

    return 0;
}
