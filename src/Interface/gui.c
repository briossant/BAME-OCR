#include <gtk/gtk.h>

GtkBuilder *builder;
GtkWidget *window_main;
GtkWidget *button_click;
GtkWidget *dialog_message;

// Fonction appelée lorsque le bouton est cliqué
void on_button_clicked(GtkWidget *widget, gpointer data)
{
    // Afficher le pop-up (dialog_message)
    gtk_widget_show(dialog_message);
}

// Fonction appelée lorsque l'utilisateur répond au pop-up
void on_dialog_response(GtkWidget *widget, gpointer data)
{
    // Cacher le pop-up après la réponse
    gtk_widget_hide(widget);
}

int main(int argc, char *argv[])
{
    // Initialiser GTK
    gtk_init(NULL, NULL);

    // Charger l'interface depuis le fichier Glade
    builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Récupérer les widgets
    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    button_click = GTK_WIDGET(gtk_builder_get_object(builder, "button_click"));
    dialog_message = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_message"));

    // Connecter les signaux aux fonctions de rappel
    // g_signal_connect(window_main, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button_click, "clicked", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect(dialog_message, "response", G_CALLBACK(on_dialog_response), NULL);

    // Afficher la fenêtre principale
    gtk_widget_show_all(window_main);

    // Lancer la boucle principale GTK
    gtk_main();

    return 0;
}
