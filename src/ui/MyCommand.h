#ifndef __MY_COMMAND_H__
#define __MY_COMMAND_H__

#include <stdio.h>
#include "common_defs.h"

#if PLATFORM == MAC
@class MyEditor;
#else
class MyEditor;
#endif

class MyCommand
{
private:
  int  _startPos;
  bool _canHide;
  
  int _startSel,
      _endSel;

public:

  MyCommand():
    _startPos(0), _canHide(false), _startSel(-1), _endSel(-1)
    {;}

  void CmdEntry
  (
    MyEditor* me,
    const char* cmd
  );

  void CmdExecute
  (
    MyEditor* me,
    const char* cmd
  );
  
  void setStartPos
  ( int start )
  { _startPos = start; }
  
  bool regex( const char* cmd, int len, MyEditor* me, bool loop = true );
};

#endif
