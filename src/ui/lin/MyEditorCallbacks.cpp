#include "MyEditorCallbacks.h"
#include "MyEditorHandlerLin.h"
#include "MyEditorCommon.h"
#include "MyEditor.h"
#include "ScintillaSend.h"
#include "GTKConversion.h"

extern const char *bookcoloured[];

#include <Scintilla.h>

#define STR_VALUE(arg)      #arg
#define TO_STR(name) STR_VALUE(name)

#define V VER REL
#define VERSION TO_STR(V)

#define GET_ME \
  MyTabWindow* mtw = (MyTabWindow*)p; \
  MyEditor* me = mtw->getCurrentEditor();

using namespace Scintilla;

/* not a callback but a utility function */
GtkWidget *
gtk_container_get_child( GtkContainer *cont, int idx )
{
  GList *list = gtk_container_get_children(cont);
  if( !list ) return NULL;
  int i = 0;
  while( list->data && i < idx )
  {
    list = list->next;
    i++;
  }
  return GTK_WIDGET(list->data);
} 

void tabAdded(GtkNotebook* nb, GtkWidget* tab, guint pIdx, gpointer p)
{
  if( !nb || !tab || !p )
    return; // kick out if missing data

  MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();

  MyTabWindow* mtw = (MyTabWindow*)p;
  MyEditor* me = mtw->getEditor(tab);

  if( !me )
  {
    me = meh->findEditor(tab);
    mtw->addToList(me);
  }

  me->focus();
  meh->setTopEditor(me);
}
void tabRemoved(GtkNotebook* nb, GtkWidget* tab, guint pIdx, gpointer p)
{
  if( !nb || !tab || !p )
    return; // kick out if missing data

  MyTabWindow* mtw = (MyTabWindow*)p;
  MyEditor* me = mtw->getEditor(tab);

  if( !me )return;
  MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();

  mtw->removeEditor(me);
  if( mtw->isEmpty() )
  {
    me->setWin(NULL);
    meh->removeTabWindow(mtw);
    delete mtw;
  }
}

GtkNotebook*
tabRipped(GtkNotebook *nb, GtkWidget *tab, gint x, gint y, gpointer p)
{
  MyTabWindow* mtw = new MyTabWindow();

  GtkWindow* win = GTK_WINDOW(mtw->getWindow());
  gtk_window_move( win, x, y );

  MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();

  meh->addTabWindow(mtw);
  return GTK_NOTEBOOK(mtw->getNotebook());
}

bool tabSelected(GtkNotebook *nb, gpointer tab, guint idx, gpointer p)
{
  MyTabWindow* mtw = (MyTabWindow*)p;

  if( !mtw ) return false;

  mtw->controlWindowTitle(idx);
  return true;
}

void newEditor(void)
{
  MyEditorHandler* meh = MyEditorHandlerLin::getInstance();

  meh->createEditor();
}

void newTab(GObject* w, gpointer p)
{
  if( !p ) return;
  MyTabWindow* mtw = (MyTabWindow*)p;
  MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();

  meh->createEditor(mtw,NULL);
}

void openEditor(void)
{
  GtkWidget* openDia;
  GSList* files;
  MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();
  MyTabWindow* mtw = NULL;
  MyEditor* ed = NULL;
  int resp;
  
  if( meh->getTopEditor() )
  {
    mtw = meh->getTopEditor()->getTabWindow();
  }

  openDia = gtk_file_chooser_dialog_new("Open File ...",
                                        GTK_WINDOW(mtw->getWindow()),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_CANCEL,
                                        GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OPEN,
                                        GTK_RESPONSE_ACCEPT,
                                        NULL );

  resp = gtk_dialog_run( GTK_DIALOG(openDia) );
#ifndef GTK3
  gtk_widget_hide_all(openDia);
#else
  gtk_widget_hide(openDia);
#endif
  if( GTK_RESPONSE_ACCEPT == resp )
  {
    files = gtk_file_chooser_get_filenames( GTK_FILE_CHOOSER(openDia) );

    while( files != NULL )
    {
      ed = meh->findEditor((char*)files->data);

      if( ed )
      {
        ed->focus();
      }
      else
      {
        ed = meh->createEditor(mtw,(char*)files->data);
      }
      files = files->next;
    }
  }
  gtk_widget_destroy(openDia);
}

/* the menu functions have a slightly different signature */
bool closeEditorA(GObject* w, gpointer p)
{
  return closeEditor(NULL,NULL,p);
}

bool closeEditorB(GObject* w, gpointer p)
{
  if( !p ) return FALSE;
  MyEditor* me = (MyEditor*)p;
  MyTabWindow* mtw = me->getTabWindow();

  closeEditor(NULL,NULL,mtw,me);

  return true;
}

bool closeEditor(GObject*, GdkEvent* e, gpointer p, MyEditor* ed)
{
  if( !p ) return FALSE;
  GET_ME
  if( ed ) me = ed; // a specific editor has been supplied to delete

  bool esc = FALSE;
  int resp = 0;
  if( !me ) return TRUE;

  MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();

  if( me->isDirty() )
  {
    GtkWidget* dia = gtk_dialog_new_with_buttons(
        "Info",NULL,GTK_DIALOG_MODAL,
        GTK_STOCK_DISCARD, GTK_RESPONSE_CLOSE,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
        GTK_STOCK_OK, GTK_RESPONSE_OK,
        NULL);

#ifndef GTK3
    gtk_dialog_set_has_separator (GTK_DIALOG (dia), FALSE);
    GtkWidget* txt = gtk_label_new("Warning: You are about\nto close this editor\nwithout saving it's contents");
    GtkWidget* img = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,GTK_ICON_SIZE_DIALOG);
    GtkWidget* hbox = gtk_hbox_new(FALSE,5);

    gtk_box_pack_start_defaults(GTK_BOX(hbox), img);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), txt);

    gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dia)->vbox), hbox);
#else
    GtkWidget* txt = gtk_label_new("Warning: You are about\nto close this editor\nwithout saving it's contents");
    GtkWidget* img = gtk_button_new_with_mnemonic(GTK_STOCK_DIALOG_INFO);
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);

    gtk_box_pack_start(GTK_BOX(hbox), img,false,false,0);
    gtk_box_pack_start(GTK_BOX(hbox), txt,false,false,0);

    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(dia))), hbox,false,false,0);
#endif

    gtk_widget_show_all(dia);

    resp = gtk_dialog_run( GTK_DIALOG(dia) );

    switch( resp )
    {
    case GTK_RESPONSE_CANCEL:
      esc = TRUE;
      break;
    case GTK_RESPONSE_OK:
      esc = !saveEditor(NULL,me);
      break;
    }

    gtk_widget_destroy(dia);
  }

  if( esc ) return FALSE; // kick out

  if( me )
  {
    mtw->deleteEditor(me);
    meh->removeEditor(me);
  }

  if( meh->isEmpty() )
  {
    gtk_main_quit();
  }

  return TRUE;
}

bool saveEditor( GObject*, gpointer p )
{
  bool rc = FALSE;
  if( !p ) return rc;
  GET_ME

  if( !me ) return rc;

  if( !me->saveFile() ) rc = saveAsEditor(NULL,me->getTabWindow());
  else rc = TRUE;

  return rc;
}

bool saveAsEditor( GObject*, gpointer p )
{
  bool rc = FALSE;
  GtkWidget* saveDia = NULL;
  gchar* file = NULL;
  if( !p ) return rc;
  GET_ME

  if( !me ) return rc;

  saveDia = gtk_file_chooser_dialog_new("Save File ...",
                                        NULL,
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        GTK_STOCK_CANCEL,
                                        GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_SAVE,
                                        GTK_RESPONSE_ACCEPT,
                                        NULL );

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(saveDia),TRUE);

  if( GTK_RESPONSE_ACCEPT == gtk_dialog_run( GTK_DIALOG(saveDia) ) )
  {
    file = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(saveDia) );
    me->saveFile(file);
    g_free(file);
    rc = TRUE;
  }
  gtk_widget_destroy(saveDia);
  return rc;
}

void toggleCmd( GObject*, gpointer p )
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->toggleCmdEd();

}

void wrap(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->toggleWrap();
}

void fold(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->foldAll();
}

void unfold(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->unfoldAll();
}

void toggleEol(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->toggleShowLineEndings();
}

void winStyleEol(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->setLineEndings(SC_EOL_CRLF);
}

void macStyleEol(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->setLineEndings(SC_EOL_CR);
}

void linStyleEol(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->setLineEndings(SC_EOL_LF);
}

void zoomin(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->zoom(true);  
}

void zoomout(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->zoom(false);  
}

int quit_app(void)
{
  MyEditorHandler* meh = MyEditorHandlerLin::getInstance();
  int i = 0;

  while( i < meh->editorCount() )
  {
    MyEditor* close = meh->getEditor(i);
    bool didClose = close ? closeEditor(NULL,NULL,close->getTabWindow()) : false;
    if( close && !didClose) i++;
    else if( !close ) i++; // this is really bad!!!!
  }

  return TRUE;
}

bool closeWindow(GObject*, GdkEvent* e, gpointer p)
{
  if( !p ) return FALSE;
  MyTabWindow* mtw = (MyTabWindow*)p;
  
  int amt = mtw->editorCount();
  int i = 0;
  
  while( i < amt )
  {
    MyEditor* close = mtw->getEditor(i);
    if( close && closeEditor(NULL,NULL,mtw,close) )
      amt--;
    else
      i++;
  }
  return true;
}

void undoEditor(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->undo();
}

void redoEditor(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->redo();
}

void cutEditor(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->cut();
}

void copyEditor(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->copy();
}

void pasteEditor(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->paste();
}

void tabsEditor(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->toggleTabs();
}

GtkWidget * findWindow = NULL;
GtkWidget * gotoWindow = NULL;

void showGoto(GObject* w, gpointer p)
{
  GtkWidget * lineEntry;
  int resp = 0;
  GtkAdjustment *spinner_adj;

  spinner_adj = (GtkAdjustment *) gtk_adjustment_new (0, 0, 999999, 1, 0,0);


  gotoWindow = gtk_dialog_new();

  gtk_window_set_position(GTK_WINDOW(gotoWindow), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(gotoWindow), "Goto...");

  gtk_window_set_default_size(GTK_WINDOW(gotoWindow),100,42);
  gtk_widget_set_size_request(gotoWindow,100,42);
  gtk_window_set_resizable(GTK_WINDOW(gotoWindow), FALSE);

  lineEntry = gtk_spin_button_new(spinner_adj,1,0);

  gtk_dialog_add_action_widget(GTK_DIALOG(gotoWindow),lineEntry,GTK_RESPONSE_OK);

  gtk_widget_show(lineEntry);
  resp = gtk_dialog_run(GTK_DIALOG(gotoWindow));
  if( resp == GTK_RESPONSE_OK )
  {
    int line = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(lineEntry));
    MyEditorHandler* meh = MyEditorHandlerLin::getInstance();
    MyEditor* me = meh->getTopEditor();
    if( me && line > 0 ) me->gotoLine(line-1);
  }
  //gtk_widget_destroy(lineEntry);
  if( gotoWindow ) gtk_widget_destroy(gotoWindow);
  gotoWindow = NULL;
}

void showFind(GObject* w, gpointer p)
{

  GtkWidget * lineEntry;
  int resp = 0;

  findWindow = gtk_dialog_new();

  gtk_window_set_position(GTK_WINDOW(findWindow), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(findWindow), "Find...");

  gtk_window_set_default_size(GTK_WINDOW(findWindow),185,42);
  gtk_widget_set_size_request(findWindow,185,42);
  gtk_window_set_resizable(GTK_WINDOW(findWindow), FALSE);

  lineEntry = gtk_entry_new();

  gtk_dialog_add_action_widget(GTK_DIALOG(findWindow),lineEntry,GTK_RESPONSE_OK);

  gtk_widget_show(lineEntry);
  resp = gtk_dialog_run(GTK_DIALOG(findWindow));
  if( resp == GTK_RESPONSE_OK )
  {
    const char* text = gtk_entry_get_text(GTK_ENTRY(lineEntry));
    MyEditorHandler* meh = MyEditorHandlerLin::getInstance();
    MyEditor* me = meh->getTopEditor();
    if( me )
    {
      me->find(DOWN,(char*)text);
    }
  }
  //gtk_widget_destroy(lineEntry);
  if( findWindow) gtk_widget_destroy(findWindow);
  findWindow = NULL;
}

void findPrev(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->find(UP);
}

void findNext(GObject* w, gpointer p)
{
  if( !p ) return;
  GET_ME

  if( !me ) return;

  me->find(DOWN);
}

void showAbout(GObject* w, gpointer p)
{
  GtkWidget* aboutDlg;
  GdkPixbuf* pixbuf;

  aboutDlg = gtk_about_dialog_new();

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(aboutDlg), "CodeAssistor");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(aboutDlg), VERSION);
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(aboutDlg),
      "(c) Michael Mullin");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(aboutDlg),
     "BSD Licenced - see LICENCE.txt");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(aboutDlg),
      "https://launchpad.net/codeassistor");

  pixbuf = gdk_pixbuf_new_from_xpm_data((const char**)bookcoloured);

  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(aboutDlg), pixbuf);
  g_object_unref(pixbuf), pixbuf = NULL;


  gtk_dialog_run(GTK_DIALOG (aboutDlg));
  gtk_widget_destroy(aboutDlg);
}

void GTKNotify(GtkWidget *, gint wParam, gpointer lParam, void* nothing)
{
  MyEditor* me = NULL;
  SCNotification* scn = reinterpret_cast<SCNotification*>(lParam);
  MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();

  me = meh->findEditor((int)scn->nmhdr.idFrom);

  if( !me ) return;

  HandleNotification(me,scn);
}

static int
GetUpdateDlg(GtkWidget* win)
{
  int resp = 0;
  GtkWidget *dialog,
            *label,
            *image,
            *hbox;

  GdkPixbuf* pixbuf;
/* Create a new dialog with one OK button. */
  dialog = gtk_dialog_new_with_buttons( "External File Change",
                                         GTK_WINDOW(win),
                                         GTK_DIALOG_MODAL,
                                         "Update",
                                         GTK_RESPONSE_OK,
                                         "Keep Mine",
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

#ifndef GTK3
  gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);

  label = gtk_label_new ("Would you like to update,\nor keep your copy?\n\nWARNING: Chosing to update\nwill discard any changes\nyou have made");
  pixbuf = gdk_pixbuf_new_from_xpm_data((const char**)bookcoloured);
  image = gtk_image_new_from_pixbuf (pixbuf);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 0);

  gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
  gtk_box_pack_start_defaults (GTK_BOX (hbox), label);

/* Pack the dialog content into the dialog's GtkVBox. */
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox);
#else
  label = gtk_label_new ("Would you like to update,\nor keep your copy?\n\nWARNING: Chosing to update\nwill discard any changes\nyou have made");
  pixbuf = gdk_pixbuf_new_from_xpm_data((const char**)bookcoloured);
  image = gtk_image_new_from_pixbuf (pixbuf);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 0);

  gtk_box_pack_start (GTK_BOX (hbox), image,false,false,0);
  gtk_box_pack_start (GTK_BOX (hbox), label,false,false,0);

/* Pack the dialog content into the dialog's GtkVBox. */
  gtk_box_pack_start (GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(dialog))), hbox,false,false,0);
#endif
  gtk_widget_show_all (dialog);

/* Create the dialog as modal and destroy it when a button is clicked. */
  resp = gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);

  return resp;
}

void GTKCommand(GtkWidget *w, gint wParam, gpointer lParam, void* p)
{
  MyEditor* me = (MyEditor*)p;

  if( !me ) return;
  MyEditorHandler* meh = MyEditorHandlerLin::getInstance();

  switch ( wParam >> 16 )
  {
    case SCEN_SETFOCUS:
      if( me )
      {
        meh->setTopEditor(me);
        me->hideCmdEd();
        if( findWindow ) gtk_widget_destroy(GTK_WIDGET(findWindow));
        if( gotoWindow ) gtk_widget_destroy(GTK_WIDGET(gotoWindow));
        findWindow = gotoWindow = NULL;

        if( me->checkUpdate() )
        {
          switch( GetUpdateDlg(me->getWindow()) )
          {
            case GTK_RESPONSE_OK:
            {
              ScintillaSend* send = ScintillaSend::getInstance();

              int firstline = send->get( (void*)me->getEditor(), SCI_GETFIRSTVISIBLELINE);
              int pos = send->get( (void*)me->getEditor(), SCI_GETCURRENTPOS);
              int line = send->get( (void*)me->getEditor(), SCI_LINEFROMPOSITION,pos);

              me->openFile((char*)me->getFile());
              me->setDirtyOverride(false);
              send->set( (void*)me->getEditor(), SCI_SETSAVEPOINT, 0, 0 );
              send->set( (void*)me->getEditor(), SCI_GOTOLINE,line,0);
              send->set( (void*)me->getEditor(), SCI_SETFIRSTVISIBLELINE,firstline,0);
            }
            break;

            case GTK_RESPONSE_CANCEL:
            default:
            {
              me->setTouch(GetTime(me->getFile()),GetFileSize(me->getFile()));
              me->setDirtyOverride(true);
            }
          }
          me->getTabWindow()->controlWindowTitle();
        }
      }
      break;
  }
}

void cmd_entry( GtkEntry    *entry,
                const gchar *text,
                gint         length,
                gint        *position,
                gpointer     p)
{
  if( !p ) return;
  MyEditor* me = (MyEditor*)p;

  char* txt;
  int sz = 0;

  if( !me ) return;

  const char* buf = gtk_entry_get_text(GTK_ENTRY(me->getCmdEd()));

  sz = strlen(buf) + strlen(text);

  txt = new char[sz+1];
  sprintf(txt,"%s%s",buf,text);
  txt[sz] = '\0';

  me->_cmd.CmdEntry(me,txt);
  delete [] txt;
}

void cmd_entrydel(GObject* w, int is, int was, void* p)
{
  if( !p ) return;
  MyEditor* me = (MyEditor*)p;
  int sz = 0;
  int j = 0;
  char* txt;

  if( !me ) return;

  const char* buf = gtk_entry_get_text(GTK_ENTRY(me->getCmdEd()));

  sz = strlen(buf) - (was-is);

  txt = new char[sz+1];

  for( int i = 0; (unsigned int)i < strlen(buf); i++ )
  {
    if( i < is || i >= was )
      txt[j++] = buf[i];
  }
  txt[sz] = '\0';

  me->_cmd.CmdEntry(me,txt);
  delete [] txt;
}

void cmd_execute(GObject*, gpointer p)
{
  if( !p ) return;
  MyEditor* me = (MyEditor*)p;

  if( !me ) return;

  const char* buf = gtk_entry_get_text(GTK_ENTRY(me->getCmdEd()));

  me->_cmd.CmdExecute(me,buf);
}

/*
Copyright (c) 2010, Michael Mullin <masmullin@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by Michael Mullin <masmullin@gmail.com>.
4. Neither the name of Michael Mullin nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MICHAEL MULLIN ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICHAEL MULLIN BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
