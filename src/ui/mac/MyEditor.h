#import <Cocoa/Cocoa.h>
#import "ScintillaView.h"
#import "keywords.h"
#import "MyChromiumTabs.h"

#include "BaseEditor.h"

@class MyEditor;

@interface MyWindow : NSWindow
{
}
@end

@interface MyTextFieldDelegate : NSControl <NSTextFieldDelegate>
{
}
@end

@interface MyTextField : NSTextField
{
  MyEditor* _owner;
}
-(MyEditor*)owner;
-(void)setOwner:(MyEditor*)me;
@end

@interface MyScintillaView : ScintillaView
{
  MyEditor* _owner;
}
-(MyEditor*)owner;
-(void)setOwner:(MyEditor*)me;
-(void)scrollerAction: (id) sender;

@end

@interface MyEditor : CTTabContents
{
  IBOutlet NSView*  _sciHost;   /* the host NSView of the scintilla editor */
  MyScintillaView*  _sciEditor; /* the scintilla editor, where the magic happens */
  NSData*           _fileData;  /* file data */
  fileType_t        _fType;

  BaseEditor*       _base;

  
  IBOutlet MyTextField* _findField;
  IBOutlet MyTextField* _lineField;
  IBOutlet MyTextField* _cmdEd;
}

-(void)controlWindowTitle;

-(fileType_t)getFType;

-(BaseEditor*)base;
-(ScintillaView*)getEditor;
-(NSWindow*)getWindow;
-(NSView*)getHost;

-(id)initWithBaseTabContents:(CTTabContents*)baseContents;
-(void)setupView;

-(IBAction)findText:(id)sender;
-(IBAction)findForward:(id)sender;
-(IBAction)findBackward:(id)sender;
-(IBAction)showFind:(id)sender;
-(IBAction)hideFind:(id)sender;

-(IBAction)goToLine:(id)sender;
-(IBAction)showGoto:(id)sender;

-(BOOL)isCmdEdShown;
-(IBAction)cmdAction:(id)sender;
-(IBAction)showCmdEd:(id)sender;
-(IBAction)hideCmdEd:(id)sender;
-(IBAction)toggleCmdEd:(id)sender;

-(IBAction)toggleWrap:(id)sender;
-(IBAction)foldAll:(id)sender;
-(IBAction)unfoldAll:(id)sender;

//-(void) setShowLineEndings:(bool)isShow;
-(void) setLineEndings:(int)style;
-(bool) getLineEndingsShown;

-(const char*) getFile;

-(void)loadText;

- (void) alertDidEnd:(NSAlert *)alert
          returnCode:(NSInteger)returnCode
         contextInfo:(void *)contextInfo;

-(void) checkUpdate;
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