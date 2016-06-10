#import "MyBrowser.h"
#import "MyEditor.h"

@implementation MyBrowser

// This method is called when a new tab is being created. We need to return a
// new CTTabContents object which will represent the contents of the new tab.
-(CTTabContents*)createBlankTabBasedOn:(CTTabContents*)baseContents {
  // Create a new instance of our tab type
  MyEditor* me = [[[MyEditor alloc]
      initWithBaseTabContents:baseContents] autorelease];

  return me;
}

-(CTTabContents*)addTabContents:(CTTabContents*)contents
                        atIndex:(int)index
                   inForeground:(BOOL)foreground
{
  MyEditor* me = [super addTabContents:contents atIndex:index inForeground:foreground];
  if( foreground )
    [me hideCmdEd:nil];

  return me;
}

-(void)closeDelegateForDocument:(MyEditor*)ed
                    shouldClose:(BOOL)shouldClose
                    contextInfo:(void*)contextInfo
{
  _canClose = shouldClose;
  [super closeTab];
}

-(void) closeTab
{
  MyEditor* ed = (MyEditor*)[self tabContentsAtIndex:[self selectedTabIndex]];

  if( ed && [ed isDocumentEdited] )
  {
    [ed canCloseDocumentWithDelegate:self
        shouldCloseSelector:@selector(closeDelegateForDocument:shouldClose:contextInfo:)
        contextInfo:nil];
  }
  else
  {
    _canClose = YES;
    [super closeTab];
  }
}

-(BOOL)canCloseTab
{
  return _canClose;
}

-(CTToolbarController *)createToolbarController;
{
  return nil;
}

-(void)selectTabContentsAtIndex:(int)index userGesture:(BOOL)userGesture;
{
  [super selectTabContentsAtIndex:index userGesture:userGesture];
  MyEditor* me = [self selectedTabContents];
  [me hideCmdEd:nil];
}

@end

MyEditor* getTopEditor(MyEditor *last)
{
  CTBrowserWindowController* topWC = [CTBrowserWindowController mainBrowserWindowController];
  MyEditor* ed = nil;
  if( topWC )
  {
    MyBrowser* browser = [topWC browser];
    ed = [browser selectedTabContents];
  }

  if( !ed )
    ed = last;

  return ed;
}
