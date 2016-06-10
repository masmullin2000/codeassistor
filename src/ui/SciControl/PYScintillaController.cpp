/** title info */
#include "PYScintillaController.h"
#include "ScintillaSend.h"
#include "common_defs.h"

/* colourizing defines */
#define DEF_KEYW_1    "#FF627E"
#define DEF_KEYW_2    "#FF00FF"
#define DEF_NUM       "#0000FF"
#define DEF_OP        "#0000FF"
#define DEF_STRING    "#EE9A4D"
#define DEF_CHAR      "#EE9A4D"
#define DEF_PREPRO    "#C35617"
#define DEF_IDENT     "#000000"
#define DEF_COMMENT   "#777777"

#define TAB_WIDTH     2

PYScintillaController* PYScintillaController::_me = NULL;

/* The PYScintillaController is a singleton */
ScintillaController*
PYScintillaController::getInstance()
{
  if( !_me ) _me = new PYScintillaController();

  return _me;
}

void
PYScintillaController::setDefaultEditor
(
  void* sci,
  fileType_t subtype
)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  const char* keywordsToUse = PythonKeywords;
  const char* secondaryKeywordsToUse = "";
  ScintillaController::setDefaultEditor(sci,subtype);

  // CPP is used, this is also good for ObjC/Java/C
  send->set(sci,SCI_SETLEXER,SCLEX_PYTHON,0);

  // tabs are replaced with spaces, tabs are 2 spaces wide
  send->set(sci,SCI_SETUSETABS,0,0);
  send->set(sci,SCI_SETTABWIDTH,TAB_WIDTH,0);

  send->set(sci,SCI_SETKEYWORDS,0,(void*)keywordsToUse);
  send->set(sci,SCI_SETKEYWORDS,1,(void*)secondaryKeywordsToUse);

  // set the font for strings (ie "this is a string" ) as italic of courier new
  send->set(sci,SCI_STYLESETFONT,SCE_P_STRING,ITALIC_STRING);
  send->set(sci,SCI_STYLESETITALIC,SCE_P_STRING,1);
  send->set(sci,SCI_STYLESETBOLD,SCE_P_STRING,1);
  send->set(sci,SCI_STYLESETSIZE,SCE_P_STRING,INITIAL_TEXT_SIZE);

  send->set(sci,SCI_STYLESETFONT,SCE_P_CHARACTER,ITALIC_STRING);
  send->set(sci,SCI_STYLESETITALIC,SCE_P_CHARACTER,1);
  send->set(sci,SCI_STYLESETBOLD,SCE_P_CHARACTER,1);
  send->set(sci,SCI_STYLESETSIZE,SCE_P_CHARACTER,INITIAL_TEXT_SIZE);

  // colourizing
  // keywords
  send->set(sci,SCI_STYLESETFORE,SCE_P_WORD,DEF_KEYW_1,true);

  // c standard lib
  send->set(sci,SCI_STYLESETFORE,SCE_P_WORD2,DEF_KEYW_2,true);

  // numbers
  send->set(sci,SCI_STYLESETFORE,SCE_P_NUMBER,DEF_NUM,true);

  // operators/operations
  send->set(sci,SCI_STYLESETFORE,SCE_P_OPERATOR,DEF_OP,true);

  // strings
  send->set(sci,SCI_STYLESETFORE,SCE_P_STRING,DEF_STRING,true);

  // 'a' characters
  send->set(sci,SCI_STYLESETFORE,SCE_P_CHARACTER,DEF_CHAR,true);

  // preprocessors #define/#include
  send->set(sci,SCI_STYLESETFORE,SCE_P_DECORATOR,DEF_PREPRO,true);
  send->set(sci,SCI_STYLESETFORE,SCE_P_CLASSNAME,DEF_PREPRO,true);
  send->set(sci,SCI_STYLESETFORE,SCE_P_DEFNAME,DEF_PREPRO,true);

  // identifiers aka everything else
  send->set(sci,SCI_STYLESETFORE,SCE_P_IDENTIFIER,DEF_IDENT,true);

  // comments
  send->set(sci,SCI_STYLESETFORE,SCE_P_COMMENTBLOCK,DEF_COMMENT,true);

  send->set(sci,SCI_STYLESETFORE,SCE_P_COMMENTLINE,DEF_COMMENT,true);

  // Line numbering
  send->set(sci,SCI_STYLESETFORE,STYLE_LINENUMBER,"#000000",true);
  send->set(sci,SCI_STYLESETBACK,STYLE_LINENUMBER,"#DDDDDD",true);
  send->set(sci,SCI_SETMARGINTYPEN,0,SC_MARGIN_NUMBER);
  send->set(sci,SCI_SETMARGINWIDTHN,0,48);

  // long lines
  send->set(sci,SCI_SETEDGEMODE,1,0);
  send->set(sci,SCI_SETEDGECOLUMN,80,0);

  // folding
  send->set(sci,SCI_SETMARGINWIDTHN,1,18);
  send->set(sci,SCI_SETFOLDFLAGS,SC_FOLDFLAG_LINEAFTER_CONTRACTED,0);
  send->set(sci,SCI_SETPROPERTY,"fold","1");
  send->set(sci,SCI_SETPROPERTY,"fold.compact","0");
  send->set(sci,SCI_SETPROPERTY,"fold.at.else","1");

  send->set(sci,SCI_SETMARGINTYPEN,  1, SC_MARGIN_SYMBOL);
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
