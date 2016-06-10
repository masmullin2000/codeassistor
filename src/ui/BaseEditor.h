#ifndef __BASE_EDITOR_H__
#define __BASE_EDITOR_H__

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "common_defs.h"

#include <Scintilla.h>
#include <SciLexer.h>

#include "ScintillaController.h"
#include "MyCommand.h"

using namespace Scintilla;

#if PLATFORM == WIN
 #include <windows.h>
 #define SciComp HWND
 #define STAT _stat
#elif PLATFORM == LIN
 #include <gtk/gtk.h>
 #define PLAT_GTK 1
 #include <ScintillaWidget.h>
 #define SciComp ScintillaObject*
 #define STAT stat
#elif PLATFORM == MAC
 #include "ScintillaView.h"
 #define SciComp ScintillaView**
 #define STAT stat
#endif

#if PLATFORM == WIN || PLATFORM == LIN
 #define _sci _comp
#elif PLATFORM == MAC
 #define _sci *_comp
#endif

long    GetFileSize( const char* file );
time_t  GetTime( const char* file );

class BaseEditor
{
private:
protected:
  SciComp _comp;
  bool _isWrap;

  char* _ttf;  
  char* _file;
  time_t _fileTime;
  int _fileSz;

  bool _dirtyOverride;
public:
  BaseEditor(SciComp sci = NULL):
    _isWrap(false),
    _ttf(NULL),
    _file(NULL),
    _fileTime(0),
    _dirtyOverride(false)
    { if( sci ) _comp = sci; _cmd.setStartPos(0); }

  virtual ~BaseEditor();

  virtual void undo();
  virtual void redo();
  virtual void cut();
  virtual void copy();
  virtual void paste();
  virtual void toggleTabs();

  void toggleWrap();

  void foldAll();
  void unfoldAll();

  void setTextToFind( char* ttf );
  void find( FindDirection dir = DOWN, char* ttf = NULL );
  void gotoLine( unsigned int line );

  const SciComp getEditor() { return (const SciComp)_comp; }
  const char* getFile() { return _file; }

  virtual bool setFile(char* file);

  void setDirtyOverride( bool isDirty );
  bool isDirty();

  void setTouch(void);
  void setTouch( time_t t, int sz );
  time_t getTime();
  int getFileSize() { return _fileSz; }
  virtual bool checkUpdate();

  virtual void controlWindowTitle( void ) {;}
  
  virtual void toggleShowLineEndings(void);
  virtual bool getLineEndingsShown(void);
  virtual void setLineEndings(int style);
  virtual void zoom(bool in);
  
  MyCommand _cmd;
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
