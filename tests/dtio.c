/**********************************************************
* dtio.c - direct-terminal I/O functions supporting both  *
*          Borland C, AIX cc, and GNU/LINUX gcc platforms.*
*          A program that wants to use these should:      *
*          #include "dtio.h"                              *
*                                                         *
* AUTHOR:          Michael Mullin (with code from         *
*                  Evan Weavers btp300 website            *
* DATE MODIFIED :  October 27 2004                        *
**********************************************************/

#include "dtio.h"
#include <string.h>
#include <stdlib.h>
#if PLATFORM == AIX
	#include <curses.h>
#elif PLATFORM == LINUX
	#include <ncurses.h>
#elif PLATFORM == BORLAND
	#include <stdio.h> /* for the beep */
	#include <conio.h>
#endif

/* AIX version has no sound because we are over a network
*  Live LINUX (not over the network) and BORLAND do    */
void dt_beep(void)
{
#if PLATFORM == LINUX
	beep();
#elif PLATFORM == BORLAND
	putchar('\a');
#endif
}

/* Initialize. */
void dt_start(void)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, 1);
#endif
}

/* Shutdown */
void dt_stop(void)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	refresh();
	endwin();
#elif PLATFORM == BORLAND
	/* we don't want the cursor left in the
	   middle of a screen. curses/ncurses endwin() handles
	   this for us, but on the PC we must take care
	   of this. Clearing the screen is an easy way. */
	clrscr();
#endif
}

/* Return number of screen rows */
int dt_rows(void)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	return LINES;
#elif PLATFORM == BORLAND
	struct text_info x;
	gettextinfo(&x);
	return x.screenheight;
#endif
}

/* Return number of screen columns */
int dt_columns(void)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	return COLS;
#elif PLATFORM == BORLAND
	struct text_info x;
	gettextinfo(&x);
	return x.screenwidth;
#endif
}

/* Clear screen */
void dt_clear(void)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	erase();
#elif PLATFORM == BORLAND
	clrscr();
#endif
}

/* Bring screen up-to-date. Note that since dt_stop() and
*  dt_getchar() both bring the screen up-to-date, programs
*  will only have to call this if the screen must be brought
*  up-to-date when a long pause (other than waiting for 
*  input) is expected. 
*/
void dt_flush(void)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	refresh();
#endif
}

/* Return one keystroke, bringing screen up-to-date first */
int dt_getchar(void)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	refresh();
	return getch();
#elif PLATFORM == BORLAND
	/* For extended keys in DOS, return DOS_INC + second key code */
	int key;
	key = getch();
	return key == 0 ? DOS_INC + getch() : key;
#endif
}

/* Move cursor. (0, 0) is the upper-left corner */
void dt_cursor(int row, int column)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	move(row, column);
#elif PLATFORM == BORLAND
	gotoxy(column + 1, row + 1);
#endif
}

/* Output one character at cursor location */
void dt_putchar(int c)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	addch(c);
#elif PLATFORM == BORLAND
	putch(c);
#endif
}

/* Output character string at cursor location */
void dt_puts(char *s)
{
#if PLATFORM == AIX || PLATFORM == LINUX
	addstr(s);
#elif PLATFORM == BORLAND
	cputs(s);
#endif
}

/* place a string in a length-sized field at a defined place
 * on the screen, s is the string we are displaying 
 * row and column are position index offsets from the upper left
 * corner of the screen, these offsets are zero based and count
 * down and right respectively */
void dt_display(const char *s, int row, int column, int length)
{
	int i, rightMost; /* counter and rightmost character */

	if( dt_columns() > column && dt_rows() >= row )
	{
		dt_cursor(row,column); /* get to the right spot */

		/* if length is in 'lazy' mode
		 * rightmost is length of s */
		rightMost = column + length;
		if( length <= 0 )
			rightMost = column + strlen(s);

		/* if the rightmost char in the string is less
		 * than the number of columns, (ie the string doesn't
		 * flow over the edge) then we output the string as
		 * the specified length */
		if( rightMost <= dt_columns() )
			if(length <= 0)
				for(i = 0; s[i] != '\0'; i++)
					dt_putchar(s[i]);
			else
				for(i = 0; i < length; i++)
					if(s[i] == '\0')
						/* end of string means spaces 
						 * and never go back to line
						 * NORMAL via using same 
						 * counter */
						for(i; i < length; i++)
							dt_putchar(' ');
					else
						dt_putchar(s[i]); /* NORMAL */
		/* if the rightmost char is greater than the total number
		 * of columns (ie overflow), then we should only display
		 * the part of the string that fits */
		else
			for(i = 0; i < dt_columns() - column; i++ )
				dt_putchar(s[i]); /* line NORMAL */
	}       
}

/* dt_edit lets programmer using this library open up a 
 * 'field' where data can be entered
 * s is the string, row and col are indexes from the upper left
 * corner of the screen (zero based counting down and right respectively)
 * flen is the desired field lenth, <= 0 is 'lazy' mode where field is the
 * length of the string being passed, slen is the maximum length of s
 * ppos is the position offset from the start of the field
 * poff is the offset position from the start of the string
 * RETURN VALUE: the key pressed to escape editing */
int dt_edit(char *s, int row, int col, int flen, int slen, int *ppos,
	int *poff)
{
	/* __initializations__ */
	char *origString;
	int uInp, offset, position, i, exit = 0;
	static int displayMode = 0; /* static to keep insert vs overwrite */

	if( dt_columns() >= col && dt_rows() >= row )
	{
		/* position handling should be done with integers *
		 * rather than pointers to integers, this eliminates *
		 * the NULL problem (cannot add to null) */
		offset = ( poff == NULL || *poff <= 0 ) ? 0 : *poff;
		position = ( ppos == NULL || *ppos <= 0 ) ? 0 : *ppos;

		/* an offset of 0 means start, and offset of 1 also means *
		 * start (first position) a negative offset means 0 */
		if( offset != 0 )
			offset--;
		if( offset < 0 )
			offset = 0;
	
		/* lazy mode field length */
		if( flen <= 0 )
			flen = strlen(s);

		/* fix if field length is off right */
		if( col + flen >= dt_columns() )
			flen = dt_columns() - col;

		if( slen <= 0 )
			slen = flen;

		/* if garbage parameters position the cursor beyond the string
		 * then we put the cursor in a suitable position if necessary
		 * we put the offset into a suitable position */
		if( offset + position > slen )
			if( offset > slen )
			{
				offset = slen;
				position = 0;
			}else
				position = slen - offset;  

		if( position > flen )
			position = flen;

		/* copy the original string in case of ESC_KEY pressed */
		origString = (char*)malloc(sizeof(char) * strlen(s) + 1);
		strcpy(origString, s);

		/* initial display of field */
		dt_display( s + offset, row, col, flen );

		/* end __initializations__ */
		do
		{

			dt_cursor( row, col + position );
			uInp = dt_getchar(); /* get the users keypress */

			/* if we get a printable char, display it at cursor,
			 * and move string if in insert mode */
			if( uInp >= ' ' && uInp <= '~' )
			{
				/* insert data */
				if( displayMode == INS_MODE )
				{
					if( offset + position > strlen(s) )
					{
						for( i = strlen(s); i < offset
							+ position;i++ )
							s[i] = ' ';

						s[i] = '\0';
						/* NULL will be moved to
						 * the correct spot */
					}       

					for(i = slen - 1; i > offset 
						+ position; i--)
						s[i] = s[i-1];

					/* insert user data into string */
					if( offset + position < slen )
						s[i] = uInp; 

					/* lets just be safe here */
					s[slen] = '\0'; 

					if(( position + col == dt_columns() - 1 
						|| position == flen ) && 
						offset + position != slen )
						++offset;
					else if( position < flen &&
						offset + position != slen )
						position++;        

					dt_display(s + offset, row, col, flen);
				/* overwrite data */
				}else if( displayMode == OVR_MODE )
				{
					if( offset + position > strlen(s) )
					{
						for( i = strlen(s); i < offset
							+ position + 1; i++ )
							s[i] = ' ';
						s[i] = '\0';
					}else if( offset + position
						== strlen(s) )
						s[offset + position + 1]
							= '\0';

					if( position + offset < slen )
					{
						s[offset + position] = uInp;

						if( position + col == 
							dt_columns() - 1 
							|| position == flen &&
							offset + position
							!= slen )
							++offset;
						else if( position < flen &&
							offset + position 
							!= slen )
							position++;
					}
					dt_display(s + offset, row, col, flen);
				}
			}
			else /* special characters */
			{
				switch(uInp)
				{
				case HOME_KEY: /* home key */
					offset = 0;
					position = 0;
					dt_display(s + offset, row, col, flen);
					dt_cursor(row, col + position);
					break;
				case END_KEY: /* end key */
					if( strlen(s) >= flen )
					{
						offset = strlen(s) - flen;
						position = strlen(s) - offset;
					}else if( strlen(s) == flen )
					{
						offset = strlen(s)-(flen - 1);
						position = strlen(s) - offset;
					}else
					{
						offset = 0;
						position = strlen(s);
					}
					if( position + col == dt_columns() )
					{
						--position;
						offset++;
					}
	
					dt_display(s + offset, row, col, flen);
					dt_cursor(row, col + position);
					break;                          
				case DEL_KEY: /* delete key */
					if( position <= 0 )
						i = offset;
					else
						i = position + offset;
					
					while( s[i] != '\0' && i < strlen(s) )
						s[i] = s[i++ +1];       

					dt_display(s + offset, row, col, flen);
					dt_cursor( row, col + position );
					break;
				case BS_KEY: /* Backspace Key */
					if( position <= 0 && position >
						(-1) * offset)
					{
						--offset;
						i = offset;
						while( s[i] != '\0' &&
							i < strlen(s) )
							s[i++] = s[i+1];        

						dt_display( s + offset, row,
							col, flen);
						dt_cursor( row, col+position );
					}else if( position > 0 )
					{
						--position;
						i = offset + position;
						while( s[i] != '\0'
							&& i < strlen(s) )
							s[i++] = s[i+1];

						dt_display(s + offset, row,
							col, flen);
						dt_cursor( row, col+position );
					}                                               
					else dt_beep();
					
					break;
				case LEFT_KEY: /* left arrow key */
					if( position <= 0 && position >
						(-1) * offset)
					{
						dt_display(s + --offset, row,
							col, flen);
						dt_cursor( row, col+position );
					}else if( position > 0 )
						dt_cursor( row, col + 
							--position );
					else dt_beep();
					
					break;
				case RIGHT_KEY: /* right arrow key */
					if( offset + position > strlen(s) )
						s[offset + position] = '\0';

					if( offset + position == slen )
						dt_beep();
					else if( position < flen  )
					{
						++position;
						if( position + col
							> dt_columns() - 1 )
						{
							offset++;
							--position;
						}
					}else if( position == flen )
						++offset;

					dt_display(s + offset,row,col, flen);
					break;
				case INS_KEY: /* insert key */
					displayMode = displayMode ? 0 : 1;
					break;  
				case ESC_KEY: /* escape key */
					strcpy(s,origString);
					exit = 1;
					break;
				default: /* Escape Characters ie ENTER_KEY */
					if( poff != NULL )
						*poff = offset + 1;

					if( ppos != NULL )
						*ppos = position;

					exit = 1;
					break;
				}
			}
			
		}while(!exit);

		free(origString); /* free up the dynamic memory */
		origString = NULL; /* NULL our pointer */
	}else /* we got too many cols or too many rows return 'false' */
		uInp = 0;

	return uInp;
}
