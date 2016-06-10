#include "MyCommand.h"
#include "ScintillaController.h"
#include "ScintillaSend.h"
#include "common_defs.h"
#include "MyEditor.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define IS_REPLACE(x,len,term,esc) (3 == FindNumTerminators(x,len,term,esc)) \
      || ((2 == FindNumTerminators(x,len,term,esc)) && (x[0] == esc));

int
BuildWord(char* to, const char* from, int len, char terminate, char escape)
{
  int rc = -1;
 
  for( int i = 1; i < len; i++ )
  {
    if( from[i] == terminate && from[i-1] != escape )
    {
      rc = i;
      break;
    }
    if(to) to[i-1] = from[i];
  }
  return rc;
}

bool
IsGlobal(const char* cmd)
{
  bool rc = false;
  int toadd = 0, i = 0;
  char isG[strlen(cmd)];
  memset(isG,'\0',sizeof(isG));
  
  for( i = 0; i < 3; i++ )
    toadd += BuildWord(isG,cmd+toadd,strlen(cmd),'/','\\');
      
  if( isG[0] == 'g' )
    rc = true;

  return rc;   
}

int
FindNumTerminators(const char* from, int len, char term, char esc )
{
  int rc=0;
  for(int i=0;i<len;i++)
  {
    if( from[i] == term && from[i-1] != esc )
      rc++;
  }
  return rc;
}

void
MyCommand::CmdEntry( MyEditor* me, const char* cmd )
{
  int len = 0;
  ScintillaSend* send = ScintillaSend::getInstance();
  
  if( !cmd || (len = strlen(cmd)) < 2)
    return; //kick out because cmd is NULL or zero size
  
  if( (cmd[0] == '/') || (cmd[0] == '\\') )
  {
    // clean the selection
    send->set(EDITOR(me),SCI_SETSEL,-1,_startPos);
    
    if( len > 1 )
    {
      regex(cmd,len,me);
    }
  }
}

void
MyCommand::CmdExecute( MyEditor* me, const char* cmd )
{
  int len = 0;
  char* rplTxt = NULL;
  ScintillaSend* send = ScintillaSend::getInstance();
  int rloc = -1;
  
  _canHide = false;  
  
  if( !cmd || 0 == (len = strlen(cmd)))
    return; //kick out because cmd is NULL or zero size
    
  if( (cmd[0] == '/') || (cmd[0] == '\\') )
  {
    int isReplace = IS_REPLACE(cmd,len,'/','\\');
    bool isGlobal = IsGlobal(cmd);
    int lengthOfDoc = send->get(EDITOR(me),SCI_GETLENGTH);
    
    if( !isReplace ) _startPos = send->get(EDITOR(me),SCI_GETCURRENTPOS);
    
    if( isReplace && isGlobal )
      _startPos = 0;
    
    if( (len > 1) && regex(cmd,len,me) && ((rloc = BuildWord(NULL,cmd,len,'/','\\')) >= 0) )
    {
      cmd += rloc;

      if( cmd[1] != '\0' && isReplace)
      {
        int prevStartPos = -1;
        do
        {
          if( _startPos > prevStartPos )
          {
            if( _startSel >= 0 )
              send->set(EDITOR(me),SCI_SETTARGETSTART,_startSel,0);
            if( _endSel >= 0 )
              send->set(EDITOR(me),SCI_SETTARGETEND,_endSel,0);
          
            prevStartPos = _startPos;
            rplTxt = new char[strlen(cmd)+1];
            memset(rplTxt,'\0',strlen(cmd)+1);
            BuildWord(rplTxt,cmd,strlen(cmd),'/','\\');
            int more = send->set(EDITOR(me),SCI_REPLACETARGETRE,-1,(void*)rplTxt);
            
            if( cmd[0] == '/' )
              send->set(EDITOR(me),SCI_SETSEL,_startSel,_endSel);
            else
              send->set(EDITOR(me),SCI_SETSEL,_endSel,_startSel);
            delete [] rplTxt; rplTxt = NULL;
            _startPos = _startSel + more;
            
          }else break;
        }while(regex(cmd-rloc,len,me,!isGlobal) && _startPos < lengthOfDoc && isGlobal);
        if( isGlobal )
        {
          lengthOfDoc = send->get(EDITOR(me),SCI_GETLENGTH);
          send->set(EDITOR(me),SCI_SETSEL,-1,lengthOfDoc);
          send->set(EDITOR(me),SCI_SETCURRENTPOS,lengthOfDoc,0);
        }
      }
    }
  }
  _canHide = true;
}

bool
MyCommand::regex(const char* cmd, int len, MyEditor* me, bool loop)
{
  bool rv = false;
  ScintillaController* ctrl = ScintillaController::getInstance();
  ScintillaSend* send = ScintillaSend::getInstance();
  char re[len];
  memset(re,'\0',len);
 
  BuildWord(re,cmd,len,'/','\\');
 
  int end = send->get(EDITOR(me),SCI_GETLENGTH);
  FindDirection dir = cmd[0] == '/' ? DOWN : UP;
  if( strlen(re) > 0 )
    rv = ctrl->findText( EDITOR(me),re,dir,TRUE,_startPos,end,&_startSel,&_endSel,loop);
#if PLATFORM == WIN
  /* flickers sometimes, but the best of what I could find */
  ::InvalidateRect(me->getWindow(),NULL,false);
#endif
  return rv;
}
