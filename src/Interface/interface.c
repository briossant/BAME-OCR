#include <complex.h>
#include <gtk/gtk.h>
#include <err.h>
typedef struct UserInterface
{
    GtkBuilder *builder;
    GtkWindow *window;            // Main window
    GtkButton *help_button;       // Start button
    GtkButton *upload_button;     // Stop button
    GtkCheckButton *Step_by_step; // Step by step
    GtkCheckButton *Rotate;
    GtkEntry *insert_angle;
    GtkImage *baseImage;
    GtkWidget *baseContainer;
    GtkImage *solvedImage;
    guint window_size;

} UserInterface;
// Function to handle button click event
static void solve_button_clicked(GtkWidget *widget, gpointer user_data)
{
    g_print("Solving OCR...\n");
    UserInterface *interface = user_data;
    const gchar *text = gtk_entry_get_text(interface->insert_angle);
}

gboolean rotate_check_clicked(GtkWidget *widget, gpointer user_data)
{
    UserInterface *interface = user_data;
    gtk_widget_set_sensitive(interface->insert_angle,
                             gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)));
    return TRUE;
}

void show_warning(GtkWidget *widget, gpointer window)
{

    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_WARNING,
                                    GTK_BUTTONS_OK,
                                    "Grid not detected -- retake the picture or manually rotate it to be straight");
    gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_error(GtkWidget *widget, gpointer window)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Error loading file");
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void help_button_clicked(GtkWidget *widget, gpointer user_data)
{
    GtkWindow *window = GTK_WINDOW(user_data);
    g_print("Help button clicked!\n");
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_CLOSE,
                                    "- Step by step: show all the process that is done by the AI\n- Rotate: If your image is not straight ither auto rotate it or select the angle\n - Upload: Select the picture of the sudoku you want to solve from your computer\n- Solve: button accessible once image is uploaded it will solve the grid with the current parameters till the end\n");
    gtk_window_set_title(GTK_WINDOW(dialog), "Help");
    // gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dialog),
    //       "Texte 2 si necessaire  printf fromat ", );

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    show_error(widget, window);
    show_warning(widget, window);
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

gboolean set_image(GtkWidget *window, GdkEvent *event, gpointer user_data)
{
    UserInterface *interface = user_data;

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->solvedImage);
    if (pixbuf != NULL)
    {
        int img_width = gdk_pixbuf_get_width(pixbuf);
        int img_height = gdk_pixbuf_get_height(pixbuf);

        gint wd_width, wd_height;

        gtk_window_get_size(GTK_WINDOW(window), &wd_width, &wd_height);

        int new_width = (wd_width / 3);
        int new_height = (wd_width / 3);

        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);

        gtk_image_set_from_pixbuf(interface->solvedImage, scaled_pixbuf);

        g_object_unref(scaled_pixbuf);
    }

    GdkPixbuf *pixbuf2 = gtk_image_get_pixbuf(interface->baseImage);
    if (pixbuf2 != NULL)
    {
        int img_width = gdk_pixbuf_get_width(pixbuf2);
        int img_height = gdk_pixbuf_get_height(pixbuf2);

        gint wd_width, wd_height;

        gtk_window_get_size(GTK_WINDOW(window), &wd_width, &wd_height);

        int new_width = (wd_width / 5);
        int new_height = (wd_width / 5);

        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf2, new_width, new_height, GDK_INTERP_BILINEAR);

        gtk_image_set_from_pixbuf(interface->baseImage, scaled_pixbuf);

        g_object_unref(scaled_pixbuf);
    }

    return FALSE;
}

static void upload_button_clicked(GtkWidget *widget, gpointer user_data)
{
    UserInterface *interface = user_data;
    g_print("Uploading Image ...\n");
    GtkWindow *window = interface->window;
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open Image",
                                                    GTK_WINDOW(interface->window),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "Cancel", GTK_RESPONSE_CANCEL,
                                                    "Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    // Set the dialog to select only image files
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Image Files");
    gtk_file_filter_add_mime_type(filter, "image/*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Run the dialog and get the response
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
        // Get the selected file
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GtkWidget *image_widget = gtk_image_new_from_file(filename);
        if (image_widget == NULL)
        {
            g_printerr("Error loading image: %s\n", filename);
            errx(1, "Could not get the image from file path");
        }

        // NEXT LINE IS THE ERROR
        interface->baseImage = GTK_IMAGE(image_widget);
        interface->solvedImage = GTK_IMAGE(image_widget);

        // set_image(window, "size-allocate", user_data);
        gtk_image_set_from_file(interface->baseImage, filename);
        gtk_image_set_from_file(interface->solvedImage, filename);

        // gtk_container_add(GTK_CONTAINER(interface->baseContainer), image_widget);
        //  Process the selected file (you can replace this with your own logic)
        g_print("Selected file: %s\n", filename);

        // Free the filename string
        g_free(filename);
    }

    // Destroy the dialog
    gtk_widget_destroy(dialog);
}

int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    //     gtk_window_set_title(GTK_WINDOW(window), "--- Sudoku Solver --");
    //     gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkButton *help_button = GTK_BUTTON(gtk_builder_get_object(builder, "help_button"));
    GtkCheckButton *step_check = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "step_by_step"));
    GtkCheckButton *rotate_check = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "rotation_check"));
    GtkButton *upload_button = GTK_BUTTON(gtk_builder_get_object(builder, "upload_button"));
    GtkButton *solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "solve_button"));
    gtk_widget_set_sensitive(solve_button, FALSE);
    GtkEntry *insert_angle = GTK_ENTRY(gtk_builder_get_object(builder, "insert_angle"));
    gtk_widget_set_sensitive(insert_angle, FALSE);
    GtkImage *image_base = GTK_IMAGE(gtk_builder_get_object(builder, "base_image"));
    gtk_image_set_from_file(image_base, "Image-Defaults/BaseImage.png");
    GtkWidget *box_base = GTK_WIDGET(gtk_builder_get_object(builder, "left_grid"));

    GtkImage *image_solved = GTK_IMAGE(gtk_builder_get_object(builder, "solved_image"));
    gtk_image_set_from_file(image_solved, "Image-Defaults/Solved_Image.png");
    GtkWidget *box_solved = GTK_WIDGET(gtk_builder_get_object(builder, "box_solved"));

    UserInterface Interface = {
        .builder = builder,
        .window = window,
        .upload_button = upload_button,
        .help_button = help_button,
        .Rotate = rotate_check,
        .insert_angle = insert_angle,
        .Step_by_step = step_check,
        .baseImage = image_base,
        .baseContainer = box_solved,
        .solvedImage = image_solved,
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(help_button, "clicked", G_CALLBACK(help_button_clicked), window);
    g_signal_connect(upload_button, "clicked", G_CALLBACK(upload_button_clicked), &Interface);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_button_clicked), NULL);
    g_signal_connect(window, "size-allocate", G_CALLBACK(set_image), &Interface);
    g_signal_connect(rotate_check, "clicked", G_CALLBACK(rotate_check_clicked), &Interface);

    // G_OBJECT(check_button), "toggled", G_CALLBACK(on_check_button_toggled), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));

    gtk_main();

    return 0;
}
