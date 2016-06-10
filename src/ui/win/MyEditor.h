#ifndef __MY_EDITOR__
#define __MY_EDITOR__

#include <shlwapi.h>

#include "keywords.h"
#include "resource.h"

#include "BaseEditor.h"
#include "MyTabWindow.h"

#include "MyEditorDlg.h"

extern WNDPROC gEditProc;
extern WNDPROC gSciProc;

class MyEditor : public BaseEditor
{
private:
  MyTabWindow* _win;
  HWND  _cmdEd;
  HWND  _cmdButton;
  int _showParms;

  HINSTANCE _app; // this should probably be global

  fileType_t _fType;

  int createTab(MyTabWindow* win);
  int createEditor();
protected:
public:
  MyEditor(MyTabWindow* win, HINSTANCE app, int nCmdShow, char* file = NULL);
  virtual bool setFile( char* file );
  ~MyEditor();
  
  virtual void undo();
  virtual void redo();
  virtual void cut();
  virtual void copy();
  virtual void paste();
  
  void close();
  
  const HWND getWindow();
  const MyTabWindow* getTabWindow() { return _win; }
  const HWND getCmdEd() { return _cmdEd; }

  const fileType_t getFType() { return _fType; }
    
  bool isOwner(HWND hwnd);
  
  void setType(char* fType);
  void applyStyle(fileType_t fType);
  
  bool save( bool isNew = false );
  
  void focus( void );

  virtual void controlWindowTitle( void );
  void showCmdEd(void);
  void hideCmdEd(void);
  bool isCmdEdShown(void);
  
  void setParentWin(MyTabWindow* win);
  void showWindow(void);
};

struct MessageToExternal
{
  char fileName[MAX_PATH];
  int  lineNum;
};

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
