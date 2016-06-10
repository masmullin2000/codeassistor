#include "MyEditor.h"

#include "MyEditorCommon.h"
#include "MyEditorHandlerLin.h"
#include "MyEditorCallbacks.h"
#include "keywords.h"
#include "ScintillaSend.h"

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "bookcoloured.xpm"

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

#include <limits.h>
#include <stdlib.h>

using namespace Scintilla;

fileType_t getExt(char* f)
{
  if( !f ) return OBJCPPFILE;

  int len = strlen(f);
  char rExt[7], ext[7];
  int i, j=0;

  for( i = len-1; i > 0 && i > len-7 && f[i+1] != '.'; i-- )
  {
    rExt[j++] = f[i];
  }
  rExt[j] = '\0';

  len = j;
  j = 0;

  for( i = len-1; i >= 0; i-- )
  {
    ext[j++] = rExt[i];
  }

  ext[j] = '\0';

  return GetFileType(ext);
}

MyEditor::MyEditor( MyTabWindow* win, int id, char* file ) : BaseEditor()
{
  _win = win;
#ifdef GTK3
  _vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
#else
  _vbox = gtk_vbox_new(FALSE,0);
#endif
  _editor = scintilla_new();
  _sci = SCINTILLA(_editor);

  _cmdEd = gtk_entry_new();

  g_signal_connect(G_OBJECT(_cmdEd),"activate",G_CALLBACK(cmd_execute),this);
  g_signal_connect(G_OBJECT(_cmdEd),"insert_text",G_CALLBACK(cmd_entry),this);
  g_signal_connect(G_OBJECT(_cmdEd),"delete_text",G_CALLBACK(cmd_entrydel),this);

  gtk_box_pack_start(GTK_BOX(_vbox),_cmdEd,FALSE,FALSE,0);

  gtk_container_add(GTK_CONTAINER(_vbox), _editor);

  scintilla_set_id(_sci, id);
  _id = id;

  ScintillaController* sed = GetSciType( getExt(file) );
  openFile(file);
  sed->setDefaultEditor(_sci,OBJCPPFILE);

  g_signal_connect(G_OBJECT(_sci), SCINTILLA_NOTIFY,
                   G_CALLBACK(GTKNotify), NULL);
  g_signal_connect(G_OBJECT(_sci), "command",
                   G_CALLBACK(GTKCommand), this);

  gtk_widget_hide(_cmdEd);
  gtk_widget_grab_focus(GTK_WIDGET(_editor));
}

MyEditor::~MyEditor()
{
  //gtk_widget_destroy(_editor);
  //gtk_widget_destroy(_cmdEd);
}

GtkWidget*
MyEditor::getWindow()
{
  return _win->getWindow();
}

bool
MyEditor::setFile( char* file )
{
  bool rc = BaseEditor::setFile(file);

  _win->controlWindowTitle();
  
  return rc;
}

void
MyEditor::openFile( char* file )
{
  gchar* content;
  gsize size;
  GError* error = NULL;

  if( !file ) return;

  ScintillaSend* send = ScintillaSend::getInstance();

  setFile(file);

  if( g_file_get_contents( _file, &content, &size, &error ) )
    if( _sci ) send->set(_sci,SCI_SETTEXT,0,content);

  send->set(_sci,SCI_SETSAVEPOINT,0,0);
  send->set(_sci,SCI_EMPTYUNDOBUFFER,0,0);
  send->set(_sci,SCI_COLOURISE,0,-1);

  setTouch(GetTime(getFile()),GetFileSize(getFile()));
  setDirtyOverride(false);
}

bool
MyEditor::saveFile( char* file )
{
  int len = 0;
  char* text = NULL;
  GError* err = NULL;
  bool rc = TRUE;
  ScintillaSend* send = ScintillaSend::getInstance();

  if( file ) setFile(file);

  if( !_file ) return FALSE; //kick out early if we dont know where to save to

  len = send->get(_sci,SCI_GETLENGTH,0,0) + 1;
  text = new char[len];
  send->set(_sci,SCI_GETTEXT,len,text);

  if( !g_file_set_contents( _file, text, -1, &err ) )
  {
    rc = FALSE;
    printf("%s. The code is 0x%x :: enum starts at 0x%x\n",err->message,err->code,G_FILE_ERROR_EXIST);
  }else
  {
    delete [] text;
    send->set(_sci,SCI_SETSAVEPOINT,0,0);
    setTouch(GetTime(getFile()),GetFileSize(getFile()));
    setDirtyOverride(false);
  }

  return rc;
}

bool
MyEditor::isCmdEdShown()
{
  return gtk_widget_get_visible(_cmdEd);
}

void
MyEditor::toggleCmdEd()
{
  if( isCmdEdShown() )
    hideCmdEd();
  else
    showCmdEd();
}

void
MyEditor::showCmdEd()
{
  gtk_widget_show(_cmdEd);
  gtk_widget_grab_focus(GTK_WIDGET(_cmdEd));

}

void
MyEditor::hideCmdEd()
{
  gtk_widget_hide(_cmdEd);
  gtk_widget_grab_focus(GTK_WIDGET(_editor));
}

void
MyEditor::undo()
{
  if( isCmdEdShown() )
    ;//g_signal_emit_by_name(_cmdEd,"undo-selection",NULL);
  else
    BaseEditor::undo();
}

void
MyEditor::redo()
{
  if( isCmdEdShown() )
    ;// cannot redo in a normal text edit SendMessage(_cmdEd,WM_REDO,0,0);
  else
    BaseEditor::redo();
}

void
MyEditor::cut()
{
  if( isCmdEdShown() )
    g_signal_emit_by_name(_cmdEd,"cut-clipboard",NULL);
  else
    BaseEditor::cut();
}

void
MyEditor::copy()
{
  if( isCmdEdShown() )
    g_signal_emit_by_name(_cmdEd,"copy-clipboard",NULL);
  else
    BaseEditor::copy();
}

void
MyEditor::paste()
{
  if( isCmdEdShown() )
    g_signal_emit_by_name(_cmdEd,"paste-clipboard",NULL);
  else
    BaseEditor::paste();
}

void
MyEditor::toggleTabs()
{
  BaseEditor::toggleTabs();
}

void
MyEditor::focus()
{
  gtk_window_present( GTK_WINDOW(_win->getWindow()));

  int page = gtk_notebook_page_num( GTK_NOTEBOOK(_win->getNotebook()), getBox() );
  gtk_notebook_set_current_page( GTK_NOTEBOOK(_win->getNotebook()),page);
  
  gtk_widget_show_all(_win->getWindow());
  _win->hideAllCmdEd();

  gtk_widget_grab_focus( _editor );
  gtk_window_activate_focus( GTK_WINDOW(_win->getWindow()) );
}

/* helper file... consumer must free the return */
char*
fileFullPath(char* file)
{
  char* f = NULL;
  
  if( file && strlen(file) > 0)
  {
    f = realpath(file,NULL);
    if( !f )
    {
      char * x = file+7;
      int len = strlen(x);

      for(int i = 0; i < len; i++ )
        if( x[i] == '\n' ) x[i-1] = '\0';

      f = realpath(x,NULL);
    }
  }
  
  return f;
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
