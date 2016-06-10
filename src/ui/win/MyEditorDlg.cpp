#include <Shlwapi.h>
#include <stdio.h>
#include <commctrl.h>

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "MyEditor.h"
#include "MyEditorHandlerWin.h"
#include "MyEditorCommon.h"
#include "WinFile.h"
#include "MyEditorDlg.h"
#include "ScintillaSend.h"

#include "MyCommand.h"

using namespace Scintilla;

HWND g_finder = NULL;
HWND g_goto =   NULL;
HWND g_about =  NULL;

WNDPROC gEditProc = 0;
WNDPROC gTabProc = 0;
WNDPROC gSciProc = 0;

HCURSOR g_NormalCursor;
HCURSOR g_BookCursor;

bool HandleClose(int idx, MyTabWindow** mtw, MyEditorHandlerWin* meh);

BOOL CALLBACK AboutDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  switch(Message)
  {
    case WM_CLOSE:
      DestroyWindow(hwnd);
      EndDialog(hwnd, IDC_FIND_CANCELBUTTON);
      g_finder = NULL;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

BOOL CALLBACK FindDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  switch(Message)
  {
    case WM_INITDIALOG:
      return TRUE;

    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_COMMAND:
    {
      switch(LOWORD(wParam))
      {
        case ID_CMDED:
        {
          SendMessage(hwnd,WM_COMMAND,(WPARAM)IDC_GOTO_CANCELBUTTON,(LPARAM)0);
          SendMessage(hwnd,WM_COMMAND,(WPARAM)IDC_FIND_CANCELBUTTON,(LPARAM)0);
        }
        break;

        case IDC_FIND_FINDBUTTON:
        {
          int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_FIND_TEXT));
          if(len > 0)
          {
            int i;
            char* buf;
            MyEditor* ed = NULL;
            MyEditorHandler* meh = MyEditorHandlerWin::getInstance();

            buf = (char*)GlobalAlloc(GPTR, len + 1);
            GetDlgItemText(hwnd, IDC_FIND_TEXT, buf, len + 1);

            ed = meh->getTopEditor();
            if( ed )
            {
              ed->find(DOWN,buf);
            }

            GlobalFree((HANDLE)buf);
          }
          break;
        }
        case IDC_GOTO_FINDBUTTON:
        {
          int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_GOTO_TEXT));
          if(len > 0)
          {
            int i;
            int line = 0;
            char* buf;
            MyEditor* ed = NULL;
            MyEditorHandler* meh = MyEditorHandlerWin::getInstance();

            buf = (char*)GlobalAlloc(GPTR, len + 1);
            GetDlgItemText(hwnd, IDC_GOTO_TEXT, buf, len + 1);

            ed = meh->getTopEditor();

            line = atoi(buf);

            if( ed )
            {
              ed->gotoLine(line-1);
              ed->focus();
            }

            GlobalFree((HANDLE)buf);
          }
        }
        //break; intentionally flow

        case IDC_GOTO_CANCELBUTTON:
        {
          EndDialog(hwnd, IDC_GOTO_CANCELBUTTON);
          g_goto = NULL;
        }
        break;
        case IDC_FIND_CANCELBUTTON:
        {
          EndDialog(hwnd, IDC_FIND_CANCELBUTTON);
          MyEditor* ed = NULL;
          MyEditorHandler* meh = MyEditorHandlerWin::getInstance();

          ed = meh->getTopEditor();
          ed->focus();

          g_finder = NULL;
          break;
        }
        case ID_EDIT_PASTE:
        {
          char* clipboard = PasteFromClipboard();
          if( clipboard )
          {
            int len = strlen(clipboard);
            SetDlgItemText(hwnd, IDC_FIND_TEXT, clipboard);
            HWND edit = GetDlgItem(hwnd,IDC_FIND_TEXT);
            SendMessage(edit,EM_SETSEL,len,-1);

            delete [] clipboard;
          }
          break;
        }
      }
    }
    break;
    default:
      return FALSE;
  }
  return TRUE;
}

BOOL CALLBACK ReloadDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_INITDIALOG:
      return TRUE;
    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case IDOK:
          EndDialog(hwnd, IDOK);
          break;
        case IDCANCEL:
          EndDialog(hwnd, IDCANCEL);
          break;
      }
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

LRESULT CALLBACK CmdProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  MyEditorHandlerWin* meh = (MyEditorHandlerWin*)MyEditorHandlerWin::getInstance();
  MyTabWindow* mtw = meh->findTabWindow(::GetParent(::GetParent(hwnd)));
  MyEditor* ed = NULL;
  if( mtw )
    ed = mtw->getTopEditor();

  if( ed ) // no point doing any work if we didn't find Ed.
  {
    switch(msg)
    {
      case WM_KEYDOWN:
      case WM_CHAR:
      {
        if( wParam == VK_RETURN || wParam == VK_TAB )
          return 0;
      }
      break;
      case WM_KEYUP:
      {
        static char lastStr[256];
        HWND cmdEd = ed->getCmdEd();
        int len = SendMessage(cmdEd,WM_GETTEXTLENGTH,0,0);

        if(len > 0)
        {
          int i;
          char* buf;

          buf = (char*)GlobalAlloc(GPTR, len + 1);
          ::SendMessage(cmdEd,WM_GETTEXT,len+1,(LPARAM)buf);

          if( strncmp(lastStr,buf,256) || LOWORD(wParam) == VK_RETURN )
          {
            strncpy(lastStr,buf,256);

            switch(LOWORD(wParam))
            {
              case VK_RETURN:
                ed->_cmd.CmdExecute(ed,buf);
                break;
              default:
                ed->_cmd.CmdEntry(ed,buf);
            }
          }
          GlobalFree((HANDLE)buf);
        }
      }
      break;
      case WM_COMMAND:
      {
      }
      break;
    }
  }
  if( gEditProc ) return CallWindowProc(gEditProc,hwnd,msg,wParam,lParam);
  else return DefWindowProc(hwnd,msg,wParam,lParam);
}

LRESULT CALLBACK TabProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  MyEditorHandlerWin* meh = (MyEditorHandlerWin*)MyEditorHandlerWin::getInstance();
  MyTabWindow* mtw = meh->findTabWindow(::GetParent(hwnd));
  static BOOL isInside = TRUE;

  switch(msg)
  {
    case WM_MOUSEMOVE:
    {
      if( mtw && mtw->isDragging() && (wParam & MK_LBUTTON)  )
      {
        POINT point;
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);

        if( mtw->getEditorIdxAtTabPoint(&point) != -1 )
        {
          mtw->doTabSwitch(mtw->getTopEditorIdx(),&point);
          isInside = TRUE;
        }
        else
        {
          isInside = FALSE;
        }
      }
      else if( mtw )
      {
        mtw->setDragging(false);
      }
    }
    break;
    case WM_LBUTTONUP:
    {
      mtw->setDragging(false);
      ::SetCursor(g_NormalCursor);

      if( !isInside )
      {
        MyEditor* ed = NULL;
        if( mtw )
        {
          POINT point;
          ::GetCursorPos(&point);

          ed = mtw->getTopEditor();
          HWND toDel = ed->getWindow();

          HWND wfp = ::WindowFromPoint(point);

          while( wfp && (WS_CHILD & GetWindowLongPtr(wfp,GWL_STYLE)) )
          {
            wfp = ::GetParent(wfp);
          }
          MyTabWindow* hit = meh->findTabWindow(wfp);
          meh->ripEditor(ed,hit);

          if( !hit )
          {
            RECT sizer;
            ::GetWindowRect(toDel,&sizer);
            int width = sizer.right - sizer.left;
            int height = sizer.bottom - sizer.top;
            MyEditor* newEd = meh->getTopEditor();
            ::MoveWindow(newEd->getWindow(),point.x,point.y,width,height,TRUE);
          }
          ed->showWindow();

          if( mtw->isEmpty() )
          {
            meh->removeTabWindow(mtw);
            ::DestroyWindow(toDel);
            return TRUE;
          }
        }
        //isInside = TRUE;
      }
      ::ReleaseCapture();
      return TRUE;
    }
    break;
    case WM_COMMAND:
    {
      switch(HIWORD(wParam))
      {
        case SCEN_SETFOCUS:
        {
          ::SendMessage(::GetParent(hwnd),msg,wParam,lParam);
        }
        break;
      }
    }
    break;
    case WM_LBUTTONDOWN:
    {
      POINT point;
      point.x = LOWORD(lParam);
      point.y = HIWORD(lParam);
      if( ::DragDetect(hwnd,point) )
      {
        mtw->setDragging(true);
        ::SetCapture(hwnd);
        isInside = TRUE;
        ::SetCursor(g_BookCursor);

      }
    }
    //break;
    default:
    {
      //kludge, get item 0x1305 seems to cause a crash here
      if( msg == 0x1305 && !isInside )
        return TRUE;
      if( gTabProc ) return CallWindowProc(gTabProc,hwnd,msg,wParam,lParam);
      else return DefWindowProc(hwnd,msg,wParam,lParam);
    }
  }
  //MessageBox(NULL,"gonnacrash","ohno",NULL);
  return TRUE;
}

LRESULT CALLBACK SciProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_MOUSELEAVE:
    case WM_MOUSEMOVE:
    {
      ::SendMessage(::GetParent(::GetParent(hwnd)),msg,wParam,lParam);
    }
    break;
  }

  if( gSciProc ) return CallWindowProc(gSciProc,hwnd,msg,wParam,lParam);
  else return DefWindowProc(hwnd,msg,wParam,lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  MyEditorHandlerWin* meh = (MyEditorHandlerWin*)MyEditorHandlerWin::getInstance();
  MyTabWindow* mtw = meh->findTabWindow(hwnd);
  MyEditor* ed = NULL;
  static BOOL track = FALSE;
  if( mtw )
    ed = mtw->getTopEditor();

  static HWND findTE = NULL;

  switch(msg)
  {
    case WM_MOUSEMOVE:
    {
      if(!track)
      {
        POINT point;
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        track = TRUE;
        ::TrackMouseEvent(NULL);
      }
    }
    break;
    case WM_MOUSELEAVE:
    {
      track = FALSE;
    }
    case WM_CREATE:
    {
      break;
    }
    case WM_ACTIVATE:
    {
      switch(wParam)
      {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
          if( ed ) SetFocus(ed->getEditor());
          break;
      }
      break;
    }
    case WM_SIZE:
    {
      HWND hEdit;
      RECT rct;

      ::GetClientRect(hwnd, &rct);

      if( mtw )
      {
        HWND tabsz = mtw->getTabCtrl();
        ::SetWindowPos(tabsz, NULL, 0, 0, rct.right, rct.bottom, SWP_NOZORDER);
      }

      if( ed )
      {
        hEdit = ed->getEditor();
        if( ed && ed->isCmdEdShown() )
        {
          ::MoveWindow(ed->getCmdEd(),0,rct.bottom-20,rct.right-rct.left,20,FALSE);
          SetWindowPos(hEdit, NULL, 0, 20, rct.right, rct.bottom-40, SWP_NOZORDER);
        }
        else
        {
          SetWindowPos(hEdit, NULL, 0, 20, rct.right, rct.bottom-20, SWP_NOZORDER);
        }
      }
    }
    break;
    case WM_CLOSE:
    {
      if( lParam )
      {
        HandleClose(wParam,&mtw,meh);
        if( mtw && !mtw->isEmpty() )
        {
          ::SendMessage(mtw->getTopEditor()->getWindow(),WM_SIZE,0,0);
        }
      }
      else // close the whole window
      {
        int i;
        for( i = 0; mtw && i < mtw->editorCount(); i++ )
        {
          mtw->setTopEditor(i);
          if( HandleClose(i,&mtw,meh) )
            i--;
        }
        if( mtw && !mtw->isEmpty() )
        {
          mtw->setTopEditor(mtw->editorCount()-1);
        }
      }
    }
    break;
    case WM_DESTROY:
    {
      //meh->removeEditor(ed);
      if( meh->isEmpty() )
        PostQuitMessage(0);
    }
    break;
    case WM_COMMAND:
    {
      switch(LOWORD(wParam))
      {
        case ID_FILE_NEW:
        {
          meh->createEditor(NULL,NULL,0);
        }
        break;

        case ID_TAB_NEW:
        {
          if( mtw )
          {
            mtw->hideAllEditors();
          }

          meh->createEditor(mtw,NULL,0);
        }
        break;
        case ID_FILE_OPEN_WIN:
        case ID_FILE_OPEN:
        {
          ScintillaSend* send = ScintillaSend::getInstance();
          MyEditor* usedEd;
          char fileName[MAX_PATH] = "";

          if( DoFileOpen(fileName))
          {
            if( GetFileSize(fileName) > MAX_FILE_SIZE )
            {
              MessageBox(hwnd,"File is too large, Maximum file size is 100MB","File Open Error",MB_OK|MB_ICONERROR);
              break;
            }
            if( ed->isDirty() || 0 != send->get(ed->getEditor(), SCI_GETTEXTLENGTH,0,0) )
            {
              if( ID_FILE_OPEN == LOWORD(wParam) )
              {
                if( mtw )
                {
                  mtw->hideAllEditors();
                }

                usedEd = meh->createEditor(mtw,NULL,0);
              }
              else
              {
                usedEd = meh->createEditor(NULL,NULL,0);
              }
            }
            else
            {
              usedEd = ed;
            }
            LoadTextFileToEdit(usedEd->getEditor(),fileName);

            usedEd->setFile(fileName);
            send->set( usedEd->getEditor(), SCI_EMPTYUNDOBUFFER, 0, 0 );
            usedEd->setDirtyOverride(false);
            ::SetFocus(usedEd->getEditor());
          }
        }
        break;

        case ID_FILE_SAVE:
        {
          ed->save();
        }
        break;
        case ID_FILE_SAVE_AS:
        {
          ed->save(true);
        }
        break;

        case ID_FILE_CLOSE:
        {
          PostMessage(hwnd,WM_CLOSE,-1,-1);
        }
        break;

        case ID_FILE_EXIT:
        {
          for( int i = 0; i < meh->editorCount(); i++ )
          {
            MyEditor* close = meh->getEditor(i);
            int idx = mtw->getEditorIdx(close);
            MyTabWindow* tabwin = (MyTabWindow*)close->getTabWindow();
            if( HandleClose(idx,&tabwin, meh) )
              i--;
          }
          break;
        }
        case ID_EDIT_UNDO:
        {
          ed->undo();
          break;
        }
        case ID_EDIT_REDO:
        {
          ed->redo();
          break;
        }
        case ID_EDIT_CUT:
        {
          ed->cut();
          break;
        }
        case ID_EDIT_COPY:
        {
          ed->copy();
          break;
        }
        case ID_EDIT_PASTE:
        {
          ed->paste();
          break;
        }
        case ID_FIND:
        {
          meh->setTopEditor(ed);
          if( g_finder == NULL )
          {
            g_finder = CreateDialog(GetModuleHandle(NULL),
              MAKEINTRESOURCE(IDD_FIND_DIALOG), NULL, FindDialogProc);
            if( g_finder != NULL)
             ShowWindow(g_finder,SW_SHOW);
          }else
          {
            SetFocus(g_finder);
          }
          break;
        }
        case ID_FIND_NEXT:
        {
          ed->find(DOWN);
          break;
        }
        case ID_FIND_PREV:
        {
          ed->find(UP);
          break;
        }
        case ID_GOTO:
        {
          meh->setTopEditor(ed);
          if( g_goto == NULL )
          {
            g_goto = CreateDialog(GetModuleHandle(NULL),
              MAKEINTRESOURCE(IDD_GOTO_DIALOG), NULL, FindDialogProc);
            if( g_goto != NULL)
             ShowWindow(g_goto,SW_SHOW);
          }else
          {
            SetFocus(g_goto);
          }
          break;
        }
        case ID_CMDED:
        {
          if( !ed->isCmdEdShown() )
            ed->showCmdEd();
          else
            ed->focus();
        }
        break;
        case ID_VIEW_WRAP:
        {
          ed->toggleWrap();
        }
        break;
        case ID_VIEW_FOLDALL:
        {
          ed->foldAll();
        }
        break;
        case ID_VIEW_UNFOLDALL:
        {
          ed->unfoldAll();
        }
        break;
        case ID_VIEW_SHOWEOL:
        {
          ed->toggleShowLineEndings();
        }
        break;
        case ID_VIEW_WIN_STYLE:
        {
          ed->setLineEndings(SC_EOL_CRLF);
        }
        break;
        case ID_VIEW_MAC_STYLE:
        {
          ed->setLineEndings(SC_EOL_CR);
        }
        break;
        case ID_VIEW_LIN_STYLE:
        {
          ed->setLineEndings(SC_EOL_LF);
        }
        break;
        case ID_ABOUT:
        {
          if( g_about == NULL )
          {
            g_about = CreateDialog(GetModuleHandle(NULL),
              MAKEINTRESOURCE(IDD_ABOUT), NULL, AboutDialogProc);
            if( g_about != NULL)
             ShowWindow(g_about,SW_SHOW);
          }else
          {
            SetFocus(g_about);
          }
          break;
        }
      }

      switch( HIWORD(wParam) )
      {
        case SCEN_SETFOCUS:
        {
          ScintillaSend* send = ScintillaSend::getInstance();
          if( ed )
          {
            static bool isShowing = false; //kludgy mc kludge!
            if(ed->isCmdEdShown() )
              ed->hideCmdEd();
            meh->setTopEditor(ed);
            mtw->resetPrevTabIdx();
            if( ed->checkUpdate() && !isShowing)
            {
              isShowing = true;
              int ret = DialogBox(GetModuleHandle(NULL),
                MAKEINTRESOURCE(IDD_RELOAD), hwnd, ReloadDlgProc);

              switch( ret )
              {
                case IDOK:
                {
                  int firstline = send->get( ed->getEditor(), SCI_GETFIRSTVISIBLELINE);
                  int pos = send->get( ed->getEditor(), SCI_GETCURRENTPOS);
                  int line = send->get( ed->getEditor(), SCI_LINEFROMPOSITION,pos);
                  LoadTextFileToEdit(ed->getEditor(),ed->getFile());
                  ed->setTouch(GetTime(ed->getFile()),GetFileSize(ed->getFile()));
                  ed->setDirtyOverride(false);
                  send->set( ed->getEditor(), SCI_SETSAVEPOINT, 0, 0 );
                  send->set( ed->getEditor(), SCI_GOTOLINE,line,0);
                  send->set( ed->getEditor(), SCI_SETFIRSTVISIBLELINE,firstline,0);
                }
                break;

                case IDCANCEL:
                {
                  ed->setTouch(GetTime(ed->getFile()),GetFileSize(ed->getFile()));
                  ed->setDirtyOverride(true);
                }
                break;
              }
            }
            isShowing = false;
          }
        }
        break;

        case SCEN_KILLFOCUS:
        {

        }
        break;
      }
    }
    break;

    case WM_NOTIFY:
    {
      SCNotification* scn = reinterpret_cast<SCNotification*>(lParam);
      if( ed ) HandleNotification(ed,scn);

      switch( ((LPNMHDR)lParam)->code )
      {
        case TCN_SELCHANGING:
        {
          int idx = TabCtrl_GetCurSel(mtw->getTabCtrl());
          mtw->display(idx,false);
        }
        break;
        case TCN_SELCHANGE:
        {
          int idx = TabCtrl_GetCurSel(mtw->getTabCtrl());
          mtw->display(idx,true);
          ::SendMessage(mtw->getHWND(),WM_SIZE,0,0);
        }
        break;
      }

    }
    break;

    case WM_COPYDATA:
    {
      COPYDATASTRUCT *cds = (COPYDATASTRUCT*)lParam;
      if( cds->dwData == WM_EXTERNAL_OPEN )
      {
        MessageToExternal mte;
        MessageToExternal* c_mte = (MessageToExternal*)cds->lpData;
        strncpy(mte.fileName,c_mte->fileName,MAX_PATH);
        mte.lineNum = c_mte->lineNum;

        char fullFile[MAX_PATH];
        int retval = GetFullPathName(mte.fileName,MAX_PATH,fullFile,NULL);

        MyEditor* found = NULL;
        if( retval ) found = meh->findEditor(fullFile);
        if( found )
        {
          found->focus();
          if( mte.lineNum > 0 )
            found->gotoLine(mte.lineNum);
          meh->setTopEditor(found);
          mtw->setTopEditor(found);
          ::SendMessage(found->getWindow(),WM_SIZE,0,0);

        }
        else
        {
          if( mtw )
          {
            mtw->hideAllEditors();
          }
          meh->createEditor(mtw,fullFile);
        }
      }
    }
    break;
    case WM_DROPFILES:
    {
      char fileName[MAX_PATH];
      int amtFiles = ::DragQueryFile((HDROP)wParam,0xFFFFFFFF,NULL,0);
      for( int i = 0; i < amtFiles; i++ )
      {
        memset(fileName,'\0',MAX_PATH);
        ::DragQueryFile((HDROP)wParam,i,fileName,MAX_PATH);

        char fullFile[MAX_PATH];
        int retval = GetFullPathName(fileName,MAX_PATH,fullFile,NULL);

        MyEditor* found = NULL;
        if( retval ) found = meh->findEditor(fullFile);
        if( found )
        {
          found->focus();
          meh->setTopEditor(found);
          mtw->setTopEditor(found);
          ::SendMessage(found->getWindow(),WM_SIZE,0,0);

          break;
        }
        else
        {
          if( mtw )
          {
            mtw->hideAllEditors();
          }
          meh->createEditor(mtw,fullFile);
        }
      }
    }
    break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

bool
HandleClose(int idx, MyTabWindow** mtw, MyEditorHandlerWin* meh)
{
  MyEditor* ed = NULL;
  bool rc = true;
  HWND hwnd;


  int id;
  if( idx >= 0 )
  {
    (*mtw)->setTopEditor(idx);
    ed = (*mtw)->getEditor(idx);
    ::SendMessage(ed->getWindow(),WM_SIZE,0,0);
  }
  else
    ed = (*mtw)->getTopEditor();

  hwnd = ed->getWindow();

  if( ed->isDirty() )
  {
    char* file = NULL;
    if( ed->getFile() )
    {
      file = new char[strlen(ed->getFile())+14];
      sprintf(file,"Save File \"%s\"?",ed->getFile());
    }
    else
    {
      file = new char[15];
      sprintf(file,"Save Untitled?");
    }

    id = MessageBox(ed->getWindow(),file,"Save",MB_YESNOCANCEL);
    if( id == IDYES )
    {
      if( ed->save() )
      {
        ed->close();
        meh->removeEditor(ed);

        if( (*mtw)->isEmpty() )
        {
          meh->removeTabWindow((*mtw));
          DestroyWindow(hwnd);
        }
        else
          (*mtw)->display((*mtw)->getTopEditorIdx(),true);
      }
    }
    else if( id == IDNO )
    {
      ed->close();
      meh->removeEditor(ed);
      if( (*mtw)->isEmpty() )
      {
        meh->removeTabWindow((*mtw));
        DestroyWindow(hwnd);
        (*mtw) = NULL;
      }
      else
        (*mtw)->display((*mtw)->getTopEditorIdx(),true);
    }
    else
      rc = false;
  }
  else
  {
    ed->close();
    meh->removeEditor(ed);

    if( (*mtw)->isEmpty() )
    {
      meh->removeTabWindow((*mtw));
      DestroyWindow(hwnd);
      (*mtw) = NULL;
    }
    else
    {
      (*mtw)->display((*mtw)->getTopEditorIdx(),true);
    }
  }
  return rc;
}


/*
Copyright (c) 2010-2011, Michael Mullin <masmullin@gmail.com>
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
