#include <complex.h>
#include <err.h>
#include <gtk/gtk.h>

typedef struct UserInterface {
  GtkBuilder *builder;
  GtkWindow *window;
  GtkButton *help_button;
  GtkButton *upload_button;
  GtkButton *solve_button;
  GtkCheckButton *Step_by_step;
  GtkCheckButton *Rotate;
  GtkEntry *insert_angle;
  GtkImage *baseImage;
  GtkWidget *baseContainer;
  GtkImage *solvedImage;
  guint window_size;
  char *filename;
  char *filename_solved;
  GtkButton *save_button;

} UserInterface;

void show_warning(char *message, gpointer window) {
  if (message == NULL) {
    message = "Grid not detected -- retake the picture or manually rotate it "
              "to be straight";
  }

  GtkWidget *dialog;
  dialog =
      gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
                             GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, message);
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_error(char *message, gpointer window) {
  GtkWidget *dialog;
  dialog =
      gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
                             GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, message);
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

int isInteger(const char *text) {
  char *endptr;
  strtol(text, &endptr, 10);

  // Check if conversion was successful and the entire string was consumed
  return (*text != '\0' && *endptr == '\0');
}

// Function to handle button click event
static void solve_button_clicked(GtkWidget *widget, gpointer user_data) {
  (void)widget;
  UserInterface *interface = user_data;
  const gchar *text = gtk_entry_get_text(interface->insert_angle);
  char *def = "Default: Automatic";
  int need_rotate = 0;
  int is_automatic = 1;
  int angle = 0;

  gboolean step_by_step =
      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(interface->Step_by_step));

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(interface->Rotate)) ==
      TRUE) {
    need_rotate = 1;
    if (strcmp(def, text) != 0) {
      if (!isInteger(text)) {
        show_error("The angle must be an integer", interface->window);
        gtk_entry_set_text(interface->insert_angle, "Default: Automatic");
        return;
      } else {
        angle = atoi(text);
        is_automatic = 0;
      }
    }
  }
}

gboolean rotate_check_clicked(GtkWidget *widget, gpointer user_data) {
  UserInterface *interface = user_data;

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == FALSE) {
    gtk_entry_set_text(interface->insert_angle, "Default: Automatic");
  }
  gtk_widget_set_sensitive(
      GTK_WIDGET(interface->insert_angle),
      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)));
  return TRUE;
}

static void help_button_clicked(GtkWidget *widget, gpointer user_data) {
  (void)widget;
  UserInterface *interface = user_data;
  GtkWindow *window = interface->window;
  GtkWidget *dialog = gtk_message_dialog_new(
      GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO,
      GTK_BUTTONS_CLOSE,
      "- Step by step: show all the process that is done by the AI\n- Rotate: "
      "If your image is not straight you can leave the autor rotate default "
      "text or insert the angle(must be an integer)\n - Upload: Select the "
      "picture of the sudoku you want to solve from your computer\n- Solve: "
      "button accessible once image is uploaded it will solve the grid with "
      "the current parameters till the end\n");
  gtk_window_set_title(GTK_WINDOW(dialog), "Help");

  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
  (void)event;
  // TODO: resize images

  // Gets the rectangle.
  GdkRectangle *r = user_data;

  int width = gtk_widget_get_allocated_width(widget);
  int height = gtk_widget_get_allocated_height(widget);

  r->x = (width - r->width) / 2;
  r->y = (height - r->height) / 2;

  return FALSE;
}

gboolean set_image(GtkWidget *window, GdkEvent *event, gpointer user_data) {
  (void)event;
  UserInterface *interface = user_data;

  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->solvedImage);
  if (pixbuf != NULL) {

    gint wd_width, wd_height;

    gtk_window_get_size(GTK_WINDOW(window), &wd_width, &wd_height);

    int new_width = (wd_width / 3);
    int new_height = (wd_width / 3);

    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
        pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(interface->solvedImage, scaled_pixbuf);

    g_object_unref(scaled_pixbuf);
  }

  GdkPixbuf *pixbuf2 = gtk_image_get_pixbuf(interface->baseImage);
  if (pixbuf2 != NULL) {
    gint wd_width, wd_height;

    gtk_window_get_size(GTK_WINDOW(window), &wd_width, &wd_height);

    int new_width = (wd_width / 5);
    int new_height = (wd_width / 5);

    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
        pixbuf2, new_width, new_height, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(interface->baseImage, scaled_pixbuf);

    g_object_unref(scaled_pixbuf);
  }

  return FALSE;
}

void fix_size(gpointer user_data) {
  UserInterface *interface = user_data;

  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(interface->solvedImage);
  if (pixbuf != NULL) {

    int new_width = 300;
    int new_height = 300;

    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
        pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(interface->solvedImage, scaled_pixbuf);

    g_object_unref(scaled_pixbuf);
  }

  GdkPixbuf *pixbuf2 = gtk_image_get_pixbuf(interface->baseImage);
  if (pixbuf2 != NULL) {

    int new_width = 200;
    int new_height = 200;

    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
        pixbuf2, new_width, new_height, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(interface->baseImage, scaled_pixbuf);

    g_object_unref(scaled_pixbuf);
  }
}
static void upload_button_clicked(GtkWidget *widget, gpointer user_data) {
  (void)widget;
  UserInterface *interface = user_data;
  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Open Image", GTK_WINDOW(interface->window), GTK_FILE_CHOOSER_ACTION_OPEN,
      "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

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
    // realloc(interface->filename, 999*sizeof(char));
    interface->filename = filename;
    // realloc(interface->filename_solved, 999*sizeof(char));
    strcpy(interface->filename_solved, filename);
    GtkWidget *image_widget = gtk_image_new_from_file(filename);
    if (image_widget == NULL) {
      g_printerr("Error loading image: %s\n", filename);
      errx(1, "Could not get the image from file path");
    }

    // set_image(window, "size-allocate", user_data);
    gtk_image_set_from_file(interface->baseImage, filename);
    gtk_image_set_from_file(interface->solvedImage, filename);

    fix_size(user_data);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->solve_button), TRUE);

    // Free the filename string
    g_free(filename);
  }
  // Destroy the dialog
  gtk_widget_destroy(dialog);
}

gboolean save_button_clicked(GtkWidget *widget, GdkEvent *event,
                             gpointer user_data) {
  (void)event;
  // Récupérer l'objet GtkImage à partir des données utilisateur
  UserInterface *interface = user_data;
  GtkImage *image = interface->solvedImage;

  // Récupérer le GdkPixbuf associé à l'image
  // GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);
  GdkPixbuf *pixbuf =
      gdk_pixbuf_new_from_file(interface->filename_solved, NULL);

  if (pixbuf != NULL) {
    // Demander à l'utilisateur le nom du fichier de sauvegarde
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Save Image", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel",
        GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);

    // Ajouter un filtre d'extension pour les fichiers image
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Image Files");
    gtk_file_filter_add_mime_type(filter, "image/*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Exécuter la boîte de dialogue et obtenir la réponse de l'utilisateur
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
      // Obtenir le chemin du fichier sélectionné
      char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

      // Sauvegarder le pixbuf dans le fichier
      gdk_pixbuf_save(pixbuf, filename, "png", NULL, NULL);

      // Libérer la mémoire du chemin du fichier
      g_free(filename);
    }

    // Détruire la boîte de dialogue
    gtk_widget_destroy(dialog);
  }
  return TRUE;
}

int main() {
  // Initializes GTK.
  gtk_init(NULL, NULL);

  // Loads the UI description and builds the UI.
  // (Exits if an error occurs.)
  GtkBuilder *builder = gtk_builder_new();
  GError *error = NULL;
  if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  // Gets the widgets.
  GtkWindow *window =
      GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));

  GtkButton *help_button =
      GTK_BUTTON(gtk_builder_get_object(builder, "help_button"));
  GtkCheckButton *step_check =
      GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "step_by_step"));
  GtkCheckButton *rotate_check =
      GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "rotation_check"));
  GtkButton *upload_button =
      GTK_BUTTON(gtk_builder_get_object(builder, "upload_button"));
  GtkButton *solve_button =
      GTK_BUTTON(gtk_builder_get_object(builder, "solve_button"));
  gtk_widget_set_sensitive(GTK_WIDGET(solve_button), FALSE);
  GtkEntry *insert_angle =
      GTK_ENTRY(gtk_builder_get_object(builder, "insert_angle"));
  gtk_widget_set_sensitive(GTK_WIDGET(insert_angle), FALSE);
  GtkImage *image_base =
      GTK_IMAGE(gtk_builder_get_object(builder, "base_image"));
  gtk_image_set_from_file(image_base, "Image-Defaults/BaseImage.png");
  // GtkWidget *box_base = GTK_WIDGET(gtk_builder_get_object(builder,
  // "left_grid"));
  GtkImage *image_solved =
      GTK_IMAGE(gtk_builder_get_object(builder, "solved_image"));
  gtk_image_set_from_file(image_solved, "Image-Defaults/Solved_Image.png");
  GtkWidget *box_solved =
      GTK_WIDGET(gtk_builder_get_object(builder, "box_solved"));
  GtkButton *save_button =
      GTK_BUTTON(gtk_builder_get_object(builder, "save_button"));

  UserInterface Interface = {
      .builder = builder,
      .window = window,
      .upload_button = upload_button,
      .help_button = help_button,
      .solve_button = solve_button,
      .Rotate = rotate_check,
      .insert_angle = insert_angle,
      .Step_by_step = step_check,
      .baseImage = image_base,
      .baseContainer = box_solved,
      .solvedImage = image_solved,
      .filename = malloc(999 * sizeof(char)),
      .filename_solved = "Image-Defaults/Solved_Image.png",
      .save_button = save_button,
  };

  // Interface.filename_solved = "Image-Defaults/Solved_Image.png";

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(help_button, "clicked", G_CALLBACK(help_button_clicked),
                   &Interface);
  g_signal_connect(upload_button, "clicked", G_CALLBACK(upload_button_clicked),
                   &Interface);
  g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_button_clicked),
                   &Interface);
  g_signal_connect(window, "size-allocate", G_CALLBACK(set_image), &Interface);
  g_signal_connect(rotate_check, "clicked", G_CALLBACK(rotate_check_clicked),
                   &Interface);
  g_signal_connect(save_button, "clicked", G_CALLBACK(save_button_clicked),
                   &Interface);

  gtk_widget_show_all(GTK_WIDGET(window));

  gtk_main();

  return 0;
}
