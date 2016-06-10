#include "WinFile.h"
#include "resource.h"

#include "MyTabWindow.h"
#include "MyEditorDlg.h"
#include "MyEditor.h"
#include "MyEditorHandlerWin.h"

extern WNDPROC gTabProc;

HIMAGELIST g_ImageList;

using namespace std;

MyTabWindow::MyTabWindow(HINSTANCE app)
{
  _app = app;
  _isDragging = false;
  _tab = NULL;
  resetPrevTabIdx();
  createWindow();
  createMenu();
  createTabCtrl();
}

int
MyTabWindow::createWindow()
{
  _window = CreateWindowEx( 0,
             g_szClassName,
             "The CodeAssistor Editor",
             WS_OVERLAPPEDWINDOW,
             CW_USEDEFAULT,
             CW_USEDEFAULT,
             720,
             700,
             NULL,
             NULL,
             _app,
             NULL );

  if( _window == NULL )
  {
    MessageBox(NULL, "Window Creation Failed!", "Error!",
      MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  ::DragAcceptFiles(_window,true);

  return 1;
}

int
MyTabWindow::createTabCtrl()
{
  if( !_window )
    return 0;

  _tab = CreateWindowEx( 0,
          WC_TABCONTROL,
          "zzzzz",
          WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE |
            TCS_FOCUSNEVER | TCS_TABS,
          CW_USEDEFAULT,
          CW_USEDEFAULT,
          720,
          700,
          _window,
          NULL,
          _app,
          NULL );

  if( !_tab )
  {
    MessageBox(NULL, "Tab Creation Failed!", "Error!",
      MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  ::SendMessage(_tab, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
  TabCtrl_SetImageList(_tab,g_ImageList);

  gTabProc = (WNDPROC)GetWindowLongPtr(_tab,GWLP_WNDPROC);
  SetWindowLongPtr( _tab,GWLP_WNDPROC,(LONG_PTR)TabProc );

  return 1;
}

int
MyTabWindow::createTab(MyEditor* ed)
{
  TCITEM tab;

  tab.mask = TCIF_TEXT | TCIF_IMAGE;
  tab.iImage = 0;
  tab.pszText = "Untitled";

  int idx = TabCtrl_GetItemCount(_tab);
  TabCtrl_InsertItem(_tab,idx,&tab);
  TabCtrl_SetCurSel(_tab,idx);
  MyEditorHandler* meh = MyEditorHandlerWin::getInstance();
  meh->setTopEditor(ed);

  MyEditorIndexer* mei = new MyEditorIndexer();
  mei->_editor = ed;
  mei->_idx = idx;

  _editorList.push_back(mei);

  return 1;
}

int
MyTabWindow::createMenu()
{
  HMENU file, edit, find, view, help, lineEnd;
  MENUITEMINFO menu_info;

  _menu = CreateMenu();

  file = CreatePopupMenu();
  AppendMenu(file,MF_STRING, ID_FILE_NEW, "&New...\tCtrl+N");
  AppendMenu(file,MF_STRING, ID_TAB_NEW, "New &Tab...\tCtrl+T");
  AppendMenu(file,MF_STRING, ID_FILE_OPEN, "&Open...\tCtrl+O");
  AppendMenu(file,MF_STRING, ID_FILE_OPEN_WIN, "O&pen Window...\tCtrl+Shift+O");
  AppendMenu(file,MF_STRING, ID_FILE_SAVE, "&Save...\tCtrl+S");
  AppendMenu(file,MF_STRING, ID_FILE_SAVE_AS, "S&ave As...\tCtrl+Alt+S");
  AppendMenu(file,MF_STRING, ID_FILE_CLOSE, "&Close...\tCtrl+W");
  AppendMenu(file,MF_STRING, ID_FILE_EXIT, "E&xit...\tCtrl+Q");
  AppendMenu(_menu,MF_STRING|MF_POPUP, (UINT)file, "&File");

  edit = CreatePopupMenu();

  AppendMenu(edit,MF_STRING, ID_EDIT_UNDO, "&Undo...\tCtrl+Z");
  AppendMenu(edit,MF_STRING, ID_EDIT_REDO, "&Redo...\tCtrl+Y");
  AppendMenu(edit,MF_STRING, ID_EDIT_CUT,  "C&ut...\tCtrl+X");
  AppendMenu(edit,MF_STRING, ID_EDIT_COPY, "&Copy...\tCtrl+C");
  AppendMenu(edit,MF_STRING, ID_EDIT_PASTE, "&Paste...\tCtrl+V");
  AppendMenu(edit,MF_STRING, ID_CMDED, "&Execute...\tESC");


  AppendMenu(_menu,MF_STRING|MF_POPUP,(UINT)edit,"&Edit");

  find = CreatePopupMenu();

  AppendMenu(find,MF_STRING, ID_FIND, "&Find...\tCtrl+F");
  AppendMenu(find,MF_STRING, ID_FIND_PREV, "Find &Previous...\tF3");
  AppendMenu(find,MF_STRING, ID_FIND_NEXT, "Find &Next...\tF4");
  AppendMenu(find,MF_STRING, ID_GOTO, "&Goto...\tCtrl+G");

  AppendMenu(_menu,MF_STRING|MF_POPUP,(UINT)find,"&Search");

  view = CreatePopupMenu();

  AppendMenu(view,MF_STRING, ID_VIEW_WRAP, "&Wrap...\tAlt+W");
  AppendMenu(view,MF_STRING, ID_VIEW_FOLDALL, "&Fold All...\tAlt+ +");
  AppendMenu(view,MF_STRING, ID_VIEW_UNFOLDALL, "&Unfold All...\tAlt+ -");
  
  lineEnd = CreatePopupMenu();
  
  AppendMenu(lineEnd,MF_STRING, ID_VIEW_SHOWEOL, "Show/Hide Line &Endings...\tCtrl+Alt+ L");
  AppendMenu(lineEnd,MF_STRING, ID_VIEW_WIN_STYLE, "&Windows Line Endings");
  AppendMenu(lineEnd,MF_STRING, ID_VIEW_MAC_STYLE, "Old &Macintosh Line Endings");
  AppendMenu(lineEnd,MF_STRING, ID_VIEW_LIN_STYLE, "&Linux Line Endings");
  AppendMenu(view,MF_STRING|MF_POPUP,(UINT)lineEnd,"&Line Endings");
  AppendMenu(_menu,MF_STRING|MF_POPUP,(UINT)view,"&View");

  help = CreatePopupMenu();

  AppendMenu(help,MF_STRING, ID_ABOUT, "&About");
  AppendMenu(_menu,MF_STRING|MF_POPUP,(UINT)help,"&Help");

  AppendMenu(_menu,MF_STRING|MFT_RIGHTJUSTIFY,ID_FILE_CLOSE,"X");

  SetMenu(_window,_menu);

  return 1;
}

bool
MyTabWindow::isOwner(HWND win)
{
  return _window == win;
}

bool
MyTabWindow::isEmpty()
{
  bool rc = true;

  if( _editorList.size() > 0 )
    rc = false;

  return rc;
}

void
MyTabWindow::hideAllEditors(void)
{
  for( int i = 0; i < _editorList.size(); i++ )
    display(i,false);
}

void
MyTabWindow::display(int idx, boolean show)
{
  // this is wrong, need to search for editorList->MyEditorIndex->_idx
  MyEditor* ed = getEditorIndexerAtIdx(idx)->_editor;
  
  if(!ed) return;

  ::ShowWindow(ed->getEditor(),show ? SW_SHOW : SW_HIDE);
  if( show )
  {
    ::SetFocus(ed->getEditor());
    MyEditorHandler* meh = MyEditorHandlerWin::getInstance();
    meh->setTopEditor(ed);
    ed->controlWindowTitle();
  }
  else
  {
    ShowWindow(ed->getCmdEd(),SW_HIDE);
  }
}

int
MyTabWindow::getEditorIdx( MyEditor* ed)
{
  MyEditorIndexer* mti = getEditorIndexerAtEditor(ed);
  if( mti ) return mti->_idx;

  return -1;
}

MyEditor*
MyTabWindow::getEditor( int idx )
{
  if( idx < 0 )
    return NULL;

  MyEditorIndexer* mti = getEditorIndexerAtIdx(idx);
  if( mti )
    return mti->_editor;

  return NULL;
}

MyEditor*
MyTabWindow::getTopEditor()
{
  int idx = getTopEditorIdx();
  MyEditor* me = getEditor(idx);
  return me;
}

int
MyTabWindow::getTopEditorIdx()
{
  int idx = -1;

  if( _tab )
    idx = TabCtrl_GetCurSel(_tab);

  return idx;
}

MyEditor*
MyTabWindow::getEditorAtTabPoint(POINT* p)
{

  int idx = getEditorIdxAtTabPoint(p);
  return getEditor(idx);
}

int
MyTabWindow::getEditorIdxAtTabPoint(POINT* p)
{
  TCHITTESTINFO hit;
  int idx = -1;

  hit.pt.x = p->x;
  hit.pt.y = p->y;

  return TabCtrl_HitTest(_tab,&hit);
}

void
MyTabWindow::setTopEditor( MyEditor* ed )
{
  int idx = getEditorIdx(ed);
  setTopEditor(idx);
}

void
MyTabWindow::setTopEditor( int idx )
{
  if( idx < 0 )
    return;

  hideAllEditors();
  display(idx,true);
  TabCtrl_SetCurSel(_tab,idx);
}

void
MyTabWindow::removeEditor( MyEditor* ed )
{
  int idx = getEditorIdx(ed);
  MyEditorIndexer* mei = NULL;
  int newIdx = 0;
  vector<MyEditorIndexer*>::iterator it = _editorList.begin();

  mei = getEditorIndexerAtIdx(idx);
  mei->_idx = -1;  if( !isEmpty() )
  {
    if( idx != 0 )
    {
      TabCtrl_SetCurSel(_tab,idx-1);
      newIdx = idx-1;
    }
    else
    {
      TabCtrl_SetCurSel(_tab,idx+1);
      newIdx = idx;
    }

    for( int i = idx; i < TabCtrl_GetItemCount(_tab)-1; i++ )
    {
      mei = getEditorIndexerAtIdx(i+1);
      mei->_idx = i;
    }
  }

  mei = getEditorIndexerAtIdx(-1);

  for( it; it != _editorList.end(); it++ )    if( *it == mei )    {
      if( *it )
      {
        delete *it;
        *it = NULL;
      }
      _editorList.erase(it);      break;    }
  TabCtrl_DeleteItem(_tab,idx);
  if( TabCtrl_GetItemCount(_tab) > 0 )
    display(newIdx);
}

MyEditorIndexer*
MyTabWindow::getEditorIndexerAtIdx(int idx)
{
  vector<MyEditorIndexer*>::const_iterator it = _editorList.begin();
  if( _editorList.size() <= 0 )
  {
    return NULL;
  }  for( it; it != _editorList.end(); it++ )    if( *it && (*it)->_idx == idx )      return *it;  return NULL;
}

MyEditorIndexer*
MyTabWindow::getEditorIndexerAtEditor(MyEditor* ed)
{
  vector<MyEditorIndexer*>::const_iterator it = _editorList.begin();
  if( _editorList.size() <= 0 )
  {
    return NULL;
  }  for( it; it != _editorList.end(); it++ )    if( *it && (*it)->_editor == ed )      return *it;  return NULL;
}

bool
MyTabWindow::doTabSwitch(int frmTabIdx, POINT* p)
{
  bool rc = false;
  int toTabIdx = getEditorIdxAtTabPoint(p);
  TCITEM kludgeTab, frmTab, toTab;
  char frmText[256];
  char toText[256];

  toTab.mask = frmTab.mask = TCIF_TEXT;
  toTab.cchTextMax = frmTab.cchTextMax = 256;
  frmTab.pszText = frmText;
  toTab.pszText = toText;

  if( frmTabIdx == toTabIdx )
  {
    resetPrevTabIdx();
    return false;
  }

  if( toTabIdx < 0 || toTabIdx == frmTabIdx)
    return false;

  if( toTabIdx == _prevFromTabIdx && frmTabIdx == _prevToTabIdx )
    return false;

  _prevFromTabIdx = frmTabIdx;
  _prevToTabIdx = toTabIdx;

  TabCtrl_GetItem(_tab,frmTabIdx,&frmTab);
  MyEditorIndexer* mei = getEditorIndexerAtIdx(frmTabIdx);
  mei->_idx = -1;

  if( toTabIdx > frmTabIdx ) // move everything left
  {
    for( int i = frmTabIdx; i < toTabIdx; i++ )
    {
      TabCtrl_GetItem(_tab,i+1,&toTab);
      toTab.mask = TCIF_TEXT;

      TabCtrl_SetItem(_tab,i,&toTab);
      mei = getEditorIndexerAtIdx(i+1);
      mei->_idx = i;
    }
  }
  else // move everything right
  {
    for( int i = frmTabIdx; i > toTabIdx; i-- )
    {
      TabCtrl_GetItem(_tab,i-1,&toTab);
      toTab.mask = TCIF_TEXT;
      TabCtrl_SetItem(_tab,i,&toTab);
      mei = getEditorIndexerAtIdx(i-1);
      mei->_idx = i;
    }
  }

  TabCtrl_SetItem(_tab,toTabIdx,&frmTab);

  mei = getEditorIndexerAtIdx(-1);

  mei->_idx = toTabIdx;
  TabCtrl_SetCurSel(_tab,toTabIdx);

  return true;
}

void
MyTabWindow::setTabTitle( int idx, char* title )
{
  TCITEM tab;
  TabCtrl_GetItem(_tab,idx,&tab);
  tab.mask = LVIF_TEXT;
  tab.pszText = title;
  TabCtrl_SetItem(_tab,idx,&tab);
}