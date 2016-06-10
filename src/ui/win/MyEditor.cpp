#include <stdio.h>

#include "MyEditor.h"
#include "MyEditorHandler.h"
#include "MyEditorDlg.h"
#include "MyEditorCommon.h"
#include "MyTabWindow.h"

#include "Scintilla.h"

#include "WinFile.h"
#include "CScintillaController.h"
#include "TXTScintillaController.h"
#include "MKScintillaController.h"
#include "SQLScintillaController.h"
#include "PYScintillaController.h"
#include "PerlScintillaController.h"
#include "PHPScintillaController.h"
#include "SHScintillaController.h"
#include "BATScintillaController.h"
#include "ScintillaSend.h"

using namespace Scintilla;

MyEditor::MyEditor(MyTabWindow* win, HINSTANCE app, int nCmdShow, char* file)
{
  int length = 0;

  _sci = NULL;
  _file = NULL;
  _app = app;
  _fType = FILE_TYPE_UNINIT;
  _ttf = NULL;
  _win = win;
  _showParms = nCmdShow;

  if( _win )
  {
    if( createEditor() )
    {
      ShowWindow(_win->getHWND(), nCmdShow);
      UpdateWindow(_win->getHWND());
      if( file && (GetFileSize(file) <= MAX_FILE_SIZE) && setFile(file) )
      {
        LoadTextFileToEdit(_sci,_file);
        ScintillaSend* send = ScintillaSend::getInstance();
        send->set(_sci, SCI_EMPTYUNDOBUFFER, 0, 0 );
      }else if( GetFileSize(file) > MAX_FILE_SIZE )
      {
        MessageBox(NULL,"File is too large, Maximum file size is 100MB","File Open Error",MB_OK|MB_ICONERROR);
      }else
        applyStyle(HDFILE);

      SetFocus(_sci);
      ShowWindow(_cmdEd,SW_HIDE);
      ::SetForegroundWindow(_win->getHWND());
    }
  }
}

void
MyEditor::setParentWin(MyTabWindow* win)
{
  _win = win;
  _win->createTab(this);

  ::SetParent(_sci,win->getTabCtrl());
  ::SetParent(_cmdEd,win->getTabCtrl());
}

void
MyEditor::showWindow(void)
{
  ::ShowWindow(_win->getHWND(), _showParms);
  ::UpdateWindow(_win->getHWND()); 
}

const HWND
MyEditor::getWindow()
{
  HWND rc = NULL;
  
  if( _win ) rc = _win->getHWND();
  
  return rc;
}

bool
MyEditor::setFile(char* file)
{
  if( file )
  {
    char fullFile[MAX_PATH];
    int retval = GetFullPathName(file,MAX_PATH,fullFile,NULL);

    int length = strlen(fullFile);

    if( length > 0 )
    {
      char* ext = NULL;
      if( _file ) delete [] _file;
      _file = new char[length+1];
      strncpy(_file,fullFile,length);
      _file[length] = '\0';
      ext = PathFindExtension(_file);
      if( ext )
      {
        setType(ext);
        applyStyle(_fType);
      }
      memset(fullFile,'\0',MAX_PATH);
      sprintf(fullFile,"%s",PathFindFileName(_file));

      SetWindowText(_win->getHWND(),fullFile);
      setTouch(GetTime(_file),GetFileSize(_file));

      return true;
    }
  }

  return false;
}

MyEditor::~MyEditor()
{
}

void
MyEditor::close()
{
  DestroyWindow(_sci);
  DestroyWindow(_cmdEd);

  _win->removeEditor(this);
}

int
MyEditor::createEditor()
{
  if( _win )
  {
    _win->createTab(this);
    _sci = CreateWindowEx( WS_EX_CLIENTEDGE,
               "Scintilla", "",
               WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL
                | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
               0,
               20,
               720,
               700,
               _win->getTabCtrl(),
               (HMENU)IDC_MAIN_EDIT,
               GetModuleHandle(NULL),
               NULL);

    gSciProc = (WNDPROC)GetWindowLongPtr(_sci,GWLP_WNDPROC);
    SetWindowLongPtr( _sci,GWLP_WNDPROC,(LONG_PTR)SciProc );

    _cmdEd = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW,
               "EDIT", "",
               WS_CHILD | WS_VISIBLE | WS_HSCROLL | ES_AUTOHSCROLL,
               0,
               0,
               100,
               20,
               _win->getTabCtrl(),
               (HMENU)IDC_CMDED,
               GetModuleHandle(NULL),
               NULL);

    HFONT hFont = CreateFont(16,
               0,
               0,
               0,
               FW_DONTCARE,
               FALSE,
               FALSE,
               FALSE,
               ANSI_CHARSET,
               OUT_DEFAULT_PRECIS,
               CLIP_DEFAULT_PRECIS,
               DEFAULT_QUALITY,
               DEFAULT_PITCH | FF_SWISS, "Courier");

    SendMessage(_cmdEd,WM_SETFONT,WPARAM(hFont),TRUE);

    gEditProc = (WNDPROC)GetWindowLongPtr(_cmdEd,GWLP_WNDPROC);
    SetWindowLongPtr( _cmdEd,GWLP_WNDPROC,(LONG_PTR)CmdProc );
  }

  if(_sci == NULL)
  {
    MessageBox( _win->getHWND(),
                "Could not create edit box.",
                "Error",
                MB_OK | MB_ICONERROR);
    return 0;
  }

  return 1;
}

bool
MyEditor::isOwner(HWND hwnd)
{
  return _win->getHWND() == hwnd;
}

void
MyEditor::setType(char* fType)
{
  _fType = GetFileType(fType);
}

void
MyEditor::applyStyle(fileType_t fType)
{
  ScintillaController* ed = GetSciType(fType);

  if( ed ) ed->setDefaultEditor(_sci,fType);
}

bool
MyEditor::save(bool isNew)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  ERRSAVE saved = ERRSAVE_NONE;

  if( isNew || _file == NULL )
  {
    saved = DoFileSave(this);
  }
  else
  {
    saved = SaveTextFileFromEdit(_sci,_file);
  }

  if( saved == ERRSAVE_NONE )
  {
    send->set( _sci, SCI_SETSAVEPOINT, 0, 0 );
    setTouch(GetTime(getFile()),GetFileSize(getFile()));
    setDirtyOverride(false);
  }
  else
  {
    if( saved == ERRSAVE_PERMDENIED )
    {
      char* fileMsg = new char[strlen(_file) + 40];
      sprintf(fileMsg,"You do not have permission to write to\n\n%s",_file);
      MessageBox( _win->getHWND(), fileMsg, "Permission Error", MB_OK|MB_ICONERROR);
    }
    else if( !isNew )
    {
      MessageBox( _win->getHWND(), "There is no filename.\nFile was not saved","File Error",MB_OK|MB_ICONERROR);
    }
  }
  return saved;
}

void
MyEditor::controlWindowTitle( void )
{
  if( _file && _win->getHWND() && _sci )
  {
    ScintillaSend* send = ScintillaSend::getInstance();
    char fullFile[MAX_PATH];
    memset(fullFile,'\0',MAX_PATH);
    if( isDirty() )
      sprintf(fullFile,"-*%s*-",PathFindFileName(_file));
    else
      sprintf(fullFile,"%s",PathFindFileName(_file));

    ::SetWindowText(_win->getHWND(),fullFile);
    _win->setTabTitle( _win->getEditorIdx(this), fullFile );
  }
  else
  {
    if( isDirty() )
    {
      ::SetWindowText(_win->getHWND(),"*~Untitled~*");

      _win->setTabTitle( _win->getEditorIdx(this), "*~Untitled~*" );
    }
    else
    {
      ::SetWindowText(_win->getHWND(),"Untitled");
      _win->setTabTitle( _win->getEditorIdx(this), "Untitled" );
    }
  }
}

void
MyEditor::showCmdEd()
{
  RECT rct;
  ::GetWindowRect(_win->getHWND(),&rct);
  ::MoveWindow(_cmdEd,0,0,rct.right-rct.left,20,FALSE);
  ::ShowWindow(_cmdEd,SW_SHOWNORMAL);
  //::SetWindowPos(_sci, NULL, 0, 0, rct.right, rct.bottom-00, SWP_NOZORDER);
  SendMessage(_win->getHWND(),WM_SIZE,0,0);
  ::SetFocus(_cmdEd);
}

void
MyEditor::hideCmdEd()
{
  ::ShowWindow(_cmdEd,SW_HIDE);
  ::SendMessage(_win->getHWND(),WM_SIZE,0,0);
}

bool
MyEditor::isCmdEdShown()
{
  return ::IsWindowVisible(_cmdEd);
}

void
MyEditor::undo()
{
  if( ::IsWindowVisible(_cmdEd) )
    SendMessage(_cmdEd,WM_UNDO,0,0);
  else
    BaseEditor::undo();
}

void
MyEditor::redo()
{
  if( ::IsWindowVisible(_cmdEd) )
    ;// cannot redo in a normal text edit SendMessage(_cmdEd,WM_REDO,0,0);
  else
    BaseEditor::redo();
}

void
MyEditor::cut()
{
  if( ::IsWindowVisible(_cmdEd) )
    SendMessage(_cmdEd,WM_CUT,0,0);
  else
    BaseEditor::cut();
}

void
MyEditor::copy()
{
  if( ::IsWindowVisible(_cmdEd) )
    SendMessage(_cmdEd,WM_COPY,0,0);
  else
    BaseEditor::copy();
}

void
MyEditor::paste()
{
  if( ::IsWindowVisible(_cmdEd) )
    SendMessage(_cmdEd,WM_PASTE,0,0);
  else
    BaseEditor::paste();
}

void
MyEditor::focus( void )
{
  hideCmdEd();
  ::SetFocus(_sci);
  ::SetForegroundWindow(_win->getHWND());
}


/*
Copyright (c) 2010-2011, Michael Mullin <masmullin@gmail.com>
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
