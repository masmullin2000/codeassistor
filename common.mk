NAME=CodeAssistor
VERSION=v0.0.8i-ColourExperiment
RLC=Final

usage:
	@echo make -f win.mk test

$(BLD)/libapp.a: $(BLD)/main.o $(BLD)/$(STARTAPP).o
	$(AR) $@ $^

$(BLD)/libwin.a: $(BLD)/MyEditor.o \
	$(BLD)/MyEditorHandler.o \
	$(BLD)/MyEditorHandlerWin.o \
	$(BLD)/MyEditorDlg.o \
	$(BLD)/WinFile.o \
	$(BLD)/BaseEditor.o \
	$(BLD)/MyTabWindow.o
	$(AR) $@ $^

$(BLD)/liblin.a: $(BLD)/MyEditor.o \
	$(BLD)/MyEditorHandler.o \
	$(BLD)/MyEditorHandlerLin.o \
	$(BLD)/MyEditorCallbacks.o \
	$(BLD)/MyTabWindow.o \
	$(BLD)/BaseEditor.o
	$(AR) $@ $^

$(BLD)/libmac.a: $(BLD)/MyEditor.o \
	$(BLD)/MyEditorNotify.o \
	$(BLD)/BaseEditor.o \
	$(BLD)/AppDelegate.o \
	$(BLD)/MyBrowser.o
	$(AR) $@ $^
  
$(BLD)/libcontrol.a: $(BLD)/CScintillaController.o \
	$(BLD)/TXTScintillaController.o \
	$(BLD)/MKScintillaController.o \
	$(BLD)/SQLScintillaController.o \
	$(BLD)/PYScintillaController.o \
	$(BLD)/PerlScintillaController.o \
	$(BLD)/PHPScintillaController.o \
	$(BLD)/SHScintillaController.o \
	$(BLD)/BATScintillaController.o \
	$(BLD)/ScintillaController.o \
	$(BLD)/ScintillaSend.o \
	$(BLD)/MyEditorCommon.o
	$(AR) $@ $^
	
$(BLD)/libcommand.a: $(BLD)/MyCommand.o
	$(AR) $@ $^

$(BLD)/%.o : %.m
	$(C_OBJC) $(COMPILE)

$(BLD)/%.o : %.mm
	$(C_OBJC) $(COMPILE)

$(BLD)/%.o : %.c
	$(C_C) $(COMPILE)

$(BLD)/%.o : %.cpp
	$(C_CPP) $(COMPILE)

$(BLD)/%.o : %.cxx
	$(C_CPP) $(COMPILE)
	
$(BLD)/%.o : %.rc
	windres $< -o $@

$(BLD)/main.o:  main.c appStart.h

$(BLD)/WinAppStart.o: WinAppStart.c appStart.h

$(BLD)/BaseEditor.o: BaseEditor.xc BaseEditor.h ScintillaSend.h

$(BLD)/MyEditorHandler.o: MyEditorHandler.cpp MyEditorHandler.h MyEditor.h

$(BLD)/MyEditorHandlerWin.o: MyEditorHandlerWin.cpp MyEditorHandler.h MyEditorHandlerWin.h MyEditor.h

$(BLD)/MyEditorHandlerLin.o: MyEditorHandlerLin.cpp MyEditorHandler.h MyEditorHandlerLin.h MyEditor.h

$(BLD)/MyEditorHandlerMac.o: MyEditorHandlerMac.mm MyEditorHandlerMac.h

$(BLD)/MyEditorCallbacks.o: MyEditorCallbacks.cpp MyEditorCallbacks.h MyEditorHandlerLin.h $(BLD)/bookcoloured.xpm
	$(C_CPP) $(COMPILE) -DVER=$(VERSION) -DREL=$(RLC)

$(BLD)/WinFile.o: WinFile.cpp WinFile.h

$(BLD)/MyEditorCommon.o: MyEditorCommon.xc MyEditorCommon.h ScintillaSend.h common_defs.h keywords.h ScintillaController.h

$(BLD)/MyEditorNotify.o: MyEditorNotify.mm MyEditorNotify.h ScintillaSend.h MyEditor.h MyEditorCommon.h

$(BLD)/MyEditorDlg.o: MyEditorDlg.cpp MyEditorDlg.h MyEditor.h MyEditorHandler.h MyEditorCommon.h WinFile.h resource.h ScintillaSend.h

$(BLD)/CScintillaController.o : CScintillaController.cpp CScintillaController.h keywords.h

$(BLD)/TXTScintillaController.o : TXTScintillaController.cpp TXTScintillaController.h keywords.h

$(BLD)/SQLScintillaController.o : SQLScintillaController.cpp SQLScintillaController.h keywords.h

$(BLD)/MKScintillaController.o : MKScintillaController.cpp MKScintillaController.h keywords.h

$(BLD)/ScintillaController.o : ScintillaController.cpp ScintillaController.h keywords.h

$(BLD)/ScintillaSend.o : ScintillaSend.xc ScintillaSend.h keywords.h

$(BLD)/PYScintillaController.o : PYScintillaController.cpp PYScintillaController.h keywords.h

$(BLD)/PerlScintillaController.o : PerlScintillaController.cpp PerlScintillaController.h keywords.h

$(BLD)/PHPScintillaController.o : PHPScintillaController.cpp PHPScintillaController.h keywords.h

$(BLD)/SHScintillaController.o :  SHScintillaController.cpp SHScintillaController.h keywords.h

$(BLD)/BATScintillaController.o :  BATScintillaController.cpp BATScintillaController.h keywords.h

$(BLD)/MyCommand.o : MyCommand.xc MyCommand.h MyEditor.h

$(BLD)/resource.o : resource.rc resource.h
	-cp res/ui/win/resource.rc $(BLD)/resource.rc
	-cp res/ui/win/resource.h $(BLD)/resource.h
	-$(SED) -i "s/__VERSION/$(VERSION)/g" $(BLD)/resource.rc
	-$(SED) -i "s/__RELEASE/$(RLC)/g" $(BLD)/resource.rc
	windres $(BLD)/resource.rc -o $@

$(BLD)/findres.o : findres.rc resource.h

$(BLD)/bookcoloured.xpm: res/icons/from_www.clker.com/bookcoloured.ico
	convert res/icons/from_www.clker.com/bookcoloured-lin.ico $(BLD)/bookcoloured.xpm
	sed -i "s/static char /const char /" $(BLD)/bookcoloured.xpm