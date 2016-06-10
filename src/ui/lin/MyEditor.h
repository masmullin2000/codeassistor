#ifndef __MY_EDITOR_LIN_H___
#define __MY_EDITOR_LIN_H___

#include "keywords.h"

#include "BaseEditor.h"

#include "MyTabWindow.h"

class MyEditor : public BaseEditor
{
private:
  MyTabWindow*  _win;
  GtkWidget*    _editor, * _cmdEd, * _vbox;
  fileType_t    _fType;
  int           _id;

public:
  MyEditor(MyTabWindow* win, int id, char* file = NULL);
  virtual ~MyEditor();

  virtual void undo();
  virtual void redo();
  virtual void cut();
  virtual void copy();
  virtual void paste();
  virtual void toggleTabs();

  const fileType_t getFType() { return _fType; }

  virtual bool setFile( char* file );
  void openFile( char* file );

  int getID() { return _id; }

  bool saveFile( char* file = NULL);

  bool isCmdEdShown();
  void toggleCmdEd();
  void showCmdEd();
  void hideCmdEd();

  GtkWidget* getCmdEd() { return _cmdEd; }
  GtkWidget* getEditor() { return _editor; }
  GtkWidget* getBox() { return _vbox; }
  GtkWidget* getWindow();
  MyTabWindow* getTabWindow() { return _win; }

  void setWin( MyTabWindow* win ) { _win = win; }

  void focus(void);

};

char*
fileFullPath(char* file);

#endif

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
