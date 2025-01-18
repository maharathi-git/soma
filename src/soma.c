// calendar.c
#include <gtk/gtk.h>
#include <time.h>

GtkWidget *calendar;
GtkWidget *event_window;
GtkWidget *event_entry;
GtkWidget *event_label;
GtkWidget *date_label;
gchar current_event[1024];

// Function to update the calendar widget
void update_calendar(GtkWidget *widget, gpointer user_data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Get selected date
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);

    tm.tm_year = year - 1900;
    tm.tm_mon = month;
    tm.tm_mday = day;
    
    // Format and display selected date
    gchar *date_text = g_strdup_printf("Selected Date: %d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    gtk_label_set_text(GTK_LABEL(date_label), date_text);
    g_free(date_text);
}

// Function to show event window for the selected date
void show_event_window(GtkWidget *widget, gpointer user_data) {
    gtk_window_present(GTK_WINDOW(event_window));
}

// Function to save the event for the selected date
void save_event(GtkWidget *widget, gpointer user_data) {
    const gchar *event = gtk_entry_get_text(GTK_ENTRY(event_entry));
    if (event != NULL && event[0] != '\0') {
        g_strlcpy(current_event, event, sizeof(current_event));
        gtk_label_set_text(GTK_LABEL(event_label), current_event);
        gtk_entry_set_text(GTK_ENTRY(event_entry), "");
    }
}

// Function to initialize the application window
GtkWidget* create_main_window() {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button_show_event, *button_save_event;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "పంచాంగం");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a box container to arrange widgets vertically
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create a calendar widget
    calendar = gtk_calendar_new();
    gtk_box_pack_start(GTK_BOX(box), calendar, TRUE, TRUE, 0);

    // Label for selected date
    date_label = gtk_label_new("Selected Date: None");
    gtk_box_pack_start(GTK_BOX(box), date_label, FALSE, FALSE, 0);

    // Button to show event window
    button_show_event = gtk_button_new_with_label("Show Event");
    g_signal_connect(button_show_event, "clicked", G_CALLBACK(show_event_window), NULL);
    gtk_box_pack_start(GTK_BOX(box), button_show_event, FALSE, FALSE, 0);

    // Update calendar when date is selected
    g_signal_connect(calendar, "day-selected", G_CALLBACK(update_calendar), NULL);

    return window;
}

// Function to initialize the event window
GtkWidget* create_event_window() {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button_save_event;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Event for Selected Date");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Create a box container to arrange widgets vertically
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Entry field for event description
    event_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), event_entry, FALSE, FALSE, 0);

    // Save event button
    button_save_event = gtk_button_new_with_label("Save Event");
    g_signal_connect(button_save_event, "clicked", G_CALLBACK(save_event), NULL);
    gtk_box_pack_start(GTK_BOX(box), button_save_event, FALSE, FALSE, 0);

    // Label to display saved event
    event_label = gtk_label_new("No event for this date.");
    gtk_box_pack_start(GTK_BOX(box), event_label, FALSE, FALSE, 0);

    return window;
}

int main(int argc, char *argv[]) {
    GtkWidget *main_window;

    gtk_init(&argc, &argv);

    // Create main window and event window
    main_window = create_main_window();
    event_window = create_event_window();

    // Show main window
    gtk_widget_show_all(main_window);
    gtk_main();
    
    return 0;
}
