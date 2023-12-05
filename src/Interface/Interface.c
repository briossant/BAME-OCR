#include <complex.h>
#include <gtk/gtk.h>

// Function to handle button click event
static void solve_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Solving OCR...\n");
}

void on_configure()
{
    //TODO: must reorganize the page and make the images fit
}

int main(int argc, char *argv[])
{
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "Interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "--- Sudoku Solver --");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold the elements
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Add a button with "?" label
    GtkWidget *button = gtk_button_new_with_label("?");
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

/* -->does not work 
    //ADD color 
    GdkRGBA color;
    gdk_rgba_parse(&color, "blue");  // Set color to blue
    gtk_widget_override_background_color(button, GTK_STATE_NORMAL, &color);
*/ 
    // Check box with "Step by Step" label
    GtkWidget *check_button = gtk_check_button_new_with_label("Step by Step");
    gtk_box_pack_start(GTK_BOX(vbox), check_button, TRUE, TRUE, 0);
// "solved image will appear here once image is uploaded

    GtkWidget *image_base = gtk_image_new_from_file("Image_Defaults/BaseImage.png");
    if (image_base == NULL) {
    g_printerr("Error loading base image.\n");
    }

    // Add an image called "Solved"
    
    GtkWidget *image_solved = gtk_image_new_from_file("Image_Defaults/Solved_Image.png");
    if (image_solved == NULL) {
    g_printerr("Error loading solved image.\n");
    }
    

// Create a horizontal box for the upload button and the "Solved" image
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    // Add a button for Sudoku image upload
    GtkWidget *upload_button = gtk_button_new_with_label("Sudoku Image Upload");
    gtk_box_pack_start(GTK_BOX(hbox), upload_button, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(hbox), image_base, TRUE, TRUE, 0);
    // Create a "Solve" button
    GtkWidget *solve_button = gtk_button_new_with_label("Solve");
    g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), solve_button, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), image_solved, TRUE, TRUE, 0);
    // Show all elements
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}

