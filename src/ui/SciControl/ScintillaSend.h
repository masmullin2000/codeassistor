#ifndef __SCI_SETTERS_GETTERS__
#define __SCI_SETTERS_GETTERS__

#ifndef SCI_COLOURISENOUPDATE
#define SCI_COLOURISENOUPDATE SCI_COLOURISE
#endif

class ScintillaSend
{
private:
  static ScintillaSend* _me;
protected:
public:
  static ScintillaSend* getInstance();

  // set general
  long set
  ( 
    void* sci,
    int   property,
    int   param0,
    int   param1
  );

  // set reference
  long set
  (
    void* sci,
    int   property,
    int   param0,
    void* param1
  );

  // set string/colour
  long set
  (
    void* sci,
    int   property,
    int   param0,
    char* param1,
    bool  isColour = false
  );

  // set lexer
  long set
  (
    void* sci,
    int   property,
    char* param0,
    char* param1
  );

  int get
  (
    void* sci,
    int   property,
    int   value = -2,
    int   extraV = -2
  );
};

#endif

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
