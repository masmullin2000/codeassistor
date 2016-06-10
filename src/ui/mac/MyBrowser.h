#import "MyChromiumTabs.h"
#import "MyEditor.h"

// We provide our own CTBrowser subclass so we can create our own, custom tabs.
// See the implementation file for details.

@interface MyBrowser : CTBrowser {
  BOOL _canClose;
}
@end

MyEditor* getTopEditor(MyEditor* last);