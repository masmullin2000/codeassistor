### title info ###
MAC=yes

SCI_C_FLAGS := #
CTAB_FLAGS := #

ifdef DBG
OPT_OR_DBG=-g -O0
SCI_C_FLAGS += "DBG=yes"
CTAB_FLAGS += "DBG=yes"
else
OPT_OR_DBG=-O3 -w
endif

### from all dependencies, compile the target ###
COMPILE=-c $< -o $@ $(OPT_OR_DBG)

ifdef CLANG
CC=clang
SCI_C_FLAGS += "CLANG=yes"
CTAB_FLAGS += "CLANG=yes"
else
CC=gcc
endif

ifdef 64BIT
ARCH_TYPE=x86_64
SCI_C_FLAGS += "64BIT=yes"
CTAB_FLAGS += "64BIT=yes"
else
ARCH_TYPE=i386
endif

ARCH=-arch $(ARCH_TYPE)
C_OBJC=$(CC) -x objective-c++ $(ARCH) $(gDEFs) $(INCS)
C_C=$(CC) -x c++ $(ARCH) $(gDEFs) $(INCS)
C_CPP=$(CC) -x c++ $(ARCH) $(gDEFs) $(INCS)
LD=$(CC) $(ARCH) -L$(BLD) -framework Cocoa
AR=ar rcs

gDEFs=-DSCI_NAMESPACE -DSCI_LEXER
gDEFs+=-DMACOSX -DMASMULLIN_CHANGE

### location of scintilla... cocoa will be in $(SCI)/cocoa ###
SCI=../scintilla

CTAB=../chromium-tabs

### source directories common for all platforms ###
SRC_DIRS=src src/app src/ui src/ui/SciControl

### source directories solely for MACOSX ###
MAC_SRC_DIRS=src/app/mac src/ui/mac
SRC_DIRS+=$(MAC_SRC_DIRS)

### include dirs are scintilla + sources
INC_DIRS=$(SRC_DIRS) inc inc/ui inc/app \
  $(SCI)/include $(SCI)/cocoa $(SCI)/src $(SCI)/lexlib \
  $(CTAB)/src $(CTAB)/src/chrome/browser/cocoa $(CTAB)/src/chrome/browser/tabs \
  $(CTAB)/src/chrome/common $(CTAB)/src/base
	
INCS=$(addprefix -I,$(INC_DIRS))

vpath %.m $(SRC_DIRS)
vpath %.mm $(SRC_DIRS)
vpath %.cpp $(SRC_DIRS)
vpath %.cxx $(SRC_DIRS)
vpath %.c $(SRC_DIRS)
vpath %.xc $(SRC_DIRS)

vpath %.h $(INC_DIRS)

BLDA=build
BLDB=mac
BLD=$(BLDA)/$(BLDB)

APP=$(BLD)/$(NAME).app
TARG=$(APP)/Contents/MacOS/$(NAME)
TARG_REQS=$(BLD)/libapp.a $(BLD)/libcontrol.a $(BLD)/libmac.a $(BLD)/libcommand.a
TARG_FILES=$(TARG_REQS) \
  $(APP)/Contents/Frameworks/Sci.framework/Sci \
  $(APP)/Contents/Frameworks/ChromiumTabs.framework/ChromiumTabs
STARTAPP=MacAppStart

### targets ###

test1: $(APP) $(TARG)
	./$(TARG) tests/assign1.c

run: $(APP) $(TARG)
	open $(APP)

all: $(APP) $(TARG)

test: clean $(APP)

clean: cleanca cleansci cleanctab

cleanca:
	-rm -rf $(BLD)
	
dist:
	$(MAKE) -f mac.mk clean
	$(MAKE) -f mac.mk cleansci
	$(MAKE) -f mac.mk cleanctab
	$(MAKE) -f mac.mk all --quiet
	-rm $(NAME)-$(VERSION)-macApp-$(RLC).zip
	-cp -r $(APP) .
	-zip $(NAME)-$(VERSION)-macApp-$(RLC).zip -r $(NAME).app


$(APP): scintilla ctab $(BLD)
	-rm -rf $(APP)

	-mkdir $(APP)
	-mkdir $(APP)/Contents/
	-mkdir $(APP)/Contents/Frameworks/
	-mkdir $(APP)/Contents/MacOS/
	-mkdir $(APP)/Contents/Resources/

	-cp -R $(SCI)/cocoa/build/Framework/Sci.framework $(APP)/Contents/Frameworks/
	-cp -R $(CTAB)/build/framework/Release/ChromiumTabs.framework $(APP)/Contents/Frameworks/
	
	-cp -R res/ui/mac/*CT.nib $(APP)/Contents/Resources/
	-cp res/ui/mac/Info.plist $(APP)/Contents/Info.plist

	-sed -i "s/__EXE_NAME/$(NAME)/g" $(APP)/Contents/Info.plist
	-sed -i "s/__VERSION/$(VERSION)/g" $(APP)/Contents/Info.plist
	-sed -i "s/__RELEASE/$(RLC)/g" $(APP)/Contents/Info.plist

	-cp res/icons/from_www.clker.com/bookcoloured.icns $(APP)/Contents/Resources/

scintilla:
	cd $(SCI)/cocoa && $(MAKE) $(SCI_C_FLAGS) --quiet -f Framework.mk all
	@echo finished scintilla
	
ctab:
	cd $(CTAB) && $(MAKE) $(CTAB_FLAGS) --quiet -f chromium-tabs.mk framework
	@echo finished Chromium Tabs
	
getctab:
	-rm -rf ../chromium-tabs
	git clone git://github.com/rsms/chromium-tabs.git ../chromium-tabs
	cd $(CTAB) && git am < ../codeassistor/deps/64bit.patch

cleansci:
	cd $(SCI)/cocoa && $(MAKE) -f Framework.mk clean
	
cleanctab:
	cd $(CTAB) && $(MAKE) -f chromium-tabs.mk clean

$(BLD):
	-mkdir $(BLDA)
	-mkdir $(BLD)

targ : $(TARG)

$(TARG) : scintilla $(APP) $(TARG_REQS)
	$(LD) -ObjC $(TARG_FILES) -o $(TARG) -lstdc++

$(BLD)/%.o : %.xc
	$(C_OBJC) $(COMPILE)

$(BLD)/MyEditor.o: MyEditor.mm MyEditor.h keywords.h

include common.mk