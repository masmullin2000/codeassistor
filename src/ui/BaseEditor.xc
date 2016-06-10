#include "BaseEditor.h"
#include "ScintillaSend.h"

long
GetFileSize( const char* file )
{
  long rc = 0;
  struct STAT buf;
  
  if( !file ) return 0;
  
  if( !STAT(file,&buf) )
  {
    rc = buf.st_size;
  }
  
  return rc;
}

time_t
GetTime( const char* file )
{
  time_t mtime;
  struct STAT buf;

  memset(&mtime,0,sizeof(time_t));
    
  if( !STAT(file,&buf) )
  {
    mtime = buf.st_mtime;
  }
  return mtime;
}

BaseEditor::~BaseEditor()
{
  if( _ttf ) delete [] _ttf;
  if( _file) delete [] _file;

  _ttf = _file = NULL;
}

void
BaseEditor::undo()
{
  ScintillaSend* send = ScintillaSend::getInstance();
  send->get(_sci,SCI_UNDO,0,0);
}

void
BaseEditor::redo()
{
  ScintillaSend* send = ScintillaSend::getInstance();
  send->get(_sci,SCI_REDO,0,0);
}

void
BaseEditor::cut()
{
  ScintillaSend* send = ScintillaSend::getInstance();
  send->get(_sci,SCI_CUT,0,0);
}

void
BaseEditor::copy()
{
  ScintillaSend* send = ScintillaSend::getInstance();
  send->get(_sci,SCI_COPY,0,0);
}

void
BaseEditor::paste()
{
  ScintillaSend* send = ScintillaSend::getInstance();
  send->get(_sci,SCI_PASTE,0,0);
}

void
BaseEditor::toggleTabs()
{
  ScintillaSend* send = ScintillaSend::getInstance();
  int rc = send->get(_sci,SCI_GETUSETABS,0,0);
  if( rc ) send->set(_sci,SCI_SETUSETABS,0,0);
  else   send->set(_sci,SCI_SETUSETABS,1,0);
}

void
BaseEditor::toggleWrap( void )
{
  ScintillaSend* send = ScintillaSend::getInstance();
  if( _isWrap )
  {
    _isWrap = false;
    send->set(_sci,SCI_SETWRAPMODE,SC_WRAP_NONE,0);
    send->set(_sci,SCI_SETHSCROLLBAR,1,0);
  } else {
    _isWrap = true;
    send->set(_sci,SCI_SETWRAPMODE,SC_WRAP_WORD,0);
    send->set(_sci,SCI_SETHSCROLLBAR,0,0);
  }
}

void
BaseEditor::foldAll()
{
  ScintillaController* cont = ScintillaController::getInstance();

  cont->foldAll(_sci);
}

void
BaseEditor::unfoldAll()
{
  ScintillaController* cont = ScintillaController::getInstance();

  cont->unfoldAll(_sci);
}

void
BaseEditor::find( FindDirection dir, char* ttf )
{
  if( ttf )
    setTextToFind(ttf);
    
  if( !_ttf )
    return;

  ScintillaSend* send = ScintillaSend::getInstance();
  ScintillaController* sc = ScintillaController::getInstance();
  int end = send->get(_sci,SCI_GETLENGTH);
  int start = send->get(_sci,SCI_GETCURRENTPOS);
  
  sc->findText(_sci,_ttf,dir,false,start,end);
}

void
BaseEditor::setTextToFind( char* ttf )
{
  int len = strlen(ttf);
  if( !len )
    return;
    
  if( _ttf ) delete [] _ttf;
  
  _ttf = new char[len+1];
  strncpy(_ttf,ttf,len);
  _ttf[len] = '\0';
}

void
BaseEditor::gotoLine( unsigned int line )
{
  ScintillaController* sc = ScintillaController::getInstance();
  
  sc->goToLine(_sci,line);
}

void
BaseEditor::setDirtyOverride( bool isDirty )
{
  _dirtyOverride = isDirty;
  controlWindowTitle();
}

bool
BaseEditor::isDirty()
{
  ScintillaSend* send = ScintillaSend::getInstance();
  return _dirtyOverride ? true : send->get(_sci,SCI_GETMODIFY,0,0);
}

void
BaseEditor::setTouch()
{
  setTouch( GetTime(_file), GetFileSize(_file) );
}

void
BaseEditor::setTouch( time_t t, int sz )
{
  _fileTime = t;
  _fileSz = sz;
}

time_t
BaseEditor::getTime()
{
  return _fileTime;
}

bool
BaseEditor::checkUpdate()
{
  bool rc = FALSE;

  if( _file )
  {
    time_t checkTime = GetTime( _file );
    int sz = GetFileSize( _file );

#if PLATFORM == LIN
    /* potentially hazerdous kludge... g_file_set_contents doesn't save immediately */
    if( (checkTime-15 > _fileTime) || (sz != _fileSz) ) rc = TRUE;
#else
    if( (checkTime > _fileTime) || (sz != _fileSz) ) rc = TRUE;
#endif
  }
  return rc;
}

bool
BaseEditor::setFile(char* file)
{
  if( !file ) return false;

  if( _file && !strcmp(_file,file) )
  {
    // why reset if same ?
    return true;
  }

  if( _file ) delete [] _file;

  _file = new char[strlen(file)+1];

  strncpy(_file,file,strlen(file));
  _file[strlen(file)] = '\0';

  return true;
}

void
BaseEditor::toggleShowLineEndings()
{
  bool show = !getLineEndingsShown();
  ScintillaSend* send = ScintillaSend::getInstance();

  send->set(_sci,SCI_SETVIEWEOL,show,0);
}

bool
BaseEditor::getLineEndingsShown(void)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  return send->get(_sci,SCI_GETVIEWEOL);
}

void
BaseEditor::setLineEndings(int style)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  send->set(_sci,SCI_CONVERTEOLS,style,0);
  send->set(_sci,SCI_SETEOLMODE,style,0);
}

void
BaseEditor::zoom(bool in)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  if( in ) send->set(_sci,SCI_ZOOMIN,0,0);
  else     send->set(_sci,SCI_ZOOMOUT,0,0);
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
