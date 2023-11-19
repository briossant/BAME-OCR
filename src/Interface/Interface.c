#include <gtk/gtk.h>

// Function to handle button click event
static void solve_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Solving OCR...\n");
    // Add OCR solving logic here
}

int main(int argc, char *argv[])
{
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "BAME-OCR");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold the elements
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create an "Insert Image" label (placeholder)
    GtkWidget *insert_label = gtk_label_new("Insert Image Placeholder");
    gtk_box_pack_start(GTK_BOX(vbox), insert_label, TRUE, TRUE, 0);

    // Create a "Solve" button
    GtkWidget *solve_button = gtk_button_new_with_label("Solve");
    g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), solve_button, TRUE, TRUE, 0);

    // Show all elements
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}

