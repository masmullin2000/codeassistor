#import "Scintilla.h"
#import "MyEditorCommon.h"
#import "MyEditor.h"
#import "MyEditorNotify.h"

#import "Platform.h"
#import "ScintillaCocoa.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace Scintilla;

void notification(intptr_t windowid, unsigned int iMessage, uintptr_t wParam, uintptr_t lParam)
{
  MyScintillaView* editor;
  MyEditor* me;
  switch( iMessage )
  {
    case WM_NOTIFY:
    {
      // Parent notification. Details are passed as SCNotification structure.
      SCNotification* scn = reinterpret_cast<SCNotification*>(lParam);
      ScintillaCocoa *psc = reinterpret_cast<ScintillaCocoa*>(scn->nmhdr.hwndFrom);
      editor = static_cast<MyScintillaView*>(reinterpret_cast<SCIContentView*>(psc->ContentView()).owner);
      me = (MyEditor*)[editor owner];
      HandleNotification(me,scn);
      /* special handling to update the infobar */
      if( (scn->nmhdr.code == SCN_UPDATEUI) && ([editor backend] != NULL) )
      {
        NSPoint caretPosition = [editor backend]->GetCaretPosition();
        [[editor infoBar] notify: IBNCaretChanged message: nil location: caretPosition value: 0];
      }
    }
    break;

    case WM_COMMAND:
    {
      ScintillaCocoa* backend = reinterpret_cast<ScintillaCocoa*>(lParam);
      editor = static_cast<MyScintillaView*>(backend->TopContainer());
      switch (wParam >> 16)
      {
        case SCEN_SETFOCUS:
        {
          me = (MyEditor*)[editor owner];
          [me checkUpdate];
          [me hideCmdEd: nil];
        }
        break;
        case SCEN_KILLFOCUS:
          break;
      }
    }
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
