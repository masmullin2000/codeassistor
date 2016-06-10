#include "common_defs.h"

#include <stdio.h>
#include <string.h>

#if PLATFORM == MAC
 #include "ScintillaView.h"
 #define LOCAL_SCI(x,y) ScintillaView* x = (ScintillaView*)y
#elif PLATFORM == WIN
 #include <windows.h>
 #include "Scintilla.h"
 using namespace Scintilla;
 //#define LOCAL_SCI(x,y) HWND x = (HWND)y
 
 HWND last_sci                  = NULL;
 int (*fn)(void*,int,int,int)   = NULL;
 void* ptr                      = NULL;

 #define LOCAL_SCI(x,y)              \
 HWND x = (HWND)y;                   \
 {                                   \
   if( (last_sci == NULL) ||         \
       (last_sci != x)    ||         \
       (fn == NULL)       ||         \
       (ptr == NULL) )               \
   {                                 \
     fn = (int (__cdecl*)(void*,int,int,int))SendMessage(lsci,SCI_GETDIRECTFUNCTION,0,0); \
     ptr = (void*)SendMessage(lsci,SCI_GETDIRECTPOINTER,0,0); \
     last_sci = x;                   \
   }                                 \
 } // end define local_sci
#elif PLATFORM == LIN
 #include <gtk/gtk.h>
 #include <Scintilla.h>
 using namespace Scintilla;
 #include <SciLexer.h>
 #define PLAT_GTK 1
 #include <ScintillaWidget.h>

 #define LOCAL_SCI(x,y) ScintillaObject* x = (ScintillaObject*)y

#else
 #error // no other platforms yet
#endif

#include "ScintillaSend.h"

ScintillaSend* ScintillaSend::_me = NULL;

// this needs to be unit tested
int xtoi(char* str, int len)
{
  int rc = 0;
  
  if( !(str && strlen(str) >= (unsigned int)len) && len < 5 )
    return -1;
  
  for(int i = 0; i < len; i++ )
  {
    int val = 0;
    char c = str[i];
    if( c >= '0' && c <= '9' )
      val += c - '0';
    else if( c >= 'a' && c <= 'f' )
      val += c - 'a' + 10;
    else if( c >= 'A' && c <= 'F' )
      val += c - 'A' + 10;
    else
      return -1;
    
    rc += val << 4 * (len - i -1);
  }
  return rc;
}

ScintillaSend*
ScintillaSend::getInstance()
{
  if( !_me ) _me = new ScintillaSend();

  return _me;
}

// set general
long
ScintillaSend::set
( 
  void* sci,
  int   property,
  int   param0,
  int   param1
)
{
  LOCAL_SCI(lsci,sci);
  long rc = 0;

#if PLATFORM == MAC
  rc = [lsci setGeneralProperty: property
                 parameter: param0
                     value: param1];
#elif PLATFORM == WIN
  //SendMessage(lsci,property,param0,(LPARAM)param1);

  rc = fn(ptr,property,param0,param1);

#elif PLATFORM == LIN
  rc = scintilla_send_message(lsci,property,param0,param1);
#else
  #error // no other platforms yet
#endif

  return rc;
}

// set reference
long
ScintillaSend::set
(
  void* sci,
  int   property,
  int   param0,
  void* param1
)
{
  LOCAL_SCI(lsci,sci);
  long rc = 0;

#if PLATFORM==MAC
  rc = [lsci setReferenceProperty: property
                   parameter: param0
                       value: param1];
#elif PLATFORM == WIN
  //SendMessage(lsci,property,param0,(LPARAM)param1);
  rc = fn(ptr,property,param0,(int)param1);
#elif PLATFORM == LIN
  rc = scintilla_send_message(lsci,property,param0,(sptr_t)param1);
#else
  #error // no other platforms yet
#endif

  return rc;
}

  // set string/colour
long 
ScintillaSend::set
(
  void* sci,
  int   property,
  int   param0,
  char* param1,
  bool  isColour
)
{

  LOCAL_SCI(lsci,sci);
  long rc = 0;

#if PLATFORM == MAC
  if( isColour == false )
  {
    rc = [lsci setStringProperty: property
                  parameter: param0
                      value: [NSString stringWithUTF8String:param1]];
  }
  else
  {
    rc = [lsci setColorProperty: property
                 parameter: param0
                  fromHTML: [NSString stringWithUTF8String:param1]];
  }
#elif PLATFORM == WIN
  if( isColour == false )
  {
    //SendMessage(lsci,property,param0,(LPARAM)param1);
    rc = fn(ptr,property,param0,(int)param1);
  }else
  {
    long colour = 0;
    int red = 0, green = 0, blue = 0;
    red = xtoi(param1+1,2);
    green = xtoi(param1+3,2);
    blue = xtoi(param1+5,2);

    colour = red | (green << 8) | (blue << 16);
    
    //SendMessage(lsci,property,param0,(LPARAM)colour);
    rc = fn(ptr,property,param0,colour);
  }
#elif PLATFORM == LIN
  if( isColour == false )
  {
    rc = scintilla_send_message(lsci,property,param0,(sptr_t)param1);
  }else
  {
    long colour = 0;
    int red = 0, green = 0, blue = 0;
    red = xtoi(param1+1,2);
    green = xtoi(param1+3,2);
    blue = xtoi(param1+5,2);

    colour = red | (green << 8) | (blue << 16);
   
    rc = scintilla_send_message(lsci,property,param0,colour);
  }
#else
  #error // no other platforms yet
#endif

  return rc;
}

// set lexer
long
ScintillaSend::set
(
  void* sci,
  int   property,
  char* param0,
  char* param1
)
{
  LOCAL_SCI(lsci,sci);
  long rc = 0;

#if PLATFORM == MAC
  rc = [lsci setLexerProperty: [NSString stringWithUTF8String:param0]
                   value: [NSString stringWithUTF8String:param1]];
#elif PLATFORM == WIN
  //SendMessage(lsci,property,(WPARAM)param0,(LPARAM)param1);
  rc = fn(ptr,property,(int)param0,(int)param1);
#elif PLATFORM == LIN
  rc = scintilla_send_message(lsci,property,(uptr_t)param0,(sptr_t)param1);
#else
  #error // no other platforms yet
#endif

  return rc;
}

int
ScintillaSend::get
(
  void* sci,
  int   property,
  int   value,
  int   extraV
)
{
  LOCAL_SCI(lsci,sci);

  int rc = 0;

#if PLATFORM == MAC
  if( value == -2 )
    rc = [lsci getGeneralProperty: property];
  else if( extraV == -2 )
    rc = [lsci getGeneralProperty: property parameter: value];
  else
    rc = [lsci getGeneralProperty: property parameter: value extra: extraV];
#elif PLATFORM == WIN
  //rc = SendMessage(lsci,property,value,(LPARAM)0);    
  rc = fn(ptr,property,value,extraV);
#elif PLATFORM == LIN
  rc = scintilla_send_message(lsci,property,value,extraV);
#else
  #error // no other platforms yet
#endif

  return rc;
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
