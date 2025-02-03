#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "soma.h"

GtkWidget *calendar;
GtkWidget *event_window;
GtkWidget *event_entry;
GtkWidget *event_label;
GtkWidget *date_label;
gchar current_event[1024];


int selected_phase = -1;  // -1 means no selection
int hovered_phase = -1;
GtkWidget *phase_label;


// Array to store the moon phase images
GdkPixbuf *pixbuf[THITHI];


static gboolean on_mouse_click(GtkWidget *widget, GdkEventButton *event, gpointer data) {

    (void)data; //suppress the warnings

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
    return TRUE;
}

static gboolean on_mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
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
    return TRUE;
}
// Function to load moon phase images
gboolean load_moon_phase_images() {
    
    for (int i = 0; i < THITHI; i++) {
        pixbuf[i] = gdk_pixbuf_new_from_file(image_files[i], NULL);
        if (pixbuf[i] == NULL) {
            g_printerr("Failed to load image: %s\n", image_files[i]);
            return FALSE;
        }
    }
    return TRUE;
}

// Helper function to draw text with a background
void draw_text_with_bg(cairo_t *cr, const char *text, int x, int y, double r, double g, double b) {
    cairo_text_extents_t extents;
    cairo_set_font_size(cr, 14);
    cairo_text_extents(cr, text, &extents);

    int padding = 4;
    int bg_width = extents.width + 2 * padding;
    int bg_height = extents.height + 2 * padding;

    // Draw semi-transparent rectangle
    cairo_set_source_rgba(cr, 0, 0, 0, 0.6);  // Black background, 60% opacity
    cairo_rectangle(cr, x - bg_width / 2, y - bg_height / 2, bg_width, bg_height);
    cairo_fill(cr);

    // Draw text
    cairo_set_source_rgb(cr, r, g, b);
    cairo_move_to(cr, x - extents.width / 2, y + extents.height / 2 - padding);
    cairo_show_text(cr, text);
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr) {
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    int center_x = width / 2;
    int center_y = height / 2;
    int radius = MIN(center_x, center_y) - 20;

    // Set black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    int selected_x = -1, selected_y = -1;  // To store selected phase position
    int hovered_x = -1, hovered_y = -1;    // To store hovered phase position

    for (int i = 0; i < THITHI; i++) {
        double angle = i * (2 * M_PI / THITHI) - (M_PI / 2);
        int img_width = gdk_pixbuf_get_width(pixbuf[i]);
        int img_height = gdk_pixbuf_get_height(pixbuf[i]);

        int x = center_x + (int)(radius * cos(angle)) - img_width / 2;
        int y = center_y + (int)(radius * sin(angle)) - img_height / 2;

        // Draw the moon phase image
        gdk_cairo_set_source_pixbuf(cr, pixbuf[i], x, y);
        cairo_paint(cr);

        // Store position of selected phase for later
        if (i == selected_phase) {
            selected_x = x + img_width / 2;
            selected_y = y + img_height + 20;
            cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);  // Red highlight
            cairo_set_line_width(cr, 2);
            cairo_arc(cr, x + img_width / 2, y + img_height / 2, img_width / 2 + 5, 0, 2 * M_PI);
            cairo_stroke(cr);
        }

        // Store position of hovered phase for later
        if (i == hovered_phase) {
            hovered_x = x + img_width / 2;
            hovered_y = y + img_height + 40;  // Slightly below selected text
        }
    }

    // Draw the selected phase name (if any)
    if (selected_phase != -1) {
        if( selected_phase >= 13 && selected_phase <= 17)
            draw_text_with_bg(cr, thithulu[selected_phase], selected_x, selected_y-70, 1.0, 1.0, 0.0);  // Yellow
        else
            draw_text_with_bg(cr, thithulu[selected_phase], selected_x, selected_y, 1.0, 1.0, 0.0);  // Yellow
    }

    // Draw the hovered phase name (if different from selected)
    if (hovered_phase != -1 && hovered_phase != selected_phase) {
        if( hovered_phase >= 13 && hovered_phase <= 17)
            draw_text_with_bg(cr, thithulu[hovered_phase], hovered_x, hovered_y-70, 0.5, 1.0, 1.0);  // Cyan
        else
            draw_text_with_bg(cr, thithulu[hovered_phase], hovered_x, hovered_y, 0.5, 1.0, 1.0);  // Cyan
    }

    // Draw Year Name at Center
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Yellow text
    cairo_set_font_size(cr, 36);
    cairo_text_extents_t extents;
    cairo_text_extents(cr, prasthutha_varsham, &extents);

    cairo_move_to(cr, center_x - extents.width / 2, center_y + extents.height / 2);
    cairo_show_text(cr, prasthutha_varsham);

    // If hovered or clicked, show previous & next year names
    if (show_adjacent_years) {
        cairo_set_source_rgb(cr, 0.5, 1.0, 1.0); // Cyan text
        cairo_text_extents_t prev_extents, next_extents;
        cairo_text_extents(cr, previous_varsham, &prev_extents);
        cairo_text_extents(cr, next_varsham, &next_extents);

        cairo_move_to(cr, center_x - prev_extents.width / 2, center_y - 40);
        cairo_show_text(cr, previous_varsham);

        cairo_move_to(cr, center_x - next_extents.width / 2, center_y + 40);
        cairo_show_text(cr, next_varsham);
    }
    
    return FALSE;
}

GtkWidget* create_main_window() {

    GtkWidget *window;
    GtkWidget *drawing_area;
    GtkWidget *main_box;  // A box to contain both widgets

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Moon Phases");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box layout to hold the drawing area and label
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Create the phase label
    phase_label = gtk_label_new("amasa");
    // gtk_box_pack_start(GTK_BOX(main_box), phase_label, FALSE, FALSE, 5); //commented for future use for default moon phase highlight

    // Create a drawing area to draw the moon phases
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 500, 500);
    gtk_box_pack_start(GTK_BOX(main_box), drawing_area, TRUE, TRUE, 5);

    // Connect signals
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(on_mouse_click), NULL);
    g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(on_mouse_motion), NULL);
    gtk_widget_add_events(drawing_area, GDK_POINTER_MOTION_MASK);  // Add event mask for mouse motion
    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);

    return window;
}

int main(int argc, char *argv[]) {
    GtkWidget *main_window;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Load moon phase images
    if (!load_moon_phase_images()) {
        return 1;  // Exit if images fail to load
    }

    // get varsham
    get_prasthutha_varsham();

    // Create the main window
    main_window = create_main_window();

    // Show the main window
    gtk_widget_show_all(main_window);

    // Start the GTK main event loop
    gtk_main();

    // Free the loaded images
    for (int i = 0; i < THITHI; i++) {
        g_object_unref(pixbuf[i]);
    }

    return 0;
}
