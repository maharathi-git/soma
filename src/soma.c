#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

GtkWidget *calendar;
GtkWidget *event_window;
GtkWidget *event_entry;
GtkWidget *event_label;
GtkWidget *date_label;
gchar current_event[1024];


int selected_phase = -1;  // -1 means no selection
GtkWidget *phase_label;

#define NUM_PHASES 30

// Array to store the moon phase images
GdkPixbuf *pixbuf[NUM_PHASES];

const char *phase_names[NUM_PHASES] = {
    "amasa", "shukla padyami", "shukla vidiya", "shukla thadiya",
    "shukla chavithi", "shukla panchami", "shukla shashti", "shukla sapthami",
    "shukla ashtami", "shukla navami", "shukla dashami", "shukla ekadashi",
    "shukla dwadashi", "shukla thrayodashi", "shukla chathurdashi", "punnami",
    "krushna padyami", "krushna vidiya", "krushna thadiya", "krushna chavithi",
    "krushna panchami", "krushna shashti", "krushna sapthami", "krushna ashtami",
    "krushna navami", "krushna dashami", "krushna ekadashi", "krushna dwadashi",
    "krushna thrayodashi", "krushna chathurdashi"
};

// Update global variable to track hovered phase
int hovered_phase = -1;

static gboolean on_mouse_click(GtkWidget *widget, GdkEventButton *event, gpointer data) {

    (void)data; //suppress the warnings

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    int center_x = width / 2;
    int center_y = height / 2;
    int radius = MIN(center_x, center_y) - 20;
    
    for (int i = 0; i < NUM_PHASES; i++) {
        double angle = i * (2 * M_PI / NUM_PHASES) - (M_PI / 2);
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

    (void)data; //suppress the warinings
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    int center_x = width / 2;
    int center_y = height / 2;
    int radius = MIN(center_x, center_y) - 20;
    
    hovered_phase = -1;  // Reset hovered phase

    for (int i = 0; i < NUM_PHASES; i++) {
        double angle = i * (2 * M_PI / NUM_PHASES) - (M_PI / 2);
        int img_width = gdk_pixbuf_get_width(pixbuf[i]);
        int img_height = gdk_pixbuf_get_height(pixbuf[i]);

        int x = center_x + (int)(radius * cos(angle)) - img_width / 2;
        int y = center_y + (int)(radius * sin(angle)) - img_height / 2;

        if (event->x >= x && event->x <= (x + img_width) &&
            event->y >= y && event->y <= (y + img_height)) {
            
            hovered_phase = i;  // Set hovered phase
            gtk_widget_queue_draw(widget);  // Redraw the widget to show phase name
            break;
        }
    }
    return TRUE;
}

// Function to load moon phase images
gboolean load_moon_phase_images() {
    const gchar *image_files[NUM_PHASES] = {
        "resource/images/36x36/amasa.png", "resource/images/36x36/shukla_padyami.png", 
        "resource/images/36x36/shukla_thadiya.png", "resource/images/36x36/shukla_vidiya.png", 
        "resource/images/36x36/shukla_chavithi.png", "resource/images/36x36/shukla_panchami.png", 
        "resource/images/36x36/shukla_shashti.png", "resource/images/36x36/shukla_sapthami.png", 
        "resource/images/36x36/shukla_ashtami.png", "resource/images/36x36/shukla_navami.png", 
        "resource/images/36x36/shukla_dashami.png", "resource/images/36x36/shukla_ekadashi.png", 
        "resource/images/36x36/shukla_dwadashi.png", "resource/images/36x36/shukla_thrayodashi.png", 
        "resource/images/36x36/shukla_chathurdashi.png", "resource/images/36x36/punnami.png", 
        "resource/images/36x36/krushna_padyami.png", "resource/images/36x36/krushna_thadiya.png", 
        "resource/images/36x36/krushna_vidiya.png", "resource/images/36x36/krushna_chavithi.png", 
        "resource/images/36x36/krushna_panchami.png", "resource/images/36x36/krushna_shashti.png", 
        "resource/images/36x36/krushna_sapthami.png", "resource/images/36x36/krushna_ashtami.png", 
        "resource/images/36x36/krushna_navami.png", "resource/images/36x36/krushna_dashami.png", 
        "resource/images/36x36/krushna_ekadashi.png", "resource/images/36x36/krushna_dwadashi.png", 
        "resource/images/36x36/krushna_thrayodashi.png", "resource/images/36x36/krushna_chathurdashi.png"
    };

    for (int i = 0; i < NUM_PHASES; i++) {
        pixbuf[i] = gdk_pixbuf_new_from_file(image_files[i], NULL);
        if (pixbuf[i] == NULL) {
            g_printerr("Failed to load image: %s\n", image_files[i]);
            return FALSE;
        }
    }
    return TRUE;
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr) {

    // Set black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    int center_x = width / 2;
    int center_y = height / 2;
    int radius = MIN(center_x, center_y) - 40;

    for (int i = 0; i < NUM_PHASES; i++) {
        double angle = i * (2 * M_PI / NUM_PHASES) - (M_PI / 2);
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
        }

         // If the phase is hovered over, or clicked, draw the phase name below it
        if (i == hovered_phase || i == selected_phase) {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // White text
            cairo_set_font_size(cr, 12);
            cairo_move_to(cr, x + img_width / 2 - 30, y + img_height + 20);  // Position the text below the image
            cairo_show_text(cr, phase_names[i]);
        }
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

    // Create the main window
    main_window = create_main_window();

    // Show the main window
    gtk_widget_show_all(main_window);

    // Start the GTK main event loop
    gtk_main();

    // Free the loaded images
    for (int i = 0; i < NUM_PHASES; i++) {
        g_object_unref(pixbuf[i]);
    }
    return 0;
}
