#include "keywords.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "common_defs.h"

#ifndef __SCINTILLA_CONTROLLER__
#define __SCINTILLA_CONTROLLER__

#if PLATFORM==MAC
#define INITIAL_TEXT_SIZE 14
#define DEFAULT_FONT "Courier"
#define ITALIC_STRING "Courier New"
#elif PLATFORM==WIN
#define INITIAL_TEXT_SIZE 10
#define DEFAULT_FONT "Courier New"
#define ITALIC_STRING "Courier New"
#elif PLATFORM==LIN
#define INITIAL_TEXT_SIZE 10
#define DEFAULT_FONT "!Monospace"
#define ITALIC_STRING "!Monospace"
#endif
 
enum FindDirection
{
  UP,
  DOWN
};

class ScintillaController
{
private:
  static ScintillaController* _me;
protected:
  ScintillaController() {}
  ~ScintillaController();
public:
  static ScintillaController* getInstance();
 
  virtual void  setDefaultEditor
  (
    void* sci,
    fileType_t subtype
  );// {} // nothing to be done for bare sci controller

  virtual bool  findText
  (
    void* sci,
    const char* text,
    FindDirection dir,
    bool isRegex,
    int start,
    int end,
    int* oStart = NULL,
    int* oEnd = NULL,
    bool loop = true
  );

  virtual bool  goToLine
  (
    void* sci,
    unsigned int line
  );

  virtual void  foldAll
  (
    void* sci
  );
  virtual void  unfoldAll
  (
    void* sci
  );
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
