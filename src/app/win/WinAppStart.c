#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "MyEditor.h"
#include "MyEditorHandlerWin.h"
#include "resource.h"

extern HWND g_finder;
extern HWND g_goto;
extern HWND g_about;
extern HIMAGELIST g_ImageList;

  
extern HCURSOR g_NormalCursor;
extern HCURSOR g_BookCursor;

extern "C" {
#include "appStart.h"
}

void SendToExistingApp
(
  HWND pApp,
  char* file,
  int line,
  HINSTANCE hInst
)
{
  COPYDATASTRUCT cds;
  MessageToExternal mte;

  strncpy(mte.fileName,file,MAX_PATH);
  mte.lineNum = line;

  cds.dwData = WM_EXTERNAL_OPEN;
  cds.lpData = (void*)&mte;
  cds.cbData = (long)(sizeof(mte));

  int show;

  if (::IsIconic(pApp))
    show = SW_RESTORE;
  else
    show = SW_SHOW;

  ::ShowWindow(pApp, show);
  ::SetForegroundWindow(pApp);

  ::SendMessage(pApp,WM_COPYDATA,(WPARAM)hInst,(LPARAM)&cds);

}

int start(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG Msg;
	HMODULE hmod;
  LPWSTR* argv = NULL;
  int argc;
  char file[MAX_PATH];
  int lineToOpen = 0;
  
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_TAB_CLASSES;
  InitCommonControlsEx(&icex);

  memset(file,'\0',MAX_PATH);

  argv = CommandLineToArgvW( GetCommandLineW(), &argc );
  for( int i = 0; i < argc; i++ )
  {
    char* argument = new char[wcslen(argv[i])+1];
    wcstombs( argument, argv[i], wcslen(argv[i]) );
    argument[wcslen(argv[i])] = '\0';
    
    if( i == 1 )
    {
      strncpy(file,argument,wcslen(argv[i]));
    }
    else if( i == 2 )
    {
      char line[7] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
      strncpy(line,argument,6);
      lineToOpen = atoi(line) - 1;
      if( lineToOpen < 0 ) lineToOpen = 0;
    }
      
    delete [] argument;
  }
  LocalFree(argv);

  /* check to see if a previous instance of CodeAssistor is running */
  HWND prevApp = FindWindow(g_szClassName,NULL);

  if( prevApp )
  {
    char fullFile[MAX_PATH];
    int retval = GetFullPathName(file,MAX_PATH,fullFile,NULL);
    SendToExistingApp(prevApp,fullFile,lineToOpen,hInstance);
    return FALSE;
  }
  /* end previous instance code if we pass this we are a new program */
  
  g_NormalCursor = (HCURSOR)LoadCursor(NULL,IDC_ARROW);
  
  HICON img = (HICON)LoadImage( GetModuleHandle(NULL),
                                MAKEINTRESOURCE(IDI_BOOK_ICON),
                                IMAGE_ICON,
                                16,
                                16,
                                0);
                                
  ICONINFO ii;
  ICONINFO ci;
  ::GetIconInfo(img,&ii);
  ci = ii;
  ci.fIcon = FALSE;
  
  g_BookCursor = ::CreateIconIndirect(&ci);
  
  ::DeleteObject(ii.hbmMask);
  ::DeleteObject(ii.hbmColor);
    
  g_ImageList = ImageList_Create(16,16,ILC_COLOR32|ILC_MASK,0,0);
  
  ImageList_AddIcon(g_ImageList,img);

  MyEditorHandlerWin* meh = (MyEditorHandlerWin*)MyEditorHandlerWin::getInstance();
  meh->windowsSettings(hInstance,nCmdShow);
  
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BOOK_ICON), IMAGE_ICON, 128, 128, 0);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = ImageList_GetIcon(g_ImageList,0,0);//(HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BOOK_ICON), IMAGE_ICON, 16, 16, 0);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

#ifdef STATIC_BUILD
  Scintilla_LinkLexers();
  Scintilla_RegisterClasses(hInstance);
#else	
	hmod = LoadLibrary("SciLexer.DLL");
	if (hmod==NULL)
	{
		MessageBox(NULL,
		"The Scintilla DLL could not be loaded.",
		"Error loading Scintilla",
		MB_OK | MB_ICONERROR);
		return 1;
	}
#endif
  meh->createEditor(NULL,file,lineToOpen);
  
  HACCEL fileA = LoadAccelerators(hInstance,"FileAccel");
  HACCEL editA = LoadAccelerators(hInstance,"EditAccel");
  HACCEL searchA = LoadAccelerators(hInstance,"SearchAccel");
  HACCEL viewA = LoadAccelerators(hInstance,"ViewAccel");
	
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
    if(!(TranslateAccelerator(GetActiveWindow(),fileA,&Msg)   ||
         TranslateAccelerator(GetActiveWindow(),editA,&Msg)   ||
         TranslateAccelerator(GetActiveWindow(),searchA,&Msg) ||
         TranslateAccelerator(GetActiveWindow(),viewA,&Msg))
    )
    {
      if( !IsDialogMessage(g_finder,&Msg) && 
          !IsDialogMessage(g_goto,&Msg)   &&
          !IsDialogMessage(g_about,&Msg) )
      {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
      }
    }
	}

#ifdef STATIC_BUILD
  Scintilla_ReleaseResources();
#else
  FreeLibrary(hmod);
#endif
	return Msg.wParam;
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
