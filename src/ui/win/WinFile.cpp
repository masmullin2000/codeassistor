#include "WinFile.h"
#include "MyEditor.h"
#include "MyEditorHandlerWin.h"
#include "ScintillaSend.h"

#include <stdio.h>

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
  HANDLE hFile;
  BOOL bSuccess = FALSE;

  hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
    OPEN_EXISTING, 0, NULL);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    DWORD dwFileSize;

    dwFileSize = GetFileSize(hFile, NULL);
    if(dwFileSize != 0xFFFFFFFF)
    {
      LPSTR pszFileText;

      pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
      if(pszFileText != NULL)
      {
        DWORD dwRead;

        if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
        {
          pszFileText[dwFileSize] = 0; // Add null terminator
          ScintillaSend* send = ScintillaSend::getInstance();
          send->set(hEdit, SCI_SETTEXT,0,pszFileText);
          bSuccess = TRUE; // It worked!
        }
        GlobalFree(pszFileText);
      }
    }
    CloseHandle(hFile);
  }
  return bSuccess;
}

bool DoFileOpen(char* fileName)
{
  OPENFILENAME ofn;
  bool rc = false;
  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = "All Files (*.*)\0*.*\0C Files (*.c)\0*.c\0C++ Files (*.cpp,*.cxx,*.C)\0*.cpp;*.cxx;*.C\0Header Files (*.h)\0*.h\0Text Files (*.txt)\0*.txt\0";
  ofn.lpstrFile = fileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = ".*";

  return GetOpenFileName(&ofn);
}

ERRSAVE SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
  HANDLE hFile;
  BOOL bSuccess = FALSE;

  hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    int length = 0;
    length = SendMessage(hEdit, SCI_GETLENGTH, 0, (LPARAM)0);
    // No need to bother if there's no text.
    if(length > 0)
    {
      LPSTR pszText;
      DWORD dwBufferSize = length + 1;

      pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);
      if(pszText != NULL)
      {
        SendMessage(hEdit, SCI_GETTEXT, length+1, (LPARAM)pszText );
        {
          DWORD dwWritten;

          if(WriteFile(hFile, pszText, length, &dwWritten, NULL))
            bSuccess = TRUE;
            
        }
        GlobalFree(pszText);
      }
    }
    else
    {
      DWORD dwWritten;

      if(WriteFile(hFile, "", 0, &dwWritten, NULL))
        bSuccess = TRUE;
    }
  }
  CloseHandle(hFile);
  return bSuccess ? ERRSAVE_NONE : ERRSAVE_PERMDENIED;
}

ERRSAVE DoFileSave(MyEditor* ed)
{
  ERRSAVE didSave = ERRSAVE_NONE;
  OPENFILENAME ofn;
  char szFileName[MAX_PATH] = "";
  
  if( !ed )
    return didSave;

  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = ed->getWindow();
  ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrDefExt = "txt";
  ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

  if(GetSaveFileName(&ofn))
  {
    HWND hEdit = ed->getEditor();
    didSave = SaveTextFileFromEdit(hEdit, szFileName);
    if( didSave == ERRSAVE_NONE )
    {
      MyEditorHandlerWin* meh = (MyEditorHandlerWin*)MyEditorHandlerWin::getInstance();
      ed->setFile(szFileName);
    }
  }else
  {
    didSave = ERRSAVE_CANCEL;
  }

  return didSave;
}

char* PasteFromClipboard( void )
{
  LPTSTR lptstr;
  HGLOBAL hglb;
  char* rc = NULL;

  if( !IsClipboardFormatAvailable(CF_TEXT) ||
      !OpenClipboard(NULL) )
  {
    printf("no");
    return rc;
  }

  hglb = GetClipboardData(CF_TEXT);
  if( hglb != NULL )
  {
    lptstr = (LPTSTR)GlobalLock(hglb);
    if( lptstr != NULL )
    {
      rc = new char[strlen(lptstr)+1];
      strcpy(rc,lptstr);
      rc[strlen(lptstr)] = '\0';
    }
  }

  CloseClipboard();

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
