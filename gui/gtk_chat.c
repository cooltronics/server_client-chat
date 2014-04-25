


//#include <gtk/gtk.h>
#include "/usr/local/include/gtk-2.0/gtk/gtk.h"

#define CHAT_BUTTON_CLICK_STRING "Button clicked.\n"
gboolean add_button(GtkWidget **,GtkWidget *,gint x,gint y,gchar *);
/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below. */
static void hello( GtkWidget *widget,
                   gpointer   data )
{
    //g_print ("Hello World\n");
	g_print ((const gchar *)data);
}

static gboolean button_press( GtkWidget *widget,
                              GdkEvent *event )
{

    if (event->type == GDK_BUTTON_PRESS) {
        GdkEventButton *bevent = (GdkEventButton *) event; 
    //    gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
    //                    bevent->button, bevent->time);
        /* Tell calling code that we have handled this event; the buck
         * stops here. */
        return TRUE;
    }

    /* Tell calling code that we have not handled this event; pass it on. */
    return FALSE;
}

static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    g_print ("Delete event occurred\n");
	gtk_main_quit ();
    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete-event". */

    return TRUE;
}

/* Another callback */
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}

gboolean add_button(GtkWidget **_windowFixed,GtkWidget *button,gint x,gint y,gchar *buttonName)
{
	button = gtk_button_new_with_label(buttonName);
	gtk_widget_set_size_request(button, 100, 25);
	gtk_fixed_put(GTK_FIXED(*_windowFixed), button, x, y);
	gtk_widget_show(button);
	return TRUE;
}
int create_menu(GtkWidget **window,GtkWidget **vbox)
{
	GtkWidget *file_menu,*open_item,*save_item,*quit_item;
	GtkWidget *file_menu1,*item1,*item2,*item3;
	GtkWidget *file_item,*file_item1;
	GtkWidget *menu_bar,*side_bar;
	file_menu = gtk_menu_new ();    /* Don't need to show menus */
	file_menu1 = gtk_menu_new ();    /* Don't need to show menus */

	/* Create the menu items */
	open_item = gtk_menu_item_new_with_label ("Open");
	save_item = gtk_menu_item_new_with_label ("Save");
	quit_item = gtk_menu_item_new_with_label ("Quit");
	/* Add them to the menu */
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), open_item);
	gtk_widget_show (open_item);
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), save_item);
	gtk_widget_show (save_item);
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), quit_item);
	gtk_widget_show (quit_item);

	/* Create the menu items */
	item1 = gtk_menu_item_new_with_label ("online");
	item2 = gtk_menu_item_new_with_label ("offline");
	item3 = gtk_menu_item_new_with_label ("Busy");
	/* Add them to the menu */
    	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu1), item1);
	gtk_widget_show (item1);
    	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu1), item2);
	gtk_widget_show (item2);
    	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu1), item3);
	gtk_widget_show (item3);

	file_item = gtk_menu_item_new_with_label ("File");
	file_item1 = gtk_menu_item_new_with_label ("Status");
    	gtk_widget_show (file_item);
    	gtk_widget_show (file_item1);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_item), file_menu);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_item1), file_menu1);


 /* Create a menu-bar to hold the menus and add it to our main window */
    	menu_bar = gtk_menu_bar_new ();
    	gtk_box_pack_start (GTK_BOX (*vbox), menu_bar, FALSE, FALSE, 2);
    	gtk_widget_show (menu_bar);
	gtk_widget_show (*vbox);

	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), file_item);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), file_item1 );
	return 0;
}

int main( int   argc,
          char *argv[] )
{
    /* GtkWidget is the storage type for widgets */
    GtkWidget *window;
//	GtkWindow *wind;
	GtkWindowPosition WindowPosition;
	GtkWidget *vbox,*hbox;

    GtkWidget *button[4],*button1;
//	GtkLinkButton *btn;
//	gpointer my_data = "Button clicked\n";
	gchar url[128] = {0};
	gfloat xalign,yalign;
	gboolean ret;
	GtkInfoBar * InfoBarButton;
	gint resid;
    
    /* This is called in all GTK applications. Arguments are parsed
     * from the command line and are returned to the application. */
    gtk_init (&argc, &argv);
    
    /* create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	/*Sets the title of the GtkWindow. The title of a window will be displayed 		in its title bar; on the X Window System, the title bar is rendered by the 		window manager, so exactly how the title appears to users may vary according 		to a user's exact configuration. The title should help a user distinguish 		this window from other windows they may have open. A good title might
	include the application name and current document filename, for example.*/
	gtk_window_set_title(GTK_WINDOW (window),"Radixweb");

	/*Sets a position constraint for this window. If the old or new constraint
	is GTK_WIN_POS_CENTER_ALWAYS, this will also cause the window to be
	repositioned to satisfy the new constraint.*/
	gtk_window_set_position (GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request (GTK_WIDGET (window), 400, 400);
    /* When the window is given the "delete-event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect (window, "delete-event",
		      G_CALLBACK (delete_event), NULL);
    
    /* Here we connect the "destroy" event to a signal handler.  
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete-event" callback. */
    g_signal_connect (window, "destroy",
		      G_CALLBACK (destroy), NULL);
    
    /* Sets the border width of the window. */
    gtk_container_set_border_width (GTK_CONTAINER (window), 5);
    
	/*Creates a new GtkLinkButton containing a label.
	uri :
		a valid URI
	label :
		the text of the button. [allow-none]
	Returns :
		a new link button widget. [transfer none]*/
	//button[0] = gtk_link_button_new_with_label ("https://developer.gnome.org/gtk2/stable/","Chat");
	//button[1] = gtk_link_button_new_with_label ("https://developer.gnome.org/gtk2/stable/","Ratnesh2");
	//button[2] = gtk_link_button_new_with_label ("https://developer.gnome.org/gtk2/stable/","Ratnesh3");
	//button[3] = gtk_link_button_new_with_label ("https://developer.gnome.org/gtk2/stable/","Ratnesh4");

//	InfoBarButton = gtk_info_bar_new();
//	gtk_info_bar_add_button(InfoBarButton,"text1",resid);
//	gtk_info_bar_add_button(InfoBarButton,"text2",resid);

//	gtk_button_set_focus_on_click(button,FALSE);
	//gtk_button_set_alignment(GTK_BUTTON (button[0]),1,0);
	//gtk_button_set_alignment(GTK_BUTTON  (button[1]),1,1);
	//gtk_button_set_alignment(GTK_BUTTON (button[2]),0,0);
	//gtk_button_set_alignment(GTK_BUTTON (button[3]),0,1);

//	button = gtk_button_new_with_label ("hi");
//  g_print (gtk_link_button_get_uri(GTK_LINK_BUTTON(button[0])));
	vbox = gtk_vbox_new (FALSE, 0);
   	gtk_container_add (GTK_CONTAINER (window), vbox);
//	gtk_widget_show (vbox);

	ret = create_menu(&window,&vbox);
	if(ret == -1)
	{
		perror("menu create error.\n");
	}

	GtkWidget *_windowFixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(vbox), _windowFixed);
	gtk_widget_show(_windowFixed);

    // Create a button to which to attach menu as a popup 
	button[0] = gtk_link_button_new_with_label ("https://developer.gnome.org/gtk2/stable/","Chat");
	gtk_widget_set_size_request(button[0], 100, 25);
	gtk_fixed_put(GTK_FIXED(_windowFixed), button[0], 10, 15);
	gtk_widget_show(button[0]);

	ret = add_button(&_windowFixed,button[1],10,50,"Send");


	// When the button receives the "clicked" signal, it will call the
    // function hello() passing it NULL as its argument.  The hello()
    // function is defined above. 
    g_signal_connect (button[0], "clicked",
	      G_CALLBACK (hello), CHAT_BUTTON_CLICK_STRING);
    
    // This will cause the window to be destroyed by calling
    // gtk_widget_destroy(window) when "clicked".  Again, the destroy
    // signal could come from here, or the window manager. 
    g_signal_connect_swapped (button[0], "clicked",
			      G_CALLBACK (gtk_widget_destroy),
    					window);


	GtkWidget *scrolledWindow;
    GtkWidget *textView;
    PangoFontDescription *fontDesc;

	scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    textView = gtk_text_view_new();
    fontDesc = pango_font_description_from_string("Courier Pitch Bold 12");
    gtk_widget_modify_font(textView, fontDesc);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), textView);

	gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow, TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(scrolledWindow);
	gtk_widget_show(textView);
	gtk_widget_show (vbox);

    /* This packs the button into the window (a gtk container). */
//    gtk_container_add (GTK_CONTAINER (window), button[0]);
//    gtk_container_add (GTK_CONTAINER (window), button[1]);
//    gtk_container_add (GTK_CONTAINER (window), button[2]);
//    gtk_container_add (GTK_CONTAINER (window), button[3]);
    
    /* The final step is to display this newly created widget. */
//    gtk_widget_show (button[0]);
//    gtk_widget_show (button[1]);
//    gtk_widget_show (button[2]);
//    gtk_widget_show (button[3]);
//	gtk_widget_show (button1);
    
    /* and the window */
    gtk_widget_show (window);
//	gtk_widget_show (InfoBarButton);

    
    /* All GTK applications must have a gtk_main(). Control ends here
     * and waits for an event to occur (like a key press or
     * mouse event). */
    gtk_main ();
    
    return 0;
}
