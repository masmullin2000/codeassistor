#ifndef __MY_TAB_WINDOW_H__
#define __MY_TAB_WINDOW_H__
#include <windows.h>
#include <vector>

class MyEditor;
#ifdef RELEASE_VERSION
const char g_szClassName[] = "CodeAssistorEditor";
#else
const char g_szClassName[] = "CodeAssistorEditor_test";
#endif

struct MyEditorIndexer
{
  MyEditor* _editor;
  unsigned int _idx;
};

class MyTabWindow
{
private:
  HWND  _window;
  HWND  _tab;
  HMENU _menu;
  HINSTANCE _app;
  
  boolean _isDragging;
  
  int _prevFromTabIdx, _prevToTabIdx;
  
  std::vector<MyEditorIndexer*> _editorList;

  int createMenu(void);
  int createWindow(void);
  int createTabCtrl(void);
protected:
public:
  MyTabWindow(HINSTANCE app);

  HWND getHWND() { return _window; }
  HWND getTabCtrl() { return _tab; }
  int createTab(MyEditor* ed);
  void hideAllEditors(void);
  void display(int idx, boolean show = true);
  
  bool isOwner(HWND win);
  
  bool isEmpty();
  
  int editorCount() { return _editorList.size(); }
  
  int       getEditorIdx( MyEditor* ed);
  MyEditor* getEditor( int idx );
  
  MyEditor* getTopEditor();
  int       getTopEditorIdx();
  MyEditor* getEditorAtTabPoint(POINT* p);
  int       getEditorIdxAtTabPoint(POINT* p);
  
  void setTopEditor( MyEditor* ed );
  void setTopEditor( int idx );
  
  void removeEditor( MyEditor* ed );
  
  void setDragging(boolean isDrag = true)
  {
    _isDragging = isDrag;
  }
  
  bool isDragging() { return _isDragging; }
  
  MyEditorIndexer* getEditorIndexerAtIdx(int idx);
  MyEditorIndexer* getEditorIndexerAtEditor(MyEditor* ed);
  
  bool doTabSwitch(int frmIdx, POINT* p);
  
  void setTabTitle(int idx, char* title);
  
  void resetPrevTabIdx() { _prevFromTabIdx = _prevToTabIdx = -1; }
};

#endif