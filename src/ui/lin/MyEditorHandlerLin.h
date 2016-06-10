#ifndef __MY_EDITOR_HANDLER_LIN__#define __MY_EDITOR_HANDLER_LIN__#include "MyEditorHandler.h"

#include <vector>
using namespace std;#include "MyEditor.h"class MyEditorHandlerLin : public MyEditorHandler{private:
  int _idCount;  MyEditorHandlerLin() { _top = NULL; _idCount = 0; }
  
  vector<MyTabWindow*>  _windowList;protected:public:  static  MyEditorHandler* getInstance();
  virtual MyEditor* createEditor( char* file = NULL, int line = 0 );
  virtual MyEditor* findEditor( int id );
  virtual MyEditor* findEditor( GtkWidget* w );
  
  virtual MyEditor* createEditor( MyTabWindow* win  = NULL, 
                                  char* file        = NULL,
                                  int line          = 0 );
  
  MyTabWindow* findTabWindow( int id ){return NULL;}
  
  void addTabWindow( MyTabWindow* mtw );
  void removeTabWindow(MyTabWindow* mtw);

  using MyEditorHandler::findEditor;};#endif

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
