/**********************************************************
* dtio.h - header file for direct-terminal I/O functions  *
*          supporting both Borland C, AIX cc platforms    *
*          and LINUX gcc platforms                        *
* AUTHOR:        Michael Mullin ( with code taken from    *
*                Evan Weavers btp300 website)             *
* DATE MODIFIED: Sunday October 27 / 2004                 *
* NOTE: to change between platforms the #define PLATFORM  *
*       line must be changed.  Currently 3 platforms are  *
*       supported by this code these are                  *
* MICROSOFT WINDOWS BORLAND C 5.5:#define PLATFORM BORLAND*
* IBM AIX cc:                     #define PLATFORM AIX    *
* SUSE LINUX:                     #define PLATFORM LINUX  *
**********************************************************/

#ifndef _dtio_h_
#define _dtio_h_

#define DOS_INC                 1000
#define INS_MODE                0
#define OVR_MODE                1

#define BORLAND                 1
#define AIX                     2
#define LINUX                   3
/* change the following line to support one of the above */
#define PLATFORM LINUX

/* some platform-dependent keys (obtained by experimentation) */
#if PLATFORM == AIX
	#define ENTER_KEY       10
	#define UP_KEY          1859
	#define DOWN_KEY        1858
	#define LEFT_KEY        1860
	#define RIGHT_KEY       1861
	#define BS_KEY          8
	#define ESC_KEY         27
	#define HOME_KEY        1862
	#define END_KEY         1960
	#define INS_KEY         1931
	#define DEL_KEY         1930
	#define PGUP_KEY        1939
	#define PGDN_KEY        1938
	#define TAB_KEY         9
	#define F1_KEY          1865
	#define F2_KEY          1866
	#define F3_KEY          1867
	#define F4_KEY          1868
	#define F5_KEY          1869
	#define F6_KEY          1870
	#define F7_KEY          1871
	#define F8_KEY          1872
	#define F9_KEY          1873
	#define F10_KEY         1874
	#define F11_KEY         1875
	#define F12_KEY         1876
/* note some variations of linux including the authors Gentoo Linux *
 * have slight variations of TERMINFO backspacing. We are coding for*
 * the matrix network environment with the puttyxterm created by    *
 * Evan Weaver                                                     */
#elif PLATFORM == LINUX
	#define BS_KEY          263
	#define ESC_KEY         27
	#define DEL_KEY         330
	#define HOME_KEY        262
	#define END_KEY         360
	#define ENTER_KEY       10
	#define UP_KEY          259
	#define DOWN_KEY        258
	#define LEFT_KEY        260
	#define RIGHT_KEY       261
	#define INS_KEY         331
	#define F1_KEY          265
	#define F2_KEY          266
	#define F3_KEY          267
	#define F4_KEY          268
	#define F5_KEY          269
	#define F6_KEY          270
	#define F7_KEY          271
	#define F8_KEY          272
	#define F9_KEY          273
	#define F10_KEY         274
	#define F11_KEY         275
	#define F12_KEY         276
	#define PGUP_KEY        339
	#define PGDN_KEY        338
	#define TAB_KEY         9
#elif PLATFORM == BORLAND
	#define BS_KEY          8
	#define ESC_KEY         27
	#define ENTER_KEY       13
	#define HOME_KEY        1071
	#define UP_KEY          1072
	#define DOWN_KEY        1080
	#define LEFT_KEY        1075
	#define RIGHT_KEY       1077
	#define END_KEY         1079
	#define INS_KEY         1082
	#define DEL_KEY         1083
	#define PGUP_KEY        1073
	#define PGDN_KEY        1081
	#define TAB_KEY         9
	#define F1_KEY          1059
	#define F2_KEY          1060
	#define F3_KEY          1061
	#define F4_KEY          1062
	#define F5_KEY          1063
	#define F6_KEY          1064
	#define F7_KEY          1065
	#define F8_KEY          1066
	#define F9_KEY          1067
	#define F10_KEY         1068
	#define F11_KEY         1133
	#define F12_KEY         1134
#endif

void dt_beep();       /* beep */
void dt_start(void);  /* initializations for dt routines */
void dt_stop(void);   /* shutdown of dt routines */
int dt_rows(void);    /* find # of rows of screen */
int dt_columns(void); /* find # of columns of screen */
void dt_clear(void);  /* clear screen */
void dt_flush(void);  /* flush any un-written output */
int dt_getchar(void); /* get one key press */
void dt_cursor(int row, int column); /* move cursor */
void dt_putchar(int c);/* output one character */
void dt_puts(char* s); /* output a string */
void dt_display(const char* s, int row, int column, int length);

/* output string at desired spot & length */
int dt_edit(char* s, int row, int col, int flen,
	int slen, int* ppos,int* poff);
	
#endif 
/* end _dtio_h_ */
