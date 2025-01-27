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

#define NUM_PHASES 30

// Array to store the moon phase images
GdkPixbuf *pixbuf[NUM_PHASES];

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

// Function to draw moon phases in a circle
static gboolean on_draw(GtkWidget *widget, cairo_t *cr) {
    // Get the dimensions of the drawing area
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    // Set up the center of the circle
    int center_x = width / 2;
    int center_y = height / 2;

    // Set up the radius of the circle
    int radius = MIN(center_x, center_y) - 20;

    for (int i = 0; i < NUM_PHASES; i++) {
        // Calculate the position of each image
        double angle = i * (2 * M_PI / NUM_PHASES) - (M_PI / 2);  // angle in radians
        int x = center_x + (int)(radius * cos(angle)) - gdk_pixbuf_get_width(pixbuf[i]) / 2;
        int y = center_y + (int)(radius * sin(angle)) - gdk_pixbuf_get_height(pixbuf[i]) / 2;

        // Draw the moon phase image at the calculated position
        gdk_cairo_set_source_pixbuf(cr, pixbuf[i], x, y);
        cairo_paint(cr);
    }

    return FALSE;  // We don't need to keep redrawing
}

// Function to create the window and drawing area
GtkWidget* create_main_window() {
    GtkWidget *window;
    GtkWidget *drawing_area;
    GtkCssProvider *css_provider;
    GdkDisplay *display;
    GdkScreen *screen;

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Moon Phases");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

     // Load the CSS file
    css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(css_provider, g_file_new_for_path("resource/css/style.css"), NULL);

    // Get the default display and screen
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    // Apply the CSS to the screen
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Create a drawing area to draw the moon phases
    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Connect the draw signal to our custom draw function
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), NULL);

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
