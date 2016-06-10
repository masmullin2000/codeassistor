#include "ScintillaController.h"
#include "ScintillaSend.h"
#include <stdio.h>
#include <list>

using namespace std;

ScintillaController* ScintillaController::_me = NULL;

/* The CScintillaController is a singleton */
ScintillaController*
ScintillaController::getInstance()
{
  if( !_me ) _me = new ScintillaController();

  return _me;
}

void
ScintillaController::setDefaultEditor
(
  void* sci,
  fileType_t subtype
)
{
  ScintillaSend* send = ScintillaSend::getInstance();
 
  // I like Courier as my main font.  Start at a decent size
  send->set(sci,SCI_STYLESETFONT,STYLE_DEFAULT,DEFAULT_FONT);
  send->set(sci,SCI_STYLESETSIZE,STYLE_DEFAULT,INITIAL_TEXT_SIZE);

  send->set(sci,SCI_SETEDGEMODE,0,0);
  send->set(sci,SCI_STYLECLEARALL,0,0);
  send->set(sci,SCI_SETMARGINSENSITIVEN,1,1);

  send->set(sci,SCI_SETFOLDMARGINCOLOUR,1,0xEEEEEE);
  send->set(sci,SCI_SETFOLDMARGINHICOLOUR,1,0xEEEEEE);

  send->set(sci,SCI_SETSCROLLWIDTHTRACKING,1,0);
  send->set(sci,SCI_SETSCROLLWIDTH,80,0);
  send->set(sci,SCI_SETRECTANGULARSELECTIONMODIFIER,SCMOD_ALT,0);
}

bool
ScintillaController::findText
(
  void* sci,
  const char* text,
  FindDirection dir,
  bool isRegex,
  int start,
  int end,
  int* oStart,
  int* oEnd,
  bool loop
) 
{
  ScintillaSend* send = ScintillaSend::getInstance();
  bool rv = false;
  int rc = -1;
  int sf = 0;
  if( start == 0 && dir == UP ) start = end;
  else if( start == end && dir == DOWN ) start = 0;
  
  if( oStart )
    *oStart = -1;
  if( oEnd )
    *oEnd = -1;
  
  if( isRegex )
    sf = SCFIND_REGEXP | SCFIND_POSIX | SCFIND_MATCHCASE;

  struct Scintilla::Sci_TextToFind ttf;
  ttf.chrg.cpMin = start;
  ttf.chrg.cpMax = dir == UP ? 0 : end;
  ttf.chrgText.cpMin = ttf.chrgText.cpMax = -1;
  ttf.lpstrText = (char*)text;

  send->set(sci,SCI_FINDTEXT,sf,&ttf);
  rc = ttf.chrgText.cpMin;

  if( rc < 1 && loop )
  {
    ttf.chrg.cpMin = dir == UP ? end : 0;
    send->set(sci,SCI_FINDTEXT,sf,&ttf);
    rc = ttf.chrgText.cpMin;
  }

  if( rc > -1 )
  { 
    end   = dir ? ttf.chrgText.cpMax : ttf.chrgText.cpMin;
    start = dir ? ttf.chrgText.cpMin : ttf.chrgText.cpMax;
    
    if( oEnd ) *oEnd = end;
    if( oStart ) *oStart = start;
    
    if( oEnd && oStart && *oEnd < *oStart )
    {
      int tmp = *oEnd;
      *oEnd = *oStart;
      *oStart = tmp;
    }

    int line = send->get(sci,SCI_LINEFROMPOSITION,start);

    send->set(sci,SCI_ENSUREVISIBLE,line,0);
    send->set(sci,SCI_GOTOLINE,line-5,0);
    send->set(sci,SCI_GOTOLINE,line+5,0);
    send->set(sci,SCI_SETSEL,start,end);
    rv = true;
  }

  return rv;
}

bool
ScintillaController::goToLine
(
  void* sci,
  unsigned int line
)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  send->set(sci,SCI_COLOURISE,0,-1);
  send->set(sci,SCI_ENSUREVISIBLE,line,0);

  send->set(sci,SCI_GOTOLINE,line-5,0);
  send->set(sci,SCI_GOTOLINE,line+5,0);
  send->set(sci,SCI_GOTOLINE,line,0);

  return 0;
}

void
ScintillaController::foldAll(void* sci)
{
  int count = 0;

  ScintillaSend* send = ScintillaSend::getInstance();
  send->set(sci,SCI_COLOURISE,0,-1);
  int startPos = send->get(sci,SCI_GETCURRENTPOS);
  int startLine = send->get(sci,SCI_LINEFROMPOSITION,startPos);
  int firstLine = send->get(sci,SCI_GETFIRSTVISIBLELINE);
      firstLine = send->get(sci,SCI_DOCLINEFROMVISIBLE,firstLine);
  int maxLine = send->get(sci,SCI_GETLINECOUNT,0);

  for( int i = firstLine; i < startLine; i++ )
  {
    if( send->get(sci,SCI_GETLINEVISIBLE,i) )
    {
      count++;
    }
  }

  list<int> toggles;

  for( int line = 0; line <= maxLine; line++ )
  {
    int fl = send->get(sci,SCI_GETFOLDLEVEL,line);
    if( fl & SC_FOLDLEVELHEADERFLAG )
    {
      if( send->get(sci,SCI_GETLINEVISIBLE,line+1) 
      && (send->get(sci,SCI_GETLASTCHILD,line,-1) != line))
      {
        toggles.push_back(line);
      }
    }
  }
  
  for( list<int>::const_iterator it = toggles.begin(); it != toggles.end(); it++ )
  {
    send->set(sci,SCI_TOGGLEFOLD,*it,0);
  }

  send->set(sci,SCI_COLOURISENOUPDATE,0,-1);
  send->set(sci,SCI_ENSUREVISIBLE,startLine,0);
  startLine = send->get(sci,SCI_VISIBLEFROMDOCLINE,startLine);

  send->set(sci,SCI_SETFIRSTVISIBLELINE,startLine,0);
  send->set(sci,SCI_COLOURISENOUPDATE,0,-1);
  firstLine = send->get(sci,SCI_GETFIRSTVISIBLELINE);
  count -= (startLine-firstLine);
  send->set(sci,SCI_LINESCROLL,0,-count);
}

void
ScintillaController::unfoldAll(void* sci)
{
  int count = 0;
  ScintillaSend* send = ScintillaSend::getInstance();
  int startPos = send->get(sci,SCI_GETCURRENTPOS);
  int startLine = send->get(sci,SCI_LINEFROMPOSITION,startPos);
  int firstLine = send->get(sci,SCI_GETFIRSTVISIBLELINE);
      firstLine = send->get(sci,SCI_DOCLINEFROMVISIBLE,firstLine);

  for( int i = firstLine; i < startLine; i++ )
  {
    if( send->get(sci,SCI_GETLINEVISIBLE,i) )
    {
      count++;
    }
  }

  send->set(sci,SCI_COLOURISENOUPDATE,0,-1);
  int maxLine = send->get(sci,SCI_GETLINECOUNT,0);

  for( int line = 0; line < maxLine; line++ )
  {
    int fl = send->get(sci,SCI_GETFOLDLEVEL,line);
    if( fl & SC_FOLDLEVELHEADERFLAG )
    {
      if( !send->get(sci,SCI_GETLINEVISIBLE,line+1) )
      {
        send->set(sci,SCI_TOGGLEFOLD,line,0);
      }
    }
  }

  send->set(sci,SCI_COLOURISENOUPDATE,0,-1);
  send->set(sci,SCI_ENSUREVISIBLE,startLine,0);
  startLine = send->get(sci,SCI_VISIBLEFROMDOCLINE,startLine);

  send->set(sci,SCI_SETFIRSTVISIBLELINE,startLine,0);
  send->set(sci,SCI_COLOURISENOUPDATE,0,-1);
  firstLine = send->get(sci,SCI_GETFIRSTVISIBLELINE);
  count -= (startLine-firstLine);
  send->set(sci,SCI_LINESCROLL,0,-count);

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
