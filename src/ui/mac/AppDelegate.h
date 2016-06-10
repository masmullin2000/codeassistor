#import <Cocoa/Cocoa.h>
#import "MyEditor.h"

@interface MyNSDocController : NSDocumentController
{
  
}
@end

@interface AppDelegate : NSObject <NSApplicationDelegate> {
}

- (void)commandDispatch:(id)sender;

-(MyEditor*)getTopEditor;

-(IBAction)findText:(id)sender;
-(IBAction)findForward:(id)sender;
-(IBAction)findBackward:(id)sender;

-(IBAction)showFind:(id)sender;
-(IBAction)hideFind:(id)sender;
-(IBAction)toggleCmdEd:(id)sender;
-(IBAction)toggleWrap:(id)sender;
-(IBAction)foldAll:(id)sender;
-(IBAction)unfoldAll:(id)sender;
-(IBAction)goToLine:(id)sender;
-(IBAction)showGoto:(id)sender;
-(IBAction)cmdAction:(id)sender;

-(IBAction)newTab:(id)sender;
-(IBAction)openTab:(id)sender;
-(IBAction)openWindow:(id)sender;

-(IBAction)closeTab:(id)sender;

-(IBAction)saveTab:(id)sender;
-(IBAction)saveTabAs:(id)sender;

-(IBAction)toggleShowLineEndings:(id)sender;
-(IBAction)setWindowsLineEndings:(id)sender;
-(IBAction)setMacLineEndings:(id)sender;
-(IBAction)setLinuxLineEndings:(id)sender;

@end
