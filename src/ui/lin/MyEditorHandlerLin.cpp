#include "MyEditorHandlerLin.h"
#include "ScintillaSend.h"
#include <stdlib.h>

#include "MyTabWindow.h"MyEditorHandler*MyEditorHandlerLin::getInstance(){  if( !_me ) _me = new MyEditorHandlerLin();  return _me;}

MyEditor*
MyEditorHandlerLin::createEditor( char* file, int line )
{
  return createEditor(NULL,file,line);
}MyEditor*MyEditorHandlerLin::createEditor( MyTabWindow* win, char* file, int line ){
  char* f = fileFullPath(file);

  if( !win )
  {
    MyTabWindow* newWin = new MyTabWindow();
    _windowList.push_back(newWin);
    win = newWin;
  }
  MyEditor* newEd = new MyEditor(win,_idCount++,f);
  _editorList.push_back(newEd);
  win->addEditor(newEd);

  newEd->gotoLine(line-1);  _top = newEd;

  if( f ) free( f );  return newEd;}

MyEditor*
MyEditorHandlerLin::findEditor(int id)
{
  vector<MyEditor*>::const_iterator it = _editorList.begin();

  for( ; it != _editorList.end(); it++ )
    if( *it && (*it)->getID() == id )
      return *it;

  return NULL;
}

MyEditor*
MyEditorHandlerLin::findEditor( GtkWidget* w )
{
  vector<MyEditor*>::const_iterator it = _editorList.begin();

  for( ; it != _editorList.end(); it++ )
    if( *it && (*it)->getBox() == w )
      return *it;

  return NULL;
}

void
MyEditorHandlerLin::addTabWindow( MyTabWindow* mtw )
{
  vector<MyTabWindow*>::iterator it = _windowList.begin();

  for( ; it != _windowList.end(); it++ )
    if( *it == mtw )
    {
      return;
    }

  _windowList.push_back(mtw);
}

void
MyEditorHandlerLin::removeTabWindow( MyTabWindow* mtw )
{
  vector<MyTabWindow*>::iterator it = _windowList.begin();

  for( ; it != _windowList.end(); it++ )
    if( *it == mtw )
    {
      _windowList.erase(it);
      break;
    }
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
