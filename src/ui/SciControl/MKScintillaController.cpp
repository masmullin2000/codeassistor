/** title info */

#include "MKScintillaController.h"
#include "ScintillaSend.h"

/* colourizing defines */
#define DEF_KEYW_1    "#FF627E"
#define DEF_KEYW_2    "#FF00FF"
#define DEF_NUM       "#FF0000"
#define DEF_OP        "#0000FF"
#define DEF_STRING    "#EE9A4D"
#define DEF_CHAR      "#0022FF"
#define DEF_PREPRO    "#C35617"
#define DEF_IDENT     "#000000"
#define DEF_COMMENT   "#777777"

#define TAB_WIDTH     4

MKScintillaController* MKScintillaController::_me = NULL;

/* The CScintillaController is a singleton */
ScintillaController*
MKScintillaController::getInstance()
{
  if( !_me ) _me = new MKScintillaController();

  return _me;
}

void
MKScintillaController::setDefaultEditor
(
  void* sci,
  fileType_t subtype
)
{
  ScintillaSend* send = ScintillaSend::getInstance();

  ScintillaController::setDefaultEditor(sci,subtype);

  send->set(sci,SCI_SETLEXER,SCLEX_MAKEFILE,0);

  send->set(sci,SCI_SETTABWIDTH,TAB_WIDTH,0);

  send->set(sci,SCI_STYLESETFORE,SCE_MAKE_OPERATOR,DEF_OP,true);
  send->set(sci,SCI_STYLESETFORE,SCE_MAKE_COMMENT,DEF_COMMENT,true);
  send->set(sci,SCI_STYLESETFORE,SCE_MAKE_PREPROCESSOR,DEF_PREPRO,true);
  send->set(sci,SCI_STYLESETFORE,SCE_MAKE_TARGET,DEF_KEYW_1,true);
  send->set(sci,SCI_STYLESETFORE,SCE_MAKE_IDENTIFIER,DEF_KEYW_2,true);
  send->set(sci,SCI_STYLESETFORE,STYLE_LINENUMBER,"#000000",true);
  send->set(sci,SCI_STYLESETBACK,STYLE_LINENUMBER,"#DDDDDD",true);

  // Line numbering
  send->set(sci,SCI_SETMARGINTYPEN,0,SC_MARGIN_NUMBER);
  send->set(sci,SCI_SETMARGINWIDTHN,0,48);
  send->set(sci,SCI_SETMARGINWIDTHN,1,0);
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
