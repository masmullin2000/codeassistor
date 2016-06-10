#include <unique/unique.h>

#include "GTKConversion.h"
#include "MyTabWindow.h"

#include "MyEditorCommon.h"
#include "MyEditorHandlerLin.h"
#include "MyEditorCallbacks.h"
#include "keywords.h"
#include "ScintillaSend.h"

extern const char *bookcoloured[];

#include <Scintilla.h>
#include <SciLexer.h>
#define PLAT_GTK 1
#include <ScintillaWidget.h>

#include "CScintillaController.h"
#include "TXTScintillaController.h"
#include "MKScintillaController.h"
#include "SQLScintillaController.h"
#include "PYScintillaController.h"
#include "PerlScintillaController.h"
#include "PHPScintillaController.h"
#include "SHScintillaController.h"
#include "BATScintillaController.h"

extern UniqueApp* g_app;

MyTabWindow::MyTabWindow()
{
  _window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  unique_app_watch_window (g_app, GTK_WINDOW(_window));
#ifndef GTK3
  _vbox = gtk_vbox_new(FALSE,0);
#else
  _vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
#endif
  _notebook = gtk_notebook_new();

  gtk_notebook_set_scrollable(GTK_NOTEBOOK(_notebook),true);
  gtk_notebook_set_group_name(GTK_NOTEBOOK(_notebook),"CodeAssistor");
  g_signal_connect( _notebook, "page-removed", G_CALLBACK(tabRemoved), this);
  g_signal_connect( _notebook, "page-added", G_CALLBACK(tabAdded),this);
  g_signal_connect( _notebook, "create-window", G_CALLBACK(tabRipped),NULL);
  g_signal_connect( _notebook, "switch-page", G_CALLBACK(tabSelected),this);

  gtk_window_set_icon(GTK_WINDOW(_window), gdk_pixbuf_new_from_xpm_data((const char**)bookcoloured));

  gtk_container_add(GTK_CONTAINER(_window), _vbox);
  gtk_window_set_title(GTK_WINDOW(_window),"The CodeAssistor Editor");

  createMenu();

  g_signal_connect(G_OBJECT(_window),"delete_event",G_CALLBACK(closeWindow),this);
  //g_signal_connect(G_OBJECT(_window),"delete_event",G_CALLBACK(closeEditor),this);

  gtk_widget_set_size_request(_window, 200, 200);
  gtk_window_set_default_size(GTK_WINDOW(_window), 723,700);

  gtk_container_add(GTK_CONTAINER(_vbox), _notebook);

  //gtk_widget_show_all(_window);
}

MyTabWindow::~MyTabWindow()
{
  if( _window ) gtk_widget_destroy(_window);
//  if( _vbox ) gtk_widget_destroy(_vbox);
//  if( _notebook ) gtk_widget_destroy(_notebook);
}

void
MyTabWindow::createMenu()
{
  GtkWidget *menu, *file, *edit, *search, *view, *help,
    *filemenu, *editmenu, *searchmenu, *viewmenu, *helpmenu,
    *file_new, *file_new_tab, *file_open, *file_close, *file_save, *file_saveas, *file_cmd, *file_quit,
    *edit_undo, *edit_redo, *edit_cut, *edit_copy, *edit_paste, *edit_tabs,
    *search_find, *search_prev, *search_next, *search_goto,
    *view_wrap, *view_fold, *view_unfold, *view_eol,
    *view_eolSubMenu, *view_showEol, *view_winStyle, *view_macStyle, *view_linStyle,
    *help_about;

  GtkAccelGroup *accel;

  menu = gtk_menu_bar_new();
  accel = gtk_accel_group_new();

  gtk_window_add_accel_group(GTK_WINDOW(_window), accel);

  file = gtk_menu_item_new_with_label("File");
  edit = gtk_menu_item_new_with_label("Edit");
  search = gtk_menu_item_new_with_label("Search");
  view = gtk_menu_item_new_with_label("View");
  help = gtk_menu_item_new_with_label("Help");

  filemenu = gtk_menu_new();
  editmenu = gtk_menu_new();
  searchmenu = gtk_menu_new();
  viewmenu = gtk_menu_new();
  helpmenu = gtk_menu_new();

  gtk_menu_item_set_submenu( GTK_MENU_ITEM(file), filemenu );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(edit), editmenu );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(search), searchmenu );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(view), viewmenu );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(help), helpmenu );

#ifndef GTK3
  file_new      = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, accel);
  file_new_tab  = gtk_image_menu_item_new_from_stock("New Tab", accel);
  file_open     = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, accel);
  file_close    = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel);
  file_save     = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, accel);
  file_saveas   = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, accel);
  file_cmd      = gtk_image_menu_item_new_from_stock(GTK_STOCK_EXECUTE, accel);
  file_quit     = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel);
#else
  file_new      = gtk_menu_item_new_with_mnemonic(GTK_STOCK_NEW);
  file_new_tab  = gtk_menu_item_new_with_mnemonic("New Tab");
  file_open     = gtk_menu_item_new_with_mnemonic(GTK_STOCK_OPEN);
  file_close    = gtk_menu_item_new_with_mnemonic(GTK_STOCK_CLOSE);
  file_save     = gtk_menu_item_new_with_mnemonic(GTK_STOCK_SAVE);
  file_saveas   = gtk_menu_item_new_with_mnemonic(GTK_STOCK_SAVE_AS);
  file_cmd      = gtk_menu_item_new_with_mnemonic(GTK_STOCK_EXECUTE);
  file_quit     = gtk_menu_item_new_with_mnemonic(GTK_STOCK_QUIT);
#endif
  g_signal_connect(G_OBJECT(file_new),"activate",G_CALLBACK(newEditor),NULL);
  g_signal_connect(G_OBJECT(file_new_tab),"activate",G_CALLBACK(newTab),this);
  g_signal_connect(G_OBJECT(file_open),"activate",G_CALLBACK(openEditor),NULL);
  g_signal_connect(G_OBJECT(file_close),"activate",G_CALLBACK(closeEditorA),this);
  g_signal_connect(G_OBJECT(file_save),"activate",G_CALLBACK(saveEditor),this);
  g_signal_connect(G_OBJECT(file_saveas),"activate",G_CALLBACK(saveAsEditor),this);
  g_signal_connect(G_OBJECT(file_cmd),"activate",G_CALLBACK(toggleCmd),this);
  g_signal_connect(G_OBJECT(file_quit),"activate",G_CALLBACK(quit_app),NULL);

  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_new);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_new_tab);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_open);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_save);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_saveas);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_close);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_cmd);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), file_quit);

  gtk_widget_add_accelerator(file_cmd, "activate", accel,
      GDK_KEY_Escape, GdkModifierType(0), GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(file_new_tab, "activate", accel,
      GDK_KEY_t, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(file_saveas, "activate", accel,
      GDK_KEY_s, GdkModifierType(GDK_CONTROL_MASK|GDK_SHIFT_MASK), GTK_ACCEL_VISIBLE);

#ifndef GTK3
  edit_undo = gtk_image_menu_item_new_from_stock(GTK_STOCK_UNDO, accel);
  edit_redo = gtk_image_menu_item_new_from_stock(GTK_STOCK_REDO, accel);
  edit_cut = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, accel);
  edit_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, accel);
  edit_paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, accel);
  edit_tabs = gtk_image_menu_item_new_from_stock("Toggle Tabs/Spaces",accel);

#else
  edit_undo = gtk_menu_item_new_with_mnemonic(GTK_STOCK_UNDO);
  edit_redo = gtk_menu_item_new_with_mnemonic(GTK_STOCK_REDO);
  edit_cut = gtk_menu_item_new_with_mnemonic(GTK_STOCK_CUT);
  edit_copy = gtk_menu_item_new_with_mnemonic(GTK_STOCK_COPY);
  edit_paste = gtk_menu_item_new_with_mnemonic(GTK_STOCK_PASTE);
  edit_tabs = gtk_menu_item_new_with_mnemonic(GTK_STOCK_MEDIA_NEXT);
#endif
  g_signal_connect(G_OBJECT(edit_undo),"activate",G_CALLBACK(undoEditor),this);
  g_signal_connect(G_OBJECT(edit_redo),"activate",G_CALLBACK(redoEditor),this);
  g_signal_connect(G_OBJECT(edit_cut),"activate",G_CALLBACK(cutEditor),this);
  g_signal_connect(G_OBJECT(edit_copy),"activate",G_CALLBACK(copyEditor),this);
  g_signal_connect(G_OBJECT(edit_paste),"activate",G_CALLBACK(pasteEditor),this);
  g_signal_connect(G_OBJECT(edit_tabs),"activate",G_CALLBACK(tabsEditor),this);

  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), edit_undo);
  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), edit_redo);
  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), edit_cut);
  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), edit_copy);
  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), edit_paste);
  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), edit_tabs);

  gtk_widget_add_accelerator(edit_undo, "activate", accel,
      GDK_KEY_z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(edit_redo, "activate", accel,
      GDK_KEY_y, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

#ifndef GTK3
  search_find = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND, accel);
  search_prev = gtk_image_menu_item_new_from_stock(GTK_STOCK_GO_BACK, accel);
  search_next = gtk_image_menu_item_new_from_stock(GTK_STOCK_GO_FORWARD, accel);
  search_goto = gtk_image_menu_item_new_from_stock("Goto", accel);
#else
  search_find = gtk_menu_item_new_with_mnemonic(GTK_STOCK_FIND);
  search_prev = gtk_menu_item_new_with_mnemonic(GTK_STOCK_GO_BACK);
  search_next = gtk_menu_item_new_with_mnemonic(GTK_STOCK_GO_FORWARD);
  search_goto = gtk_menu_item_new_with_mnemonic("Goto");
#endif
  g_signal_connect(G_OBJECT(search_find),"activate",G_CALLBACK(showFind),NULL);
  g_signal_connect(G_OBJECT(search_prev),"activate",G_CALLBACK(findPrev),this);
  g_signal_connect(G_OBJECT(search_next),"activate",G_CALLBACK(findNext),this);
  g_signal_connect(G_OBJECT(search_goto),"activate",G_CALLBACK(showGoto),NULL);

  gtk_widget_add_accelerator(search_prev, "activate", accel,
      GDK_KEY_F3, GdkModifierType(0), GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(search_next, "activate", accel,
      GDK_KEY_F4, GdkModifierType(0), GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(search_goto, "activate", accel,
      GDK_KEY_g, GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), search_find);
  gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), search_prev);
  gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), search_next);
  gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), search_goto);


  GtkWidget *view_zoomin, *view_zoomout;
  
#ifndef GTK3
  view_zoomin = gtk_image_menu_item_new_from_stock("Zoom In", accel);
  view_zoomout = gtk_image_menu_item_new_from_stock("Zoom Out", accel);
  view_wrap = gtk_image_menu_item_new_from_stock("Wrap", accel);
  view_fold = gtk_image_menu_item_new_from_stock("Fold All", accel);
  view_unfold = gtk_image_menu_item_new_from_stock("Unfold All", accel);
#else
  view_zoomin = gtk_menu_item_new_with_mnemonic("Zoom In");
  view_zoomout = gtk_menu_item_new_with_mnemonic("Zoom Out");
  view_wrap = gtk_menu_item_new_with_mnemonic("Wrap");
  view_fold = gtk_menu_item_new_with_mnemonic("Fold All");
  view_unfold = gtk_menu_item_new_with_mnemonic("Unfold All");
#endif
  view_eol = gtk_menu_item_new_with_label("Line Endings");

  g_signal_connect(G_OBJECT(view_zoomin),"activate",G_CALLBACK(zoomin),this);
  g_signal_connect(G_OBJECT(view_zoomout),"activate",G_CALLBACK(zoomout),this);
  

  g_signal_connect(G_OBJECT(view_wrap),"activate",G_CALLBACK(wrap),this);
  g_signal_connect(G_OBJECT(view_fold),"activate",G_CALLBACK(fold),this);
  g_signal_connect(G_OBJECT(view_unfold),"activate",G_CALLBACK(unfold),this);
  
  gtk_widget_add_accelerator(view_zoomin, "activate", accel,
      GDK_KEY_plus, GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(view_zoomin, "activate", accel,
      GDK_KEY_equal, GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(view_zoomout, "activate", accel,
      GDK_KEY_minus, GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(view_wrap, "activate", accel,
      GDK_KEY_w, GdkModifierType(GDK_MOD1_MASK), GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(view_fold, "activate", accel,
      GDK_KEY_minus, GdkModifierType(GDK_MOD1_MASK), GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(view_unfold, "activate", accel,
      GDK_KEY_plus, GdkModifierType(GDK_MOD1_MASK), GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator(view_unfold, "activate", accel,
      GDK_KEY_equal, GdkModifierType(GDK_MOD1_MASK), GTK_ACCEL_VISIBLE);

  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), view_zoomin);
  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), view_zoomout);
  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), view_wrap);
  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), view_fold);
  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), view_unfold);
  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), view_eol);

  view_eolSubMenu = gtk_menu_new();
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(view_eol), view_eolSubMenu );

#ifndef GTK3
  view_showEol = gtk_image_menu_item_new_from_stock("Show/Hide Line Endings",accel);
  view_winStyle = gtk_image_menu_item_new_from_stock("Windows Style",NULL);
  view_macStyle = gtk_image_menu_item_new_from_stock("Old Macintosh Style",NULL);
  view_linStyle = gtk_image_menu_item_new_from_stock("Linux Style",NULL);
#else
  view_showEol = gtk_menu_item_new_with_mnemonic("Show/Hide Line Endings");
  view_winStyle = gtk_menu_item_new_with_mnemonic("Windows Style");
  view_macStyle = gtk_menu_item_new_with_mnemonic("Old Macintosh Style");
  view_linStyle = gtk_menu_item_new_with_mnemonic("Linux Style");
#endif
  gtk_menu_shell_append(GTK_MENU_SHELL(view_eolSubMenu), view_showEol);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_eolSubMenu), view_winStyle);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_eolSubMenu), view_macStyle);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_eolSubMenu), view_linStyle);

  g_signal_connect(G_OBJECT(view_showEol),"activate",G_CALLBACK(toggleEol),this);
  g_signal_connect(G_OBJECT(view_winStyle),"activate",G_CALLBACK(winStyleEol),this);
  g_signal_connect(G_OBJECT(view_macStyle),"activate",G_CALLBACK(macStyleEol),this);
  g_signal_connect(G_OBJECT(view_linStyle),"activate",G_CALLBACK(linStyleEol),this);

  gtk_widget_add_accelerator(view_showEol, "activate", accel,
      GDK_KEY_l, GdkModifierType(GDK_CONTROL_MASK|GDK_SHIFT_MASK), GTK_ACCEL_VISIBLE);

#ifndef GTK3
  help_about = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, accel);
#else
  help_about = gtk_menu_item_new_with_mnemonic(GTK_STOCK_ABOUT);
#endif
  g_signal_connect(G_OBJECT(help_about),"activate",G_CALLBACK(showAbout),NULL);

  gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), help_about);

  gtk_menu_shell_append( GTK_MENU_SHELL(menu), file );
  gtk_menu_shell_append( GTK_MENU_SHELL(menu), edit );
  gtk_menu_shell_append( GTK_MENU_SHELL(menu), search );
  gtk_menu_shell_append( GTK_MENU_SHELL(menu), view );
  gtk_menu_shell_append( GTK_MENU_SHELL(menu), help );

  gtk_box_pack_start(GTK_BOX(_vbox),menu,FALSE,FALSE,0);
}

MyEditor*
MyTabWindow::getCurrentEditor()
{
  if( !_notebook || gtk_notebook_get_n_pages(GTK_NOTEBOOK(_notebook)) == 0 )
    return NULL;

  gint idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(_notebook));

  MyEditor* me = getEditor(idx);

  return me;
}

static GtkWidget *
create_notebook_label
(
  const gchar       *text,
  MyEditor          *me
)
{
  GtkWidget *hbox, *label, *button, *image;

#ifndef GTK3
  hbox = gtk_hbox_new (FALSE, 0);
#else         
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
#endif
  gtk_container_set_border_width(GTK_CONTAINER(hbox),0);

  label = gtk_label_new (text);
  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);

  button = gtk_button_new();
  gtk_button_set_relief(GTK_BUTTON(button),GTK_RELIEF_NONE);
  g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(closeEditorB),me);
  gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

#ifndef GTK3
  GtkRcStyle *style;
  style = gtk_rc_style_new();
  style->xthickness = style->ythickness = 0;
  gtk_widget_modify_style(button,style);
  g_object_unref(G_OBJECT(style));
  image = gtk_image_new_from_stock( GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU );
#else
  image = gtk_image_new_from_icon_name( GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU );
#endif
  gtk_container_add( GTK_CONTAINER(button),image);
 
  gtk_widget_show_all( hbox );

  return( hbox );
}

void
MyTabWindow::addToList( MyEditor* me )
{
  me->setWin(this);
  _editorList.push_back(me);
  gtk_widget_show_all(_window);
  hideAllCmdEd();
  gtk_widget_hide(me->getCmdEd());
}

void
MyTabWindow::addEditor( MyEditor* me )
{
  if( !me ) return; // kick out if null

  char* file = "Untitled";
  if( me->getFile() )
    file = g_path_get_basename(me->getFile());

  _editorList.push_back(me);

  gint page = gtk_notebook_append_page( GTK_NOTEBOOK(_notebook),
                                        me->getBox(),
                                        //gtk_label_new(file));
                                        create_notebook_label(file,me));

  gtk_notebook_set_tab_reorderable( GTK_NOTEBOOK(_notebook),
                                    me->getBox(),
                                    true );
  gtk_notebook_set_tab_detachable( GTK_NOTEBOOK(_notebook),
                                   me->getBox(),
                                   true );

  gtk_widget_show_all(_window);

  hideAllCmdEd();
  gtk_notebook_set_current_page( GTK_NOTEBOOK(_notebook),page );

  controlWindowTitle();
  gtk_widget_grab_focus(GTK_WIDGET(me->getEditor()));
}

void
MyTabWindow::deleteEditor( MyEditor* me )
{
  gint page = gtk_notebook_page_num( GTK_NOTEBOOK(_notebook),me->getBox() );
  gtk_notebook_remove_page(GTK_NOTEBOOK(_notebook),page);
}

void
MyTabWindow::removeEditor( MyEditor* me )
{
  if( !me ) return;
  vector<MyEditor*>::iterator it = _editorList.begin();

  for( ; it != _editorList.end(); it++ )
    if( *it == me )
    {
      _editorList.erase(it);
      break;
    }
}

void
MyTabWindow::controlWindowTitle( int idx )
{
  MyEditor* me = NULL;

  if( idx < 0 ) me = getCurrentEditor();
  else me = getEditor(idx);

  if( !me ) return; // kick out if no editor

  GtkWidget* tab = gtk_notebook_get_tab_label( GTK_NOTEBOOK(_notebook),
                                               me->getBox() );
  GtkWidget* label = gtk_container_get_child( GTK_CONTAINER(tab),0);                                               

  if( _window && me->getEditor() )
  {
    char fullFile[PATH_MAX];
    memset(fullFile,'\0',PATH_MAX);

    if( me->getFile() )
    {
      if( me->isDirty() )
        sprintf(fullFile,"-*%s*-",g_path_get_basename(me->getFile()));
      else
        sprintf(fullFile,"%s",g_path_get_basename(me->getFile()));
    }
    else
    {
      if( me->isDirty() )
        sprintf(fullFile,"-*%s*-","Untitled");
      else
        sprintf(fullFile,"%s","Untitled");
    }

    gtk_window_set_title(GTK_WINDOW(_window),fullFile);
    gtk_label_set_text(GTK_LABEL(label),fullFile);
  }
}

MyEditor*
MyTabWindow::getEditor( int idx )
{
  GtkWidget* ed = gtk_notebook_get_nth_page(GTK_NOTEBOOK(_notebook),idx);

  vector<MyEditor*>::const_iterator it = _editorList.begin();

  for( ; it != _editorList.end(); it++ )
    if( (*it)->getBox() == ed )
      return *it;

  return NULL;
}

MyEditor*
MyTabWindow::getEditor( GtkWidget* w )
{
  vector<MyEditor*>::const_iterator it = _editorList.begin();

  for( ; it != _editorList.end(); it++ )
    if( (*it)->getBox() == w )
      return *it;

  return NULL;
}

void
MyTabWindow::hideAllCmdEd(void)
{
  vector<MyEditor*>::const_iterator it = _editorList.begin();

  for( ; it != _editorList.end(); it++ )
    gtk_widget_hide((*it)->getCmdEd());
}
