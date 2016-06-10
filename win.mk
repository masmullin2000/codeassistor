NAME=CodeAssistor

SCINTILLA_DEFS=#

ifdef CLANG
STATIC=yes
CC=clang
CPP=$(CC)++
SCINTILLA_DEFS+= CLANG=yes
else
CC=gcc
CPP=g++
endif
C_C=$(CC) -x c++ $(gDEFs) $(INCS)
C_CPP=$(CC) -x c++ $(gDEFs) $(INCS)

AR=ar rcs

gDEFs=-DSCI_NAMESPACE -DSCI_LEXER
gDEFs+=-DWINDOWS -D_WIN32_IE=0x500
SCI=../scintilla

SED=nsed

ifdef DBG
OPT_OR_DBG=-g -O0
SUBSYS=console
SCINTILLA_DEFS+= DEBUG=yes
STRIP=#
else
OPT_OR_DBG=-Os -w
SUBSYS=windows
STRIP=-Wl,-s
endif

### from all dependencies, compile the target ###
COMPILE=-c $< -o $@ $(OPT_OR_DBG)

ifdef STATIC
COMPILE += -DSTATIC_BUILD
SCINTILLA_DEFS+= ../bin/libsci.a STATIC=yes
endif

ifdef RELEASE
COMPILE += -DRELEASE_VERSION
endif

BLDA=build
BLDB=win
BLD=$(BLDA)/$(BLDB)

EXE=$(NAME).exe
APP=$(BLD)/$(EXE)
STARTAPP=WinAppStart


SRC_DIRS=src src/app src/ui src/ui/SciControl
WIN_SRC_DIRS=src/app/win src/ui/win res/ui/win src/ui/win/tabs
SRC_DIRS+=$(WIN_SRC_DIRS)

INC_DIRS=inc inc/ui inc/app $(SCI)/include \
	$(SCI)/win32 $(SCI)/src $(SRC_DIRS)
INCS=$(addprefix -I,$(INC_DIRS))

vpath %.m $(SRC_DIRS)
vpath %.mm $(SRC_DIRS)
vpath %.cpp $(SRC_DIRS)
vpath %.cxx $(SRC_DIRS)
vpath %.c $(SRC_DIRS)
vpath %.xc $(SRC_DIRS)
vpath %.rc $(SRC_DIRS) res/icons/from_www.clker.com

vpath %.h $(INC_DIRS)
vpath %.h res/icons/from_www.clker.com


test: all
	$(APP) tests/assign1.c 45
	
dist:
### requires 7zip
	$(MAKE) -f win.mk clean
	$(MAKE) -f win.mk cleansci
	$(MAKE) -f win.mk --quiet RELEASE=yes STATIC=yes all
	-rm CodeAssistor-$(VERSION)-winEXE-$(RLC).zip
	-cp build\win\CodeAssistor.exe .
	-7z a CodeAssistor-$(VERSION)-winEXE-$(RLC).zip CodeAssistor.exe
	-rm CodeAssistor.exe
	

install: all
	-rm -rf c:\tools\$(APP)
	-cp $(APP) c:\tools\

all: 
	$(MAKE) -f win.mk $(BLD)
	$(MAKE) -f win.mk $(APP)

clean:
	-rm -rf $(BLD)
	
cleansci:
	cd $(SCI)/win32 && $(MAKE) clean

$(APP): $(BLD) $(EXE)

$(BLD): scintilla
	-mkdir $(BLDA)
	-mkdir $(BLD)
	-cp $(SCI)/bin/*.dll $(BLD)
	
scintilla:
	-mkdir $(SCI)/bin
	cd $(SCI)/win32 && $(MAKE) $(SCINTILLA_DEFS)
	@echo finished scintilla	

ifndef STATIC
$(EXE): $(BLD)/libapp.a $(BLD)/libwin.a $(BLD)/libcontrol.a $(BLD)/libcommand.a $(BLD)/resource.o
	$(CPP) -o $(BLD)/$(EXE) -lmingw32 $^ -lshlwapi -lcomctl32 -lcomdlg32 -lgdi32 -Wl,-subsystem,$(SUBSYS)
else
$(EXE): $(BLD)/libapp.a $(BLD)/libwin.a $(BLD)/libcontrol.a  $(BLD)/libcommand.a $(BLD)/resource.o
	$(CPP) -o $(BLD)/$(EXE) -L$(SCI)/bin/ -lmingw32 $^ -lsci -lshlwapi -lcomctl32 -lcomdlg32 -lgdi32 -Wl,-subsystem,$(SUBSYS) $(STRIP) -mwindows -lcomctl32 -limm32 -lole32 -luuid
endif

$(BLD)/%.o : %.xc
	$(C_CPP) $(COMPILE)

$(BLD)/MyEditor.o: MyEditor.cpp MyEditor.h resource.h BaseEditor.h

include common.mk