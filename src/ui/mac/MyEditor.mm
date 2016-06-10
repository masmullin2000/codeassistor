#import "MyEditor.h"
#import "MyEditorNotify.h"
#import "MyEditorCommon.h"

#import "MyBrowser.h"

#import "CScintillaController.h"
#import "TXTScintillaController.h"
#import "MKScintillaController.h"
#import "SQLScintillaController.h"
#import "PYScintillaController.h"
#import "PerlScintillaController.h"
#import "PHPScintillaController.h"
#import "SHScintillaController.h"
#import "BATScintillaController.h"
#import "ScintillaSend.h"

#import "Scintilla.h"
#import "InfoBar.h"

#import "MyCommand.h"

using namespace Scintilla;

/* offset numbers to set the scintilla editor
 * in the correct spot on the NSView
 * values obtained via experimentation
 */
#define SCINTILLA_EDITOR_X_OFFSET       0 //26
#define SCINTILLA_EDITOR_Y_OFFSET        0
#define SCINTILLA_EDITOR_WIDTH_OFFSET  0//-13
#define SCINTILLA_EDITOR_HEIGHT_OFFSET   0

@implementation MyWindow

-(void)resignMainWindow
{
  [self makeFirstResponder:nil];
  [super resignKeyWindow];
}

-(void)becomeKeyWindow
{
  MyEditor* me = (MyEditor*)[[self windowController] document];
  if( me )
    [self makeFirstResponder:[[me getEditor] content]];

  [super becomeKeyWindow];
}

@end

@implementation MyTextFieldDelegate
- (void)controlTextDidChange:(NSNotification *)aNotification
{
  MyTextField* mtf = (MyTextField*)[aNotification object];
  MyEditor* me = [mtf owner];

  NSString* str = [mtf stringValue];

  BaseEditor* b = [me base];
  b->_cmd.CmdEntry(me,[str UTF8String]);
}
@end

@implementation MyTextField

-(BOOL)acceptsFirstResponder
{
  return YES;
}

-(MyEditor*)owner
{
  return _owner;
}

-(void) setOwner:(MyEditor*)me
{
  _owner = me;
}

-(void)cancelOperation:(id)sender
{
  [_owner toggleCmdEd:sender];
}
@end

@implementation MyScintillaView
-(MyEditor*)owner
{
  return _owner;
}

-(void) setOwner:(MyEditor*)me
{
  _owner = me;
}

- (void) scrollerAction: (id) sender
{
  [super scrollerAction: sender];
  [super display];
}
@end

@implementation MyEditor

-(void)makeWindowControllers
{
  CTBrowserWindowController* windowController =
      [[CTBrowserWindowController alloc] initWithBrowser:[MyBrowser browser]];
  [windowController.browser addTabContents:self inForeground:YES];
  [windowController showWindow:self];
}

- (id)initBase
{
  if( self )
  {
    _fType = FILE_TYPE_UNINIT;
  }
  _sciHost = _sciEditor = NULL;
  _base = new BaseEditor(&_sciEditor);

  [NSBundle loadNibNamed:@"MyEditorCT" owner:self];

  return self;
}
- (id)init
{
  if( !(self = [self initWithBaseTabContents:self]) )
    return nil;

  return self;
}

-(id)initWithBaseTabContents:(CTTabContents*)baseContents
{
  if( ![super initWithBaseTabContents:baseContents])
    return nil;
  [self initBase];
  [self setupView];
  self.icon = [NSImage imageNamed:@"bookcoloured.icns"];
  self.title = @"Untitled";

  return self;
}

-(BaseEditor*)base
{
  return _base;
}

-(NSWindow*)getWindow
{
  NSWindow* rc = nil;
  rc = [[self browser] window];
  return rc;
}

-(NSView*)getHost
{
  return _sciHost;
}

/* when we quit/close with a "dirty" editor, we will be promted to save */
-(BOOL) isDocumentEdited
{
  return _base->isDirty();
}

-(void) setupView
{
  ScintillaController* ed = NULL;
  NSRect newFrame = _sciHost.frame;

  /* set the scintilla editor to the right size inside the host NSView */
  newFrame.origin.x    += SCINTILLA_EDITOR_X_OFFSET;
  newFrame.origin.y    += SCINTILLA_EDITOR_Y_OFFSET;
  newFrame.size.width  += SCINTILLA_EDITOR_WIDTH_OFFSET;
  newFrame.size.height += SCINTILLA_EDITOR_HEIGHT_OFFSET;

  if( _sciEditor == NULL )
    _sciEditor = [[[MyScintillaView alloc] initWithFrame: newFrame] autorelease];

  MyTextFieldDelegate* _mtfd = [[MyTextFieldDelegate alloc] init];
  [_cmdEd setOwner: self];
  [_findField setOwner: self];
  [_lineField setOwner: self];
  [_cmdEd setDelegate:_mtfd];
  [_findField setDelegate:_mtfd];
  [_lineField setDelegate:_mtfd];

  //MyWindow* mw = (MyWindow*)[self getWindow];
  //if(mw)
  //[mw setOwner:self];

  [_sciHost addSubview: _sciEditor positioned: NSWindowBelow relativeTo: nil];
  [_sciEditor setAutoresizesSubviews: YES];
  [_sciEditor setAutoresizingMask: NSViewWidthSizable | NSViewHeightSizable];

  InfoBar* infoBar = [[[InfoBar alloc] initWithFrame: NSMakeRect(0, 0, 200, 0)] autorelease];
  [infoBar setDisplay: IBShowAll];
  [_sciEditor setInfoBar: infoBar top: NO];
  [_sciEditor setStatusText: @"Input Mode"];


  //Scintilla::ScintillaCocoa* back = [_sciEditor backend];
  //back->RegisterNotifyCallback(nil, notification);
  [_sciEditor registerNotifyCallback: nil value: notification];


  [_sciEditor setOwner: self];
  if( _fType < 0 ) return; // kick out early uninitialize or error

  ed = GetSciType(_fType);

  if( ed )
  {
    ed->setDefaultEditor(_sciEditor,_fType);
  }

  self.view = _sciHost;
  [_sciHost display];

}

/* when loaded from the NIB file, lets setup the editor as a C++ editor
 * use the CScintillaController to set us up
 */
- (void)awakeFromNib
{

}

/* we use the MyEditor.nib file for our main window */
- (NSString *)windowNibName
{
  return @"MyEditorCT";
}

-(void)loadText
{

  if( _fileData )
  {
    NSString* text = [[NSString alloc]initWithData:_fileData
                                          encoding:NSUTF8StringEncoding];

    if( text == nil ) text = [[NSString alloc]initWithData:_fileData
                                                  encoding: NSASCIIStringEncoding];
    [_sciEditor setString:text];
    [text release];
  }

  [self setupView];
  _base->setDirtyOverride(false);
  [self controlWindowTitle];
  ScintillaSend* send = ScintillaSend::getInstance();
  send->set(_sciEditor,SCI_SETSAVEPOINT,0,0);
  send->set(_sciEditor,SCI_EMPTYUNDOBUFFER,0,0);
  send->set(_sciEditor,SCI_COLOURISE,0,-1);
}

/* after the window has been setup, put the text on if there is any
 * set a save point scintilla can tell us if the document is dirty
 */
/* dead code
- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{

  [super windowControllerDidLoadNib:aController];
  [self loadText];

}*/

/* method used when we want to save */
- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{

  NSString* text = [_sciEditor string];
  return [text dataUsingEncoding:NSUTF8StringEncoding];
}

- (BOOL)saveToURL:(NSURL *)absoluteURL
           ofType:(NSString *)typeName
 forSaveOperation:(NSSaveOperationType)saveOperation
            error:(NSError **)outError
{
  BOOL rc = [super saveToURL:absoluteURL
                      ofType:typeName
            forSaveOperation:saveOperation
                       error:outError];
  if( rc == YES )
  {
    [_sciEditor setGeneralProperty: SCI_SETSAVEPOINT parameter: 0 value: 0];

    char* file = (char*)[[absoluteURL path] UTF8String];
    _base->setFile(file);
    _base->setTouch(GetTime(file),GetFileSize(file));
    _base->setDirtyOverride(false);
    [self controlWindowTitle];
  }
  return rc;
}

-(BOOL)readFromURL:(NSURL*)absoluteURL
            ofType:(NSString*)typeName
             error:(NSError**)outError
{

  BOOL rc = NO;
  char* file = (char*)[[absoluteURL path] UTF8String];

  if( GetFileSize(file) <= MAX_FILE_SIZE )
  {
    _base->setFile(file);
    _base->setTouch(GetTime(file),GetFileSize(file));
    _base->setDirtyOverride(false);

    [self setFileModificationDate: [NSDate dateWithTimeIntervalSince1970:_base->getTime()]];

    [super readFromURL:absoluteURL ofType:typeName error:outError];

    rc = YES;
  }
  return rc;
}

/* method used when we load a file */
- (BOOL)readFromData: (NSData *)data
              ofType:(NSString *)typeName
               error:(NSError **)outError
{

  const char* fType = [typeName UTF8String];

  _fType = GetFileType(fType);
  _fileData = [data retain];
  [self loadText];
  NSString *sLine = [[NSUserDefaults standardUserDefaults] stringForKey:@"line"];
  if( sLine )
  {
    int line = atoi([sLine UTF8String]);
    _base->gotoLine(line-1);
  }
  return YES;
}

-(void)controlWindowTitle
{
  NSMutableString* myTitle = [NSMutableString string];
  [myTitle setString:[self displayName]];
  if( [self isDocumentEdited] )
  {
    [myTitle insertString:@"-*" atIndex:0];
    [myTitle appendString:@"*-"];
  }
  self.title = myTitle;
}

-(fileType_t)getFType
{
  return _fType;
}

-(IBAction)findForward:(id)sender
{
  _base->find( DOWN );
}

-(IBAction)findBackward:(id)sender
{
  _base->find( UP );
}

-(IBAction)findText:(id)sender
{
  NSString* str = [_findField stringValue];
  _base->setTextToFind((char*)[str UTF8String]);

  [self findForward:sender];
}

-(IBAction)goToLine:(id)sender
{
  int line = [_lineField intValue];

  _base->gotoLine(line-1);

  [[self getWindow] makeFirstResponder:[_sciEditor content]];

  [_lineField setHidden:true];
}

-(IBAction)showFind:(id)sender
{
  [_findField setHidden:false];
  [_lineField setHidden:true];
  [_cmdEd     setHidden:true];

  [[self getWindow] makeFirstResponder:_findField];
}

-(IBAction)showGoto:(id)sender
{
  [_lineField setHidden:false];
  [_findField setHidden:true];
  [_cmdEd     setHidden:true];

  [[self getWindow] makeFirstResponder:_lineField];
}

-(IBAction)hideFind:(id)sender
{
  [_findField setHidden:true];
  [_lineField setHidden:true];
  [_cmdEd     setHidden:true];

  [[self getWindow] makeFirstResponder:[_sciEditor content]];
}

-(BOOL)isCmdEdShown;
{
  return ![_cmdEd isHidden];
}

-(IBAction)cmdAction:(id)sender
{
  NSString* str = [_cmdEd stringValue];

  BaseEditor* b = [self base];
  b->_cmd.CmdExecute(self,[str UTF8String]);
}

-(IBAction)showCmdEd:(id)sender
{

  ScintillaSend* send = ScintillaSend::getInstance();
  [_findField setHidden:true];
  [_lineField setHidden:true];
  [_cmdEd     setHidden:false];

  [_sciEditor setStatusText: @"Command Mode"];
  _base->_cmd.setStartPos( send->get(_sciEditor,SCI_GETCURRENTPOS) );

  [[self getWindow] makeFirstResponder:_cmdEd];
}

-(IBAction)hideCmdEd:(id)sender
{
  [_findField setHidden:true];
  [_lineField setHidden:true];
  [_cmdEd     setHidden:true];

  [_sciEditor setStatusText: @"Input Mode"];

  [[self getWindow] makeFirstResponder:[_sciEditor content]];
}

-(IBAction)toggleCmdEd:(id)sender
{
  if( ![_findField isHidden] || ![_lineField isHidden] || ![_cmdEd isHidden] )
    [self hideCmdEd:sender];
  else
    [self showCmdEd:sender];
}

-(IBAction)toggleWrap:(id)sender
{
  _base->toggleWrap();
}

-(IBAction)foldAll:(id)sender
{
  _base->foldAll();
}

-(IBAction)unfoldAll:(id)sender
{
  _base->unfoldAll();
}

-(void) toggleShowLineEndings
{
  _base->toggleShowLineEndings();
}

-(void) setLineEndings:(int)style
{
  _base->setLineEndings(style);
}

-(bool) getLineEndingsShown
{
  return _base->getLineEndingsShown();
}

-(ScintillaView*)getEditor
{
  return (ScintillaView*)*(_base->getEditor());
}

-(const char*)getFile
{
  return _base->getFile();
}

#define BUTTON_UPDATE     1000
#define BUTTON_KEEPMINE   1001

- (void) alertDidEnd:(NSAlert *)alert
          returnCode:(NSInteger)returnCode
         contextInfo:(void *)contextInfo
{
  switch( returnCode )
  {
    case BUTTON_UPDATE:
    {
      ScintillaSend* send = ScintillaSend::getInstance();
      int firstline = send->get( _sciEditor, SCI_GETFIRSTVISIBLELINE);
      int pos = send->get( _sciEditor, SCI_GETCURRENTPOS);
      int line = send->get( _sciEditor, SCI_LINEFROMPOSITION,pos);

      NSDocumentController* dc = [NSDocumentController sharedDocumentController];
      NSString* str = [NSString stringWithCString:[self getFile] encoding:NSUTF8StringEncoding];
      NSURL* url = [NSURL fileURLWithPath:str isDirectory:NO];
      [self readFromURL:url
                 ofType:[dc typeForContentsOfURL:url error:nil]
                  error:nil];
      [self loadText];
      send->set( _sciEditor, SCI_SETSAVEPOINT, 0, 0 );
      send->set( _sciEditor, SCI_GOTOLINE,line,0);
      send->set( _sciEditor, SCI_SETFIRSTVISIBLELINE,firstline,0);
    }
    break;
  case BUTTON_KEEPMINE:
    _base->setTouch(GetTime(_base->getFile()),GetFileSize(_base->getFile()));
    _base->setDirtyOverride(true);
    break;
  default:
    ;
  }
  [self controlWindowTitle];

  [self hideFind:nil];
}

-(void) checkUpdate
{
  if( _base->checkUpdate() )
  {
   NSAlert *theAlert = [[[NSAlert alloc] init] autorelease];
   // set up buttons
   [theAlert addButtonWithTitle:@"Update"];
   [theAlert addButtonWithTitle:@"Keep Mine"];

   // set up text
   [theAlert setMessageText:[[self displayName] stringByAppendingString: @" - External File Change"]];
   [theAlert setInformativeText:@"This file has been changed by an external application.  Would you like to update, or keep your copy?\n\nWARNING: Chosing to update will discard any changes you have made"];

    // choose alert kind
   [theAlert setAlertStyle:NSWarningAlertStyle];

      // roll out the sheet!
      // We're not using a delegate or its hook. If we were,
      // we would define a method called
      // alertDidEnd:returnCode:contextInfo  and
      // pass @selector(alertDidEnd:returnCode:contextInfo:)
      //    for didEndSelector and self for modalDelegate
   [theAlert beginSheetModalForWindow:[self getWindow]
            modalDelegate:self
            didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:)
            contextInfo: self];

  }
}

-(void)setIsSelected:(BOOL)selected
{
  [super setIsSelected:selected];
  if( [self isSelected] )
    [self hideCmdEd:nil];
}

- (void)tabWillCloseInBrowser:(CTBrowser*)browser atIndex:(NSInteger)index
{
  [super tabWillCloseInBrowser:browser atIndex:index];
  [[NSDocumentController sharedDocumentController] removeDocument:self];
  [self close];
}

-(void)tabDidBecomeSelected
{
  // nothing todo here
}
@end

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
