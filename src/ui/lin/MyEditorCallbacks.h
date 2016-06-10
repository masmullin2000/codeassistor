#include <gtk/gtk.h>
#include <MyEditor.h>

/* not a callback, just a util function */
GtkWidget *
gtk_container_get_child( GtkContainer *cont, int idx );

void tabRemoved(GtkNotebook* nb, GtkWidget* tab, guint pIdx, gpointer p);
void tabAdded(GtkNotebook* nb, GtkWidget* tab, guint pIdx, gpointer p);
GtkNotebook* tabRipped(GtkNotebook *nb, GtkWidget *tab, gint x, gint y, gpointer p);
bool tabSelected(GtkNotebook *nb, gpointer tab, guint idx, gpointer p); 

void  newEditor(void);
void  newTab(GObject* w, gpointer p);
void  openEditor(void);
bool  closeEditorA(GObject* w, gpointer p);
bool  closeEditorB(GObject* w, gpointer p);
bool  closeEditor(GObject*, GdkEvent*, gpointer p, MyEditor* ed = NULL);
bool  closeWindow(GObject*, GdkEvent*, gpointer p);
bool  saveEditor( GObject*, gpointer p );
bool  saveAsEditor( GObject*, gpointer p );
void  toggleCmd( GObject*, gpointer p );
int   quit_app(void);

void  undoEditor(GObject* w, gpointer p);
void  redoEditor(GObject* w, gpointer p);
void  cutEditor(GObject* w, gpointer p);
void  copyEditor(GObject* w, gpointer p);
void  pasteEditor(GObject* w, gpointer p);
void  tabsEditor(GObject* w, gpointer p);

void showFind(GObject* w, gpointer p);
void findPrev(GObject* w, gpointer p);
void findNext(GObject* w, gpointer p);
void showGoto(GObject* w, gpointer p);

void  wrap(GObject* w, gpointer p);
void  fold(GObject* w, gpointer p);
void  unfold(GObject* w, gpointer p);
void  toggleEol(GObject* w, gpointer p);
void  winStyleEol(GObject* w, gpointer p);
void  macStyleEol(GObject* w, gpointer p);
void  linStyleEol(GObject* w, gpointer p);

void zoomin(GObject* w, gpointer p);
void zoomout(GObject* w, gpointer p);

void showAbout(GObject* w, gpointer p);

void  GTKNotify(GtkWidget *, gint /*wParam*/, gpointer lParam, void* nothing);
void  GTKCommand(GtkWidget *, gint /*wParam*/, gpointer lParam, void* p);

void cmd_entry( GtkEntry    *entry,
                const gchar *text,
                gint         length,
                gint        *position,
                gpointer     p);

void cmd_entrydel(GObject* w, int is, int was, void* q);

void cmd_execute(GObject*, gpointer p);

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
