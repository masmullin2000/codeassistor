#ifndef __MY_TAB_WINDOW_LIN_H__
#define __MY_TAB_WINDOW_LIN_H__

#include <vector>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

using namespace std;

class MyEditor;

class MyTabWindow
{
private:
  GtkWidget* _window, * _vbox, * _notebook;

  vector<MyEditor*>  _editorList;

  void createMenu();
public:
  MyTabWindow();
  virtual ~MyTabWindow();

  GtkWidget* getWindow() { return _window; }
  GtkWidget* getNotebook() { return _notebook; }

  void controlWindowTitle( int idx = -1 );

  MyEditor* getEditor( int idx );
  MyEditor* getEditor( GtkWidget* w );
  MyEditor* getCurrentEditor();

  void addEditor( MyEditor* me );
  void addToList( MyEditor* me );

  void deleteEditor( MyEditor* me );
  void removeEditor( MyEditor* me );
  
  int  editorCount() { return _editorList.size(); }

  bool isEmpty() { if( _editorList.size() == 0 ) return true; else return false; }
  void hideAllCmdEd();
};

#endif

/*
Copyright (c) 2011, Michael Mullin <masmullin@gmail.com>
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
