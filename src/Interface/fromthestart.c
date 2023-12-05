#include <complex.h>
#include <gtk/gtk.h>

// Function to handle button click event
static void solve_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Solving OCR...\n");
}

gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    // TODO: resize images

    // Gets the rectangle.
    GdkRectangle *r = user_data;

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    r->x = (width - r->width) / 2;
    r->y = (height - r->height) / 2;

    return FALSE;
}

void update_image(GtkWidget *image, GtkWidget *box) {
    const GdkPixbuf *piximg = gtk_image_get_pixbuf(GTK_IMAGE(image));
    int width_img = gdk_pixbuf_get_width(piximg);
    int height_img = gdk_pixbuf_get_height(piximg);

    const GdkPixbuf *pixgrid = gtk_image_get_pixbuf(GTK_IMAGE(box));
    int width_grid = gdk_pixbuf_get_width(pixgrid);
    int height_grid = gdk_pixbuf_get_height(pixgrid);

    // Calculer les nouvelles dimensions
    int new_width, new_height;

    new_width = width_grid;
    new_height = height_grid;

    // if (width_img > height_img) {
    //     // L'image est plus large que haute
    //     new_width = width_grid;
    //     new_height = (int)((double)height_grid * (double)height_img / (double)width_img);
    // } else {
    //     // L'image est plus haute que large ou a les mêmes dimensions
    //     new_height = height_grid;
    //     new_width = (int)((double)width_grid * (double)width_img / (double)height_img);
    // }

    // Redimensionner l'image
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(piximg, new_width, new_height, GDK_INTERP_BILINEAR);

    // Mettre à jour l'image avec la nouvelle taille
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaled_pixbuf);

    // Libérer la mémoire utilisée par le pixbuf redimensionné
    g_object_unref(scaled_pixbuf);
}

int main(int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "fromthestart.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    //     gtk_window_set_title(GTK_WINDOW(window), "--- Sudoku Solver --");
    //     gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkButton *help_button = GTK_BUTTON(gtk_builder_get_object(builder, "Help")); // TODO: rename labels
    GtkCheckButton *step_check = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "training_cb"));
    GtkCheckButton *rotate_check = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "training_cb"));
    GtkButton *upload_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));
    // GtkWidget *image_base = Image();
    // image.set_from_file("/home/your_username/path/to/your/image.png")

    GtkWidget *image_base = GTK_IMAGE(gtk_builder_get_object(builder, "Base Image"));
    gtk_image_set_from_file(image_base, "Image-Defaults/BaseImage.png");

    GtkWidget *image_solved = GTK_IMAGE(gtk_builder_get_object(builder, "Solved Image"));
    gtk_image_set_from_file(image_solved, "Image-Defaults/Solved_Image.png");

    GtkWidget* box_base = gtk_builder_get_object(builder, "left_grid");
    GtkWidget* box_solved = gtk_builder_get_object(builder, "Grid Solved Image");

    update_image(image_base, box_base);
    update_image(image_solved, box_solved);

    // /* -->does not work
    //     //ADD color
    //     GdkRGBA color;
    //     gdk_rgba_parse(&color, "blue");  // Set color to blue
    //     gtk_widget_override_background_color(button, GTK_STATE_NORMAL, &color);
    // */
    //     // Check box with "Step by Step" label
    //     GtkWidget *check_button = gtk_check_button_new_with_label("Step by Step");
    //     gtk_box_pack_start(GTK_BOX(vbox), check_button, TRUE, TRUE, 0);
    // // "solved image will appear here once image is uploaded

    //     if (image_base == NULL) {
    //     g_printerr("Error loading base image.\n");
    //     }

    //     // Add an image called "Solved"

    //     if (image_solved == NULL) {
    //     g_printerr("Error loading solved image.\n");
    //     }

    // // Create a horizontal box for the upload button and the "Solved" image
    //     GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    //     gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    //     // Add a button for Sudoku image upload
    //     GtkWidget *upload_button = gtk_button_new_with_label("Sudoku Image Upload");
    //     gtk_box_pack_start(GTK_BOX(hbox), upload_button, TRUE, TRUE, 0);

    //     gtk_box_pack_start(GTK_BOX(hbox), image_base, TRUE, TRUE, 0);
    //     // Create a "Solve" button
    //     GtkWidget *solve_button = gtk_button_new_with_label("Solve");
    //     g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_button_clicked), NULL);
    //     gtk_box_pack_start(GTK_BOX(vbox), solve_button, TRUE, TRUE, 0);

    //     gtk_box_pack_start(GTK_BOX(vbox), image_solved, TRUE, TRUE, 0);
    // Show all elements
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
