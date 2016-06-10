#include "MyEditorHandlerLin.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <unique/unique.h>

extern "C" {
#include "appStart.h"
}

UniqueApp* g_app = NULL;

#ifdef DEBUG
static char* EDITOR = "dbg.my.codeassistor";
#else
static char* EDITOR = "my.codeassistor";
#endif

static UniqueResponse
message_received_cb (UniqueApp         *app,
                     UniqueCommand      command,
                     UniqueMessageData *message,
                     guint              time_,
                     gpointer           user_data)
{
  UniqueResponse res = UNIQUE_RESPONSE_OK;
  MyEditor* top = NULL;
  MyTabWindow* mtw = NULL;

  switch(command)
  {
    case UNIQUE_NEW:
    {
      MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();
      top = meh->getTopEditor();
      if( top ) mtw = top->getTabWindow();
      meh->createEditor(mtw,NULL,0);
      //gtk_window_set_screen (GTK_WINDOW (ed->getWindow()), unique_message_data_get_screen (message));
      //gtk_window_present_with_time( GTK_WINDOW(ed->getWindow()), time_);
    }
    break;
    case UNIQUE_OPEN:
    {
      char* f = NULL;
      char* lineStr = NULL;
      int line = 0;
      MyEditor* me = NULL;
      MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();

      top = meh->getTopEditor();
      if( top ) mtw = top->getTabWindow();

      char* file = unique_message_data_get_text(message);
      char* atSym = strrchr(file,'@');
      if( atSym )
      {
        lineStr = atSym + 1;
        *atSym = '\0';
        line = atoi(lineStr);
      }

      f = fileFullPath(file);

      if( f )
        me = meh->findEditor(f);
      else
        return UNIQUE_RESPONSE_FAIL;

      if( me )
      {
        meh->setTopEditor(me);
//        gtk_window_set_screen (GTK_WINDOW (me->getWindow()), unique_message_data_get_screen (message));
//cd         gtk_window_present_with_time( GTK_WINDOW(me->getWindow()), time_);
      }
      else
      {
        me = meh->createEditor(mtw,file,line+20);
      }

      me->focus();
      
      me->gotoLine(line-1);

      if( f ) free(f);
      if( file ) free(file);
      
    }
    break;
    default:
    {
    }
    break;
      // do nothing
  }

  return res;
}

int start( int argc, char **argv )
{
  char file[PATH_MAX];
  char *f = NULL;
  char line[7];
  int lineToOpen = 0;

  memset(file,'\0',PATH_MAX);
  memset(line,'\0',7);

  gtk_init( &argc, &argv );

  //printf("arguments::\n");

  for( int i = 0; i < argc; i++ )
  {
    //printf(" argument %d is %s\n",i,argv[i]);fflush(stdout);
    if( i == 1 )
    {
      strncpy(file,argv[i],PATH_MAX);
      f = fileFullPath(file);
    }
    else if( i == 2 )
    {
      strncpy(line,argv[i],6);
      lineToOpen = atoi(line);
      if( lineToOpen < 0 ) lineToOpen = 0;
    }
  }

  g_app = unique_app_new(EDITOR,NULL);

  if( unique_app_is_running (g_app) )
  {
    char* fl = NULL;
    int len = 0;
    UniqueCommand cmd = UNIQUE_NEW;
    UniqueMessageData* msg = unique_message_data_new();

    if( f ) len += strlen(f);
    if( lineToOpen ) len += strlen(line) + 1;
    fl = (char*)malloc(len+1);
    
    if( f )
    {
      strcpy(fl,f);
      free(f);
      
      if( lineToOpen )
      {
        strcat(fl,"@");
        strcat(fl,line);
      }
    }

    if( len > 1 && fl )
    {      
      cmd = UNIQUE_OPEN;
      unique_message_data_set_text(msg,fl,len);
      free(fl);
    }
    
    unique_app_send_message(g_app,cmd,msg);
    unique_message_data_free(msg);
  }
  else
  {
    if(f) free(f);
    //g_appWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //unique_app_watch_window (app, GTK_WINDOW(g_appWin));
    g_signal_connect(g_app,"message-received",G_CALLBACK(message_received_cb),NULL);

    MyEditorHandlerLin* meh = (MyEditorHandlerLin*)MyEditorHandlerLin::getInstance();
    meh->createEditor(file,lineToOpen);

    gtk_main();

    //gtk_widget_destroy(g_appWin);
  }

  g_object_unref (g_app);

  return 0;
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
