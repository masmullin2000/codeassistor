NAME=CodeAssistor

MAKE_DEFS:=

ifdef CLANG
MAKE_DEFS=CLANG=yes
CC=clang -fsanitize=address
else
CC=gcc
endif
C_C=$(CC) -x c++ $(gDEFs) $(INCS)
C_CPP=$(CC) -x c++ $(gDEFs) $(INCS)

AR=ar rcs

gDEFs=-DSCI_NAMESPACE -DSCI_LEXER -DGTK
gDEFs+=-DLINUX -DMASMULLIN_CHANGE
SCI=../scintilla

ifdef DBG
OPT_OR_DBG=-g -O0 -Wall -Werror -Wno-write-strings
gDEFs+=-DDEBUG
MAKE_DEFS+= DEBUG=yes
else
OPT_OR_DBG=-Os -Wall -Werror -Wno-write-strings
endif

ifdef ALLLEX
MAKE_DEFS+= ALLLEX=yes
endif

ifdef GTK3
GTKVERSION=gtk+-3.0
gDEFs+=-DGTK3
MAKE_DEFS+= GTK3=yes
LIB_UNIQUE=-lunique-3.0
else
GTKVERSION=gtk+-2.0
LIB_UNIQUE=-lunique-1.0
endif

### from all dependencies, compile the target ###
COMPILE=-c $< -o $@ $(OPT_OR_DBG) `pkg-config --cflags $(GTKVERSION)`

BLDA=build
BLDB=lin
BLD=$(BLDA)/$(BLDB)

EXE=$(NAME)
APP=$(BLD)/$(EXE)
STARTAPP=LinAppStart

UNIQUE_H=/usr/include/unique-1.0

SRC_DIRS=src src/app src/ui src/ui/SciControl
LIN_SRC_DIRS=src/app/lin src/ui/lin res/ui/lin
SRC_DIRS+=$(LIN_SRC_DIRS)

INC_DIRS=inc inc/ui inc/app $(SCI)/include \
	$(SCI)/gtk $(SCI)/src $(SRC_DIRS) $(BLD) \
	$(UNIQUE_H)
	
ifdef CLANG
INC_DIRS+=/usr/include/c++/4.5/i686-linux-gnu
endif

INCS=$(addprefix -I,$(INC_DIRS))

vpath %.m $(SRC_DIRS)
vpath %.mm $(SRC_DIRS)
vpath %.cpp $(SRC_DIRS)
vpath %.cxx $(SRC_DIRS)
vpath %.c $(SRC_DIRS)
vpath %.xc $(SRC_DIRS)
vpath %.xpm $(BLD)

vpath %.h $(INC_DIRS)

test: all
	./$(APP) tests/assign1.c 54 &

all: 
	$(MAKE) -f lin.mk $(BLD)
	$(MAKE) -f lin.mk $(APP)

clean: cleanca cleansci

cleanca:
	-rm -rf $(BLD)
	
cleansci:
	cd $(SCI)/gtk && $(MAKE) clean
	
dist:
	$(MAKE) -f lin.mk clean
	$(MAKE) -f lin.mk cleansci
	$(MAKE) -f lin.mk all --quiet
	-rm $(NAME)-$(VERSION)-linExe-$(RLC).zip
	-cp -r $(APP) .
	-zip $(NAME)-$(VERSION)-linExe-$(RLC).zip -r $(NAME)
	-rm -rf $(NAME)

$(APP): $(BLD) $(EXE)

$(BLD): scintilla
	-mkdir $(BLDA)
	-mkdir $(BLD)
	-cp $(SCI)/bin/scintilla.a $(BLD)

scintilla:
	-mkdir $(SCI)/bin
	cd $(SCI)/gtk && $(MAKE) $(MAKE_DEFS)
	@echo finished scintilla

$(EXE): $(BLD)/libapp.a $(BLD)/liblin.a $(BLD)/libcontrol.a $(BLD)/libcommand.a $(SCI)/bin/scintilla.a
	$(CC) -o $(BLD)/$(EXE) $^ `pkg-config --libs $(GTKVERSION)` $(LIB_UNIQUE) -lgmodule-2.0 -lm -lstdc++

$(BLD)/%.o : %.xc
	$(C_CPP) $(COMPILE)

$(BLD)/MyEditor.o: MyEditor.cpp MyEditor.h MyEditorCallbacks.h BaseEditor.h \
	$(BLD)/bookcoloured.xpm

include common.mk