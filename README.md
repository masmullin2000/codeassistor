**** CodeAssistor ****

Version 0.0.7

*** Introduction ***

CodeAssistor is a simple source editor usable on MacOSX, Windows and GTK/Linux. CodeAssistor's primary feature is simplicity and ease of use.

*** Required Software Packages ***

You must have Xcode installed on your Macintosh computer in order to get the necessary compile tools (eg gcc)
You must have an up-to-date 'sed' executable.  (I suggest getting sed via the fink project -> http://www.finkproject.org/).  This executable must be the first sed in your $PATH.

You must have TDM mingw version 4.5 installed on your Windows computer.

You must have GTK+2.x-Developer, libunique, GCC, and ImageMagick installed on your Linux computer.

Scintilla (copyright Neil Hodgson: http://www.scintilla.org) is the editor component used by CodeAssistor.

As of this writing, CodeAssitor utilizes patches provided by Michael Mullin and not yet integrated into the main scintilla source tree. You can get the source code for scintilla + Michael's patches @ ( https://code.launchpad.net/~masmullin/scintilla-cocoa/mpatch )
NOTE: bzr is required to download the sourcecode (http://bazaar.canonical.com/en/)

You can get the main branch of the scintilla source code @ ( http://sourceforge.net/projects/scintilla/develop )

*** Compilation ***
All done in a Terminal Window.
** source code retrieval **

bzr branch lp:~masmullin/scintilla-cocoa/mpatch scintilla

bzr branch lp:codeassistor CodeAssistor

** compile **

LIN: cd CodeAssistor && make -f lin.mk all
MAC: cd CodeAssistor && make -f mac.mk all
WIN: cd CodeAssistor && make -f win.mk all
OR for a static build (no dlls) on windows
WIN: cd CodeAssistor && make -f win.mk all STATIC=yes

Note clang+llvm is supported on Mac and Windows, but is not recommended

** Installation **

Macintosh APP can be run from anywhere.  I suggest Drag and Dropping the build/CodeAssistor.app into /Applications

Windows exe can be run from anywhere.  I suggest installing a c:\tools directory and adding c:\tools to your %PATH% then copying build/CodeAssistor.exe into c:\tools

Linux exe can be run from anywhere.  I suggest cp build/CodeAssistor /usr/bin/CodeAssistor

You can associate .c .cpp .cxx .h .m .mm .txt .java .xc perl php etc files to open using CodeAssistor by CTRL+CLICKing on the file, selecting "Get Info" and selecting "CodeAssistor.app" under the "Open with:" section.  Press the "Change All..."

** running **

On Mac, Double Click the CodeAssistor.app.  Or double click the source code file you wish to edit if you have associated the filetype to CodeAssistor.app

On Windows or Linux, you can Double click the executable, or run from the commandline.
Windows and Linux allows you to position the cursor at a specific line by simply typing the line number you wish to jump to after the filename. 

eg "CodeAssistor.exe helloworld.c 3" will jump to line 3 of the helloworld.c file.