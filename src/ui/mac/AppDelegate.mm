#import "AppDelegate.h"
#import "MyBrowser.h"
#import "MyEditor.h"

@implementation MyNSDocController

- (id)openDocumentWithContentsOfURL:(NSURL *)absoluteURL display:(BOOL)displayDocument error:(NSError **)outError
{
  MyEditor* me = nil;
  CTBrowserWindowController* topWC = [CTBrowserWindowController mainBrowserWindowController];

  me = [self documentForURL:absoluteURL];
  if( me )
  {
    CTBrowserWindowController* hostWC = [CTBrowserWindowController browserWindowControllerForWindow:[me getWindow]];
    [hostWC activate];
    int idx = [hostWC.browser indexOfTabContents:me];
    printf("idx is %d",idx);fflush(stdout);
    if( idx >= 0 )
      [hostWC.browser selectTabAtIndex:idx];
  }else if( displayDocument && topWC )
  {
    me = [super openDocumentWithContentsOfURL:absoluteURL display:NO error:outError];
    [topWC.browser addTabContents:me inForeground:YES];
  }
  else
  {
    me = [super openDocumentWithContentsOfURL:absoluteURL display:displayDocument error:outError];
  }

  return me;
}

+(id) sharedDocumentController
{
  self = [super sharedDocumentController];
  return self;
}
@end

static MyEditor* gLastTop = nil;

@implementation AppDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
  MyNSDocController* mydc = [MyNSDocController sharedDocumentController];
}

-(void)applicationWillResignActive:(NSNotification*)aNotification
{
  gLastTop = getTopEditor(gLastTop);
}

-(void)applicationDidBecomeActive:(NSNotification*)aNotification
{
  MyEditor* ed = getTopEditor(gLastTop);

  [ed checkUpdate];
}

// When there are no windows in our application, this class (AppDelegate) will
// become the first responder. We forward the command to the browser class.
- (void)commandDispatch:(id)sender {
  NSLog(@"commandDispatch %d", [sender tag]);
  [MyBrowser executeCommand:[sender tag]];
}

-(IBAction)findText:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed findText:sender];
}

-(IBAction)findForward:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed findForward:sender];
}

-(IBAction)findBackward:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed findBackward:sender];
}

-(IBAction)showFind:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed showFind:sender];
}

-(IBAction)hideFind:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed hideFind:sender];
}
-(IBAction)toggleCmdEd:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed toggleCmdEd:sender];
}
-(IBAction)toggleWrap:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed toggleWrap:sender];
}

-(IBAction)foldAll:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed foldAll:sender];
}
-(IBAction)unfoldAll:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed unfoldAll:sender];
}

-(IBAction)goToLine:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed goToLine:sender];
}

-(IBAction)showGoto:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed showGoto:sender];
}

-(IBAction)cmdAction:(id)sender;
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed cmdAction:sender];
}

-(IBAction)newTab:(id)sender
{
  CTBrowserWindowController* topWC = [CTBrowserWindowController mainBrowserWindowController];
  [topWC.browser addBlankTabInForeground:YES];
  [topWC showWindow:self];
}

-(IBAction)openTab:(id)sender
{
  CTBrowserWindowController* topWC = [CTBrowserWindowController mainBrowserWindowController];
  NSDocumentController* nsdc = [NSDocumentController sharedDocumentController];
  NSArray* filesToOpen = [nsdc URLsFromRunningOpenPanel];

  for( int i = 0; i < [filesToOpen count]; i++ )
  {
    NSURL* file = [filesToOpen objectAtIndex:i];
    MyEditor* me = [nsdc documentForURL:file];

    BOOL isNew = NO;
    isNew = (nil == me) ? YES : NO;

    me = [nsdc openDocumentWithContentsOfURL:file display:NO error:nil];
    if( isNew )
      [topWC.browser addTabContents:me inForeground:YES];

  }
}

-(IBAction)openWindow:(id)sender
{
  NSDocumentController* nsdc = [NSDocumentController sharedDocumentController];
  NSArray* filesToOpen = [nsdc URLsFromRunningOpenPanel];

  if( [filesToOpen count] > 0 )
  {
    NSURL* file = [filesToOpen objectAtIndex:0];
    MyEditor* me = [nsdc documentForURL:file];

    BOOL isNew = NO;
    isNew = (nil == me) ? YES : NO;

    me = [nsdc openDocumentWithContentsOfURL:file display:NO error:nil];
    if( isNew ) [me makeWindowControllers];
  }
  CTBrowserWindowController* topWC = [CTBrowserWindowController mainBrowserWindowController];

  for( int i = 1; i < [filesToOpen count]; i++ )
  {
    NSURL* file = [filesToOpen objectAtIndex:i];
    MyEditor* me = [nsdc documentForURL:file];

    BOOL isNew = NO;
    isNew = (nil == me) ? YES : NO;
    me = [nsdc openDocumentWithContentsOfURL:file display:NO error:nil];

    if( isNew )
      [topWC.browser addTabContents:me inForeground:YES];
  }
}

-(IBAction)closeTab:(id)sender
{
  CTBrowserWindowController* topWC = [CTBrowserWindowController mainBrowserWindowController];

  [topWC.browser closeTab];
}

-(IBAction)saveTab:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);

  [ed saveDocument:sender];
}

-(IBAction)saveTabAs:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed saveDocumentAs:sender];
}

-(IBAction)toggleShowLineEndings:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  [ed toggleShowLineEndings];
}

-(IBAction)setWindowsLineEndings:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  
  [ed setLineEndings:SC_EOL_CRLF];
}

-(IBAction)setMacLineEndings:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  
  [ed setLineEndings:SC_EOL_CR];
}

-(IBAction)setLinuxLineEndings:(id)sender
{
  MyEditor* ed = getTopEditor(gLastTop);
  
  [ed setLineEndings:SC_EOL_LF];
}

@end
