#include <complex.h>
#include <gtk/gtk.h>
#include <err.h>
typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkButton* help_button;        // Start button
    GtkButton* upload_button;         // Stop button
    GtkCheckButton* Step_by_step; // Step by step
    GtkCheckButton* Rotate;
    GtkImage* baseImage;
    GtkWidget *baseContainer;

 } UserInterface;
// Function to handle button click event
static void solve_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Solving OCR...\n");
}

static void upload_button_clicked(GtkWidget *widget, gpointer user_data)
{
    UserInterface *interface = (UserInterface *)user_data;
    g_print("Uploading Image ...\n");
    GtkWindow *window = GTK_WINDOW(user_data);
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

    if (response == GTK_RESPONSE_ACCEPT) {
        // Get the selected file
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GtkWidget *image_widget = gtk_image_new_from_file(filename);
        if (image_widget == NULL) {
        g_printerr("Error loading image: %s\n", filename);
        errx(1, "Could not get the image from file path");
        }

        // NEXT LINE IS THE ERROR 
        interface->baseImage = GTK_IMAGE(image_widget);
        //gtk_container_add(GTK_CONTAINER(interface->baseContainer), image_widget);
        // Process the selected file (you can replace this with your own logic)
        g_print("Selected file: %s\n", filename);

        // Free the filename string
        g_free(filename);
    }

    // Destroy the dialog
    gtk_widget_destroy(dialog);
}
void show_warning(GtkWidget *widget, gpointer window) {
    
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

void show_error(GtkWidget *widget, gpointer window) {

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

static void help_button_clicked(GtkWidget* widget, gpointer user_data )
{
  GtkWindow *window = GTK_WINDOW(user_data);
  g_print("Help button clicked!\n");
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
               GTK_DIALOG_DESTROY_WITH_PARENT,
               GTK_MESSAGE_INFO,
               GTK_BUTTONS_CLOSE,
               "- Step by step: show all the process that is done by the AI\n- Rotate: If your image is not straight ither auto rotate it or select the angle\n - Upload: Select the picture of the sudoku you want to solve from your computer\n- Solve: button accessible once image is uploaded it will solve the grid with the current parameters till the end\n" );
     gtk_window_set_title(GTK_WINDOW(dialog), "Help");                                                                      
     //gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dialog),
     //      "Texte 2 si necessaire  printf fromat ", );

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

void update_image(GtkWidget* container , GtkImage* image ) {
    

    
    const GdkPixbuf *piximg = gtk_image_get_pixbuf(image);
    int width_img = gdk_pixbuf_get_width(piximg);
    int height_img = gdk_pixbuf_get_height(piximg);
    

    int window_width, window_height;
    gtk_window_get_size(GTK_WINDOW(window), &window_width, &window_height);

    //g_print("with %d, height %d", width_img,height_img);
    // Calculer les nouvelles dimensions
    int new_width, new_height;

     
    new_width = window_width/2;//gtk_widget_get_allocated_width(GTK_WIDGET(container));
    new_height = window_height/2;//gtk_widget_get_allocated_height(GTK_WIDGET(container));

    double scale_x = (double)new_width/(double)width_img;
    double scale_y = (double)new_height/(double)height_img;
    g_print("Nwith %d, NHeight %d", new_width,new_height);
    int bits_sample =gdk_pixbuf_get_bits_per_sample (piximg);
    GdkPixbuf* new_image = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, bits_sample, new_width, new_height);
    // Redimensionner l'image
    gdk_pixbuf_scale(piximg,new_image, 0, 0,new_width, new_height, 0,0,scale_x,scale_y,GDK_INTERP_BILINEAR);

    // Mettre à jour l'image avec la nouvelle taille
    gtk_image_set_from_pixbuf(image, new_image);

    // Libérer la mémoire utilisée par le pixbuf redimensionné
    //g_object_unref(scaled_pixbuf);
}




int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "fromthestart2.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    //     gtk_window_set_title(GTK_WINDOW(window), "--- Sudoku Solver --");
    //     gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkButton *help_button = GTK_BUTTON(gtk_builder_get_object(builder, "help_button")); // TODO: rename labels
    GtkCheckButton *step_check = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "training_cb"));
    GtkCheckButton *rotate_check = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "training_cb"));
    GtkButton *upload_button = GTK_BUTTON(gtk_builder_get_object(builder, "upload_button"));
    GtkButton *solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "solve_button"));
   
    
    GtkImage *image_base = GTK_IMAGE(gtk_builder_get_object(builder, "Base Image"));
    gtk_image_set_from_file(image_base, "Image-Defaults/BaseImage.png");
    GtkWidget * box_base =GTK_WIDGET(gtk_builder_get_object(builder, "left_grid"));

    GtkImage *image_solved = GTK_IMAGE(gtk_builder_get_object(builder, "solved_image"));
    gtk_image_set_from_file(image_solved, "Image-Defaults/Solved_Image.png");
    GtkWidget* box_solved =GTK_WIDGET(gtk_builder_get_object(builder, "box_solved"));
    

    UserInterface Intrerface ={
                   .window = window,
                   .upload_button = upload_button,
                   .help_button =help_button,
                   .Rotate = rotate_check,
                   .Step_by_step= step_check,
                   .baseImage=image_solved,
                   .baseContainer= box_solved,
     };
    
    update_image(box_base, image_base);
    update_image(box_solved, image_solved);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(help_button, "clicked", G_CALLBACK(help_button_clicked), window);
    g_signal_connect(upload_button, "clicked", G_CALLBACK(upload_button_clicked), window);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_button_clicked), NULL);
	
//	g_signal_connect(image_solved, "expose-event", G_CALLBACK(resize_image), (gpointer)window);
//	g_signal_connect(image_base, "expose-event", G_CALLBACK(resize_image), (gpointer)window);
//	gtk_container_add(GTK_CONTAINER(window), image_base);

	gtk_widget_show_all(GTK_WIDGET(window));
    // Start the GTK main loop
    gtk_main();

    return 0;
}
