#include "MyEditorCommon.h"
#include "ScintillaSend.h"
#include "common_defs.h"
#include "keywords.h"

#include "CScintillaController.h"
#include "TXTScintillaController.h"
#include "MKScintillaController.h"
#include "SQLScintillaController.h"
#include "PYScintillaController.h"
#include "PerlScintillaController.h"
#include "PHPScintillaController.h"
#include "SHScintillaController.h"
#include "BATScintillaController.h"

#if PLATFORM == LIN
#include <stdlib.h>
#endif

using namespace Scintilla;

#if PLATFORM == MAC

#define CONTROL_WINDOW_TITLE(me) [me controlWindowTitle]

#define IS_C(fType)       !strcmp(fType,"CFile")
#define IS_CPP(fType)     !strcmp(fType,"CPPFile")
#define IS_OBJC(fType)    !strcmp(fType,"ObjCFile")
#define IS_OBJCPP(fType)  !strcmp(fType,"ObjCppFile") || !strcmp(fType,"XCFile")
#define IS_H(fType)       !strcmp(fType,"HFile")
#define IS_J(fType)       !strcmp(fType,"JFile")
#define IS_TXT(fType)     !strcmp(fType,"TXTFile")
#define IS_MK(fType)      !strcmp(fType,"MKFile")
#define IS_SQL(fType)     !strcmp(fType,"SQLFile")
#define IS_PY(fType)      !strcmp(fType,"PYFile")
#define IS_PERL(fType)    !strcmp(fType,"PerlFile")
#define IS_PHP(fType)     !strcmp(fType,"PHPFile")
#define IS_SH(fType)      !strcmp(fType,"SHFile")
#define IS_BAT(fType)     !strcmp(fType,"BATFile")

#define IS_C_TYPE(me) \
  (([me getFType] < MAX_C_FILETYPES) && ([me getFType] >= CFILE))

#elif PLATFORM == WIN || PLATFORM == LIN

#define IS_C_TYPE(me) \
  ((me->getFType() < MAX_C_FILETYPES) && (me->getFType() >= CFILE))

#if PLATFORM == WIN
#define CONTROL_WINDOW_TITLE(me) me->controlWindowTitle()
#elif PLATFORM == LIN
#define CONTROL_WINDOW_TITLE(me) me->getTabWindow()->controlWindowTitle()
#endif

#define IS_C(fType)       !strcmp(fType,".c")
#define IS_CPP(fType)     !strcmp(fType,".cpp") || !strcmp(fType, ".cxx")      \
                       || !strcmp(fType,".C")
#define IS_OBJC(fType)    !strcmp(fType,".m")
#define IS_OBJCPP(fType)  !strcmp(fType,".mm")  || !strcmp(fType,".xc")        \
                       || !strcmp(fType,".y")   || !strcmp(fType,".yy")        \
                       || !strcmp(fType,".l")   || !strcmp(fType,".ll")
#define IS_H(fType)       !strcmp(fType,".h")
#define IS_J(fType)       !strcmp(fType,".java")
#define IS_TXT(fType)     !strcmp(fType,".txt")
#define IS_MK(fType)      !strcmp(fType,".mk")  || !strcmp(fType,".min")       \
                       || !strcmp(fType,".mak")
#define IS_SQL(fType)     !strcmp(fType,".sq")
#define IS_PY(fType)      !strcmp(fType,".py")
#define IS_PERL(fType)    !strcmp(fType,".pl")
#define IS_PHP(fType)     !strcmp(fType,".php")
#define IS_SH(fType)      !strcmp(fType,".sh")  || !strcmp(fType,".bash")
#define IS_BAT(fType)     !strcmp(fType,".bat") || !strcmp(fType,".cmd")       \
                       || !strcmp(fType,".batch")

#endif

void
HandleNotification
(
  MyEditor* me,
  SCNotification* scn
)
{
  void* editor = (void*)EDITOR(me);
  switch( scn->nmhdr.code )
  {
    case SCN_URIDROPPED:
    {
#if PLATFORM == MAC
      NSDocumentController* dc = [NSDocumentController sharedDocumentController];
      [dc openDocumentWithContentsOfURL:
            [NSURL fileURLWithPath:[NSString stringWithUTF8String:scn->text]]
                                display:YES
                                  error:nil];
#elif PLATFORM == LIN
      MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();
      MyEditor* top = meh->getTopEditor();
      MyTabWindow* mtw = NULL;
      if( top ) mtw = top->getTabWindow();
      char* f = fileFullPath((char*)scn->text);
      MyEditor* ed = meh->findEditor(f);
      if( ed )
        ed->focus();
      else
        meh->createEditor(mtw,(char*)scn->text,0);
      if( f ) free(f);
#endif
    }
    break;
    case SCN_NEEDSHOWN:
    {
      if( IS_C_TYPE(me) )
      {
        ScintillaSend* send = ScintillaSend::getInstance();
        int start = send->get(editor,SCI_LINEFROMPOSITION,scn->position);
        int end = send->get(editor,SCI_LINEFROMPOSITION,scn->position+scn->length);
        char charAt;
        int i = 0;

        do
        {
          charAt = send->get(editor,SCI_GETCHARAT,scn->position-i++);
        }while(charAt > 0 && (charAt != '{' && charAt != '}' && charAt != '#'));

        for (int line = start; line <= end; line++)
        {
          if( !(send->get(editor,SCI_GETFOLDLEVEL,line-1) & SC_FOLDLEVELHEADERFLAG)
              || (charAt == '{' || charAt == '#') 
            ) 
            send->set(editor, SCI_ENSUREVISIBLE, line,0);
        }
      }else
      {
        ScintillaSend* send = ScintillaSend::getInstance();
        int start = send->get(editor,SCI_LINEFROMPOSITION,scn->position);
        int end = send->get(editor,SCI_LINEFROMPOSITION,scn->position+scn->length);
        for (int line = start; line <= end; line++)
        {
          send->set(editor, SCI_ENSUREVISIBLE, line,0);
        }
      }
    }
    break;

    case SCN_CHARADDED:
    {
      if( scn->ch == '\n' )
      {
        ScintillaSend* send = ScintillaSend::getInstance();
        int pos = send->get(editor,SCI_GETCURRENTPOS);
        int line = send->get(editor,SCI_LINEFROMPOSITION,pos)-2;
        int eod = send->get(editor,SCI_GETTEXTLENGTH);
        // ensure that the next non-space character is either } or #
        char charAt;
        do
        {
          charAt = send->get(editor,SCI_GETCHARAT,pos++);
        }while( (charAt == 0x9 || charAt == 0x20) && pos < eod );

        if( charAt == '}' || charAt == '#' )
          send->set(editor, SCI_ENSUREVISIBLE, line,0);

        /* AUTO INDENT */
        {
          send->set(editor,SCI_COLOURISENOUPDATE,0,0);
          pos = send->get(editor,SCI_GETCURRENTPOS);
          char linebuf[1000];
          memset(linebuf,'\0',sizeof(linebuf));
          int  curLine    = send->get(editor,SCI_LINEFROMPOSITION,pos);

          if( curLine  >  0 ) 
          {
            int prevLineLength = send->get(editor,SCI_LINELENGTH, curLine-1,0);

            if( prevLineLength < 1000 ) 
            {
#if PLATFORM != MAC
              send->set(editor,SCI_GETLINE, curLine - 1,linebuf);
#else
              [editor getCharProperty: SCI_GETLINE
                            parameter: curLine-1
                                value: linebuf];
#endif
              //linebuf[prevLineLength] = '\0';
              for( int pos = 0; linebuf[pos] != '\0'; pos++ ) 
              {
                if( linebuf[pos] == '{' )
                {
                  linebuf[pos++] = ' ';
                  linebuf[pos++] = ' ';
                  linebuf[pos] = '\0';
                }
                else if( (linebuf[pos] != ' ') && (linebuf[pos] != '\t') )
                {
                  linebuf[pos] = '\0';
                }else if( linebuf[pos] != '\t' )
                {
                  linebuf[pos] = ' ';
                }
              }
              send->set(editor,SCI_REPLACESEL, 0, linebuf);
            }
          }
          //delete [] linebuf;
        }
      }
    }
    break;
    
    case SCN_KEY:
    {
#if PLATFORM == MAC
      if( scn->ch == 0x35 )
      {
        [me toggleCmdEd:nil];
      }
#endif      
      ;
    }
    break;

    case SCN_MARGINCLICK:
    {
      ScintillaSend* send = ScintillaSend::getInstance();
      send->set(editor,SCI_COLOURISE,0,-1);
      int line_num = send->get(editor,SCI_LINEFROMPOSITION, scn->position);
      send->set(editor,SCI_TOGGLEFOLD,line_num,0);
    }
    break;
    case SCN_PAINTED:
    {
      ;
    }
    break;

    case SCN_SAVEPOINTREACHED:
    {
      CONTROL_WINDOW_TITLE(me);
    }
    break;

    case SCN_SAVEPOINTLEFT:
    {
      CONTROL_WINDOW_TITLE(me);
    }
    break;

    default:
    {
      ;
    }
    break;
  }
}

fileType_t
GetFileType
(
  const char* type
)
{
  fileType_t rc = FILE_TYPE_ERROR;

  if( !type )
    return rc;

  if( IS_C(type) )
  {
    rc = CFILE;
  }
  else if( IS_CPP(type) )
  {
    rc = CPPFILE;
  }
  else if( IS_OBJC(type) )
  {
    rc = OBJCFILE;
  }
  else if( IS_OBJCPP(type) )
  {
    rc = OBJCPPFILE;
  }
  else if( IS_H(type) )
  {
    rc = HDFILE;
  }
  else if( IS_J(type) )
  {
    rc = JFILE;
  }
  else if( IS_TXT(type) )
  {
    rc = TXTFILE;
  }
  else if( IS_MK(type) )
  {
    rc = MKFILE;
  }
  else if( IS_SQL(type) )
  {
    rc = SQLFILE;
  }
  else if( IS_PY(type) )
  {
    rc = PYFILE;
  }
  else if( IS_PERL(type) )
  {
    rc = PERLFILE;
  }
  else if( IS_PHP(type) )
  {
    rc = PHPFILE;
  }
  else if( IS_SH(type) )
  {
    rc = SHFILE;
  }
  else if( IS_BAT(type) )
  {
    rc = BATFILE;
  }
  else
  {
    rc = OBJCPPFILE;
  }

  return rc;
}

ScintillaController*
GetSciType
(
  fileType_t fType
)
{
  ScintillaController* ed = NULL;
  
  if( fType < 0 ) return ed; // kick out early uninitialize or error

  switch( fType )
  {
    case CFILE:
    case CPPFILE:
    case OBJCFILE:
    case OBJCPPFILE:
    case HDFILE:
    case JFILE:
    {
      ed = CScintillaController::getInstance();
      break;
    }
    case TXTFILE:
    {
      ed = TXTScintillaController::getInstance();
      break;
    }
    case MKFILE:
    {
      ed = MKScintillaController::getInstance();
      break;
    }
    case SQLFILE:
    {
      ed = SQLScintillaController::getInstance();
      break;
    }
    case PYFILE:
    {
      ed = PYScintillaController::getInstance();
      break;
    }
    case PERLFILE:
    {
      ed = PerlScintillaController::getInstance();
      break;
    }
    case PHPFILE:
    {
      ed = PHPScintillaController::getInstance();
      break;
    }
    case SHFILE:
    {
      ed = SHScintillaController::getInstance();
      break;
    }
    case BATFILE:
    {
      ed = BATScintillaController::getInstance();
      break;
    }
    default:
    {
      ed = CScintillaController::getInstance();
    }
  }

  return ed;
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
