#include "MyEditorHandlerWin.h"

MyEditor*
  return createEditor(NULL,file,line);
}

  {
    MyTabWindow* newWin = new MyTabWindow(_hin);
    _windowList.push_back(newWin);
    win = newWin;
  }
    
  newEd->controlWindowTitle();

  newEd->gotoLine(line);
  ::SendMessage(newEd->getWindow(),WM_SIZE,0,0);

void
MyEditorHandlerWin::ripEditor( MyEditor* me, MyTabWindow* newMtw )
{
  MyTabWindow* oldWin = (MyTabWindow*)me->getTabWindow();
  
  if( oldWin->editorCount() == 1 && !newMtw )
    return; // only a single editor in tab? kick out  
  
  MyTabWindow* newWin = NULL;
  if( newMtw )
    newWin = newMtw;
  else
  {
    newWin = new MyTabWindow(_hin);
    _windowList.push_back(newWin);
  }
  oldWin->removeEditor(me);
  MyEditor* oed = oldWin->getTopEditor();
  if( oed )
    ::SendMessage(oed->getWindow(),WM_SIZE,0,0);

  me->setParentWin(newWin);
  ::SendMessage(me->getWindow(),WM_SIZE,0,0);
  me->controlWindowTitle();
  newWin->setTopEditor(me);
}


MyTabWindow*
MyEditorHandlerWin::findTabWindow(HWND hwnd)
{
  vector<MyTabWindow*>::const_iterator it = _windowList.begin();
}

void
MyEditorHandlerWin::removeTabWindow(MyTabWindow* mtw)
{
  vector<MyTabWindow*>::iterator it = _windowList.begin();
      if( *it )
      {
        delete *it;
        *it = NULL;
      }
      _windowList.erase(it);
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