/** title info */
#include "CScintillaController.h"
#include "ScintillaSend.h"
#include "common_defs.h"

/* colourizing defines */
#if PLATFORM == MAC
#define DEF_KEYW_1    "#FF627E"
#define DEF_KEYW_2    "#FF00FF"
#define DEF_NUM       "#0000FF"
#define DEF_OP        "#0000FF"
#define DEF_STRING    "#EE9A4D"
#define DEF_STRING_EOL  "#D4A9EE"
#define DEF_CHAR      "#0022FF"
#define DEF_PREPRO    "#C35617"
#define DEF_IDENT     "#000000"
#define DEF_COMMENT   "#777777"
#define DEF_COMMENT_2 "#444444"
#define DEF_MARGIN    "#DADADA"
#define DEF_LINE_NO     "#CFCFCF"
#else
#define DEF_KEYW_1      "#FF627E"
#define DEF_KEYW_2      "#FF00FF"
#define DEF_NUM         "#AAAAFF"
#define DEF_OP          "#DDAADD"
#define DEF_STRING      "#EE9A4D"
#define DEF_STRING_EOL  "#D4A9EE"
#define DEF_CHAR        "#0022FF"
#define DEF_PREPRO      "#C35617"
#define DEF_IDENT       "#999988"
#define DEF_COMMENT     "#777777"
#define DEF_COMMENT_2   "#444444"
#define DEF_MARGIN      "#323232"
#define DEF_LINE_NO     "#434343"
#endif

#define TAB_WIDTH     2

CScintillaController* CScintillaController::_me = NULL;

/* The CScintillaController is a singleton */
ScintillaController*
CScintillaController::getInstance()
{
  if( !_me ) _me = new CScintillaController();

  return _me;
}

void
CScintillaController::setDefaultEditor
(
  void* sci,
  fileType_t subtype
)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  const char* keywordsToUse = NULL;
  const char* secondaryKeywordsToUse = CStdLibrary;
  ScintillaController::setDefaultEditor(sci,subtype);

#if PLATFORM != MAC
  send->set(sci,SCI_STYLESETBACK,STYLE_DEFAULT,0);
  send->set(sci,SCI_STYLESETFORE,STYLE_DEFAULT,0);
  send->set(sci,SCI_SETCARETFORE,255,0);
  send->set(sci,SCI_STYLECLEARALL,0,0);
#endif
  // CPP is used, this is also good for ObjC/Java/C
  send->set(sci,SCI_SETLEXER,SCLEX_CPP,0);

  send->set(sci,SCI_SETSTYLEBITS,send->get(sci,SCI_GETSTYLEBITSNEEDED),0);

  // tabs are replaced with spaces, tabs are 2 spaces wide
  send->set(sci,SCI_SETUSETABS,0,0);
  send->set(sci,SCI_SETTABWIDTH,TAB_WIDTH,0);

  switch( subtype )
  {
  case CFILE:
    keywordsToUse = CKeywords;
    break;
  case CPPFILE:
    keywordsToUse = CppKeywords;
    break;
  case OBJCFILE:
    keywordsToUse = ObjCKeywords;
    break;
  case OBJCPPFILE:
    keywordsToUse = ObjCppKeywords;
    break;
  case HDFILE:
    keywordsToUse = HKeywords;
    break;
  case JFILE:
    keywordsToUse = JKeywords;
    secondaryKeywordsToUse = JClasses;
    break;
  default:
    keywordsToUse = "error";
  }

  send->set(sci,SCI_SETKEYWORDS,0,(void*)keywordsToUse);
  send->set(sci,SCI_SETKEYWORDS,1,(void*)secondaryKeywordsToUse);

  for( int i = 0; i <= SCE_C_USERLITERAL; i++ )
    send->set(sci,SCI_STYLESETFORE,i,"#FFFFFF",true);

  send->set(sci,SCI_SETSELFORE,true,255|255<<8|255<<16);
  send->set(sci,SCI_SETSELBACK,true,64|64<<8|64<<16);

  // set the font for strings (ie "this is a string" ) as itali
  send->set(sci,SCI_STYLESETFONT,SCE_C_STRING,ITALIC_STRING);
  send->set(sci,SCI_STYLESETITALIC,SCE_C_STRING,1);
  send->set(sci,SCI_STYLESETBOLD,SCE_C_STRING,1);
  send->set(sci,SCI_STYLESETSIZE,SCE_C_STRING,INITIAL_TEXT_SIZE);

  // colourizing

  // keywords
  send->set(sci,SCI_STYLESETFORE,SCE_C_WORD,DEF_KEYW_1,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_WORD|0x40,DEF_COMMENT,true);

  // c standard lib
  send->set(sci,SCI_STYLESETFORE,SCE_C_WORD2,DEF_KEYW_2,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_WORD2|0x40,DEF_COMMENT,true);

  // numbers
  send->set(sci,SCI_STYLESETFORE,SCE_C_NUMBER,DEF_NUM,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_NUMBER|0x40,DEF_COMMENT,true);

  // operators/operations
  send->set(sci,SCI_STYLESETFORE,SCE_C_OPERATOR,DEF_OP,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_OPERATOR|0x40,DEF_COMMENT,true);

  // strings
  send->set(sci,SCI_STYLESETFORE,SCE_C_STRING,DEF_STRING,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_STRING|0x40,DEF_COMMENT,true);

  send->set(sci,SCI_STYLESETFORE,SCE_C_STRINGEOL,DEF_STRING_EOL,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_STRINGEOL|0x40,DEF_COMMENT,true);

  // 'a' characters
  send->set(sci,SCI_STYLESETFORE,SCE_C_CHARACTER,DEF_CHAR,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_CHARACTER|0x40,DEF_COMMENT,true);

  // preprocessors #define/#include
  send->set(sci,SCI_STYLESETFORE,SCE_C_PREPROCESSOR,DEF_PREPRO,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_PREPROCESSOR|0x40,DEF_COMMENT,true);

  // identifiers aka everything else
  send->set(sci,SCI_STYLESETFORE,SCE_C_IDENTIFIER,DEF_IDENT,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_IDENTIFIER|0x40,DEF_COMMENT,true);

  // comments
  send->set(sci,SCI_STYLESETFORE,SCE_C_COMMENT,DEF_COMMENT,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_COMMENTLINE,DEF_COMMENT,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_COMMENTDOC,DEF_COMMENT,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_COMMENTLINEDOC,DEF_COMMENT,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_COMMENTDOCKEYWORD,DEF_COMMENT_2,true);
  send->set(sci,SCI_STYLESETFORE,SCE_C_COMMENTDOCKEYWORDERROR,DEF_COMMENT_2,true);

//  send->set(sci,SCI_STYLESETFORE,SCE_C_GLOBALCLASS,"#FFFFFF",true);

  // Line numbering
  send->set(sci,SCI_STYLESETFORE,STYLE_LINENUMBER,DEF_IDENT,true);
  send->set(sci,SCI_STYLESETBACK,STYLE_LINENUMBER,DEF_LINE_NO,true);
  send->set(sci,SCI_SETMARGINTYPEN,0,SC_MARGIN_NUMBER);
  send->set(sci,SCI_SETMARGINWIDTHN,0,48);

  // long lines
  send->set(sci,SCI_SETEDGEMODE,1,0);
  send->set(sci,SCI_SETEDGECOLUMN,80,0);

  // folding
  send->set(sci,SCI_SETFOLDMARGINCOLOUR,true,DEF_MARGIN,true);
  send->set(sci,SCI_SETFOLDMARGINHICOLOUR,true,DEF_MARGIN,true);

  send->set(sci,SCI_SETMARGINWIDTHN,1,18);
  send->set(sci,SCI_SETFOLDFLAGS,SC_FOLDFLAG_LINEAFTER_CONTRACTED,0);
  send->set(sci,SCI_SETPROPERTY,"fold","1");
  send->set(sci,SCI_SETPROPERTY,"fold.compact","0");
  send->set(sci,SCI_SETPROPERTY,"fold.at.else","1");
  send->set(sci,SCI_SETPROPERTY,"fold.preprocessor","1");
  // these new preprocessor features aren't working out so well for me
  send->set(sci,SCI_SETPROPERTY,"lexer.cpp.track.preprocessor","0");
  send->set(sci,SCI_SETPROPERTY,"lexer.cpp.update.preprocessor","0");

  send->set(sci,SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
  send->set(sci,SCI_SETMARGINMASKN, 1, SC_MASK_FOLDERS);

  send->set(sci,SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_ARROWDOWN);
  send->set(sci,SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_ARROW);
  send->set(sci,SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
  send->set(sci,SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
  send->set(sci,SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
  send->set(sci,SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
  send->set(sci,SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
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
