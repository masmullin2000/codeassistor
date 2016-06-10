/********************************************************
* screen.cxx - Classes wrapping the direct terminal i/o *
*              functions that were created in assign 1  *
* Author:      Michael Mullin                           *
* Date Modified: Thursday November 25nd 2004            *
********************************************************/

#include "screen.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// helper function find the digits in a number
int digInt( int val )
{
	int count = 0;
	if( val < 0 )
		val *= -1;

	while( val > 0 )
	{
		count++;
		val /= 10;
	}
	return count;

}

// find the maximum # of digits an int can take up
// on this machine
int maxDig()
{
	int highestint = 0;

	for(int i = 0; i< sizeof(int)*8; i++ )
	{
		highestint |= 1;
		highestint <<= 1;
	}

	return digInt( highestint );
}

/* START OF BOX */
// empty box
box::box() : field(0,0,0,"",false)
{
	hi = 0;
	wide = 0;
	strcpy(edge,"+-+|+-+|");
}

// data filled box
box::box(int row, int col, int width, int height,const char *s)
	: field(row,col,0,"",false)
{
	if( s != NULL && strlen(s) == 8 )
		strcpy(edge,s);
	else
		strcpy(edge,"+-+|+-+|");

	hi = height;
	if( hi <= 0 )
		hi = 0;
	else if( getRow() + hi >= dt_rows() )
		hi = dt_rows() - getRow();

	wide = width;
	if( wide <= 0 )
		wide = 0;
	else if( getCol() + wide >= dt_columns() )
		wide = dt_columns() - getCol();

}

// copy constructor
box::box( const box& b )
{
	field::operator=( field(b.getRow(),b.getCol(),0) );
	strncpy(edge,b.edge,8);

	if( strlen(edge) != 8 )
		strcpy(edge,"+-+|+-+|");
	
	hi = b.hi;

	if( hi <= 0 )
		hi = 1;
	else if( getRow() + hi >= dt_rows() )
		hi = dt_rows() - getRow();

	wide = b.wide;

	if( wide <= 0 )
		wide = 1;
	else if( getCol() + wide >= dt_columns() )
		wide = dt_columns() - getCol();
}

// assignment operator
box& box::operator=( const box& b )
{
	field::operator=( field(b.getRow(),b.getCol(),0) );

	strcpy(edge,b.edge);
	if( strlen(edge) != 8 )
		strcpy(edge,"+-+|+-+|");

	hi = b.hi;

	if( hi <= 0 )
		hi = 1;
	else if( getRow() + hi >= dt_rows() )
		hi = dt_rows() - getRow();


	wide = b.wide;

	if( wide <= 0 )
		wide = 1;
	else if( getCol() + wide >= dt_columns() )
		wide = dt_columns() - getCol();

	return *this;
}

// display in 2D
void box::display()
{
	if( strlen(edge) != 8 )
		strcpy(edge,"+-+|+-+|");

	char* topbot = NULL;
	int j;
	topbot = new char[wide+1];
	if( wide == 1 ) // if width is only one, only left side
		for( int i = getRow(); i < getRow() + hi; i++ )
		{
			dt_cursor(i,getCol());
			if( i == getRow() )
				dt_putchar( edge[0] );
			else if( i != getRow() + hi - 1)
				dt_putchar( edge[7] );
			else
				dt_putchar( edge[6] );
		}
	else if(wide == 2) // if width is two, no space in between
	{
		for(int i = getRow(); i < getRow() + hi; i++ )
		{
			if( i == getRow() )
			{
				topbot[0] = edge[0];
				topbot[1] = edge[2];
			}else if( i != getRow() + hi - 1)
			{
				topbot[0] = edge[7];
				topbot[1] = edge[3];
			}else
			{
				topbot[0] = edge[6];
				topbot[1] = edge[4];
			}
			topbot[2] = '\0';
			dt_display(topbot,i,getCol(),wide);
		}
	}else if ( wide > 1 ) // space in between left and right edges
			      // put on either side of the space but
			      // do not touch in between
	{
		for(int i = getRow(); i < getRow() + hi; i++ )
		{
			if( i == getRow() )
			{
				topbot[0] = edge[0];
				for( j = 1; j < wide -1; j++ )
					topbot[j] = edge[1];
				
				topbot[j] = edge[2];
				topbot[j+1] = '\0';
				dt_display(topbot,i,getCol(),wide);
			}else if( i != getRow() + hi -1)
			{
				dt_cursor(i,getCol());
				dt_putchar( edge[7] );
				dt_cursor(i,getCol() + wide - 1);
				dt_putchar( edge[3] );
			}else
			{
				topbot[0] = edge[6];
				for( j = 1; j < wide -1; j++ )
					topbot[j] = edge[5];

				topbot[j] = edge[4];
				topbot[j+1] = '\0';
				dt_display(topbot,i,getCol(),wide);
			}
		}
	}
	// deallocate topbot
	if(topbot) delete [] topbot;
}

// make sure we display the correct function
int box::edit()
{
	display();
	return 0;
}

// the data of a box is its edges
void* box::data()
{
	return (void*)edge;
}

// the data of a box is its edges, no touchy
void* box::getData() const
{
	return (void*)edge;
}

// clone to a field
field* box::clone() const
{
	box* b;
	b = new box( getRow(), getCol(), getWide(), getHi(), (char*)getData() );

	return b;
}

// clone to a box
box* box::cloneB() const
{
	box* b;
	b = new box( getRow(), getCol(), getWide(), getHi(), (char*)getData() );

	return b;
}

/* END OF BOX */ 

/* START OF MMIIELD */
// empty mmifield
mmifield::mmifield() : ivfield()
{
	mini = maxi = 0;
}

// data filled mmifield
mmifield::mmifield(int r, int c, int flen, int init, int slen,
	int min, int max, void (*help)(void *))
	: ivfield(r,c,flen,init,slen,NULL,help)
{
	mini = min;
	maxi = max;
}

// edit an mmifield
int mmifield::edit()
{
	int key;
	bool validate = false;
	do
	{
		key = ivfield::edit();

		// the value of the mmi must fit inside the mix/max
		if( key == ESC_KEY || ( getVal() <= maxi && getVal() >= mini) )
			validate = true;

	}while( !validate );

	return key;
}

// clone to an sfield
sfield* mmifield::cloneS() const
{
	mmifield* mmif;
	mmif = new mmifield(getRow(),getCol(),getLen(),getVal(),
		getSLen(),mini,maxi,getHelp());

	return mmif;
}

// clone to an hfield
hfield* mmifield::cloneH() const
{
	mmifield* mmif;
	mmif = new mmifield(getRow(),getCol(),getLen(),getVal(),
		getSLen(),mini,maxi,getHelp());
	return mmif;
}

// clone to an integer validation field
ivfield* mmifield::cloneIV() const
{
	mmifield* mmif;
	mmif = new mmifield(getRow(),getCol(),getLen(),getVal(),
		getSLen(),mini,maxi,getHelp());
	return mmif;
}

// clone to an mmifield
mmifield* mmifield::cloneMMI() const
{
	mmifield* mmif;
	mmif = new mmifield(getRow(),getCol(),getLen(),getVal(),
		getSLen(),mini,maxi,getHelp());

	return mmif;
}

// clone to a field
field* mmifield::clone() const
{
	mmifield* mmif;
	mmif = new mmifield(getRow(),getCol(),getLen(),getVal(),
		getSLen(),mini,maxi,getHelp());

	return mmif;
}

// construct from sfield
mmifield::mmifield( const sfield& sf )
{
	ivfield::operator=( ivfield(sf.getRow(),sf.getCol(),
		sf.getLen(),0,sf.getSLen(),NULL,NULL) );
	mini = 0;
	maxi = 0;
}

// assign from sfield
mmifield& mmifield::operator=( const sfield& sf )
{
	ivfield::operator=( ivfield(sf.getRow(),sf.getCol(),
		sf.getLen(),0,sf.getSLen(),NULL,NULL) );
	mini = 0;
	maxi = 0;
	
	return *this;
}

// copy from hfield
mmifield::mmifield( const hfield& hf )
{
	ivfield::operator=( ivfield(hf.getRow(),hf.getCol(),
		hf.getLen(),0,hf.getSLen(),NULL,hf.getHelp()) );
	mini = 0;
	maxi = 0;
}

// assign from hfield
mmifield& mmifield::operator=( const hfield& hf ) // assign from hfield
{
	ivfield::operator=( ivfield(hf.getRow(),hf.getCol(),
		hf.getLen(),0,hf.getSLen(),NULL,hf.getHelp()) );
	mini = 0;
	maxi = 0;
	return *this;
}

// copy from ivfield
mmifield::mmifield( const ivfield& ivf )
{
	ivfield::operator=( ivfield(ivf.getRow(),ivf.getCol(),
		ivf.getLen(),ivf.getVal(),ivf.getSLen(),NULL,ivf.getHelp()) );
	mini = 0;
	maxi = 0;
}

// assign from ivfield
mmifield& mmifield::operator=( const ivfield& ivf )
{

	ivfield::operator=( ivfield(ivf.getRow(),ivf.getCol(),
		ivf.getLen(),ivf.getVal(),ivf.getSLen(),NULL,ivf.getHelp()) );
	mini = 0;
	maxi = 0;

	return *this;
}

// copy constructor
mmifield::mmifield( const mmifield& mmif ) // copy constructor
{
	ivfield::operator=( ivfield(mmif.getRow(),mmif.getCol(),
		mmif.getLen(),mmif.getVal(),
		mmif.getSLen(),NULL,mmif.getHelp()) );
	mini = mmif.mini;
	maxi = mmif.maxi;

}

// assignment operator
mmifield& mmifield::operator=( const mmifield& mmif ) // assign op
{
	ivfield::operator=( ivfield(mmif.getRow(),mmif.getCol(),
		mmif.getLen(),mmif.getVal(),
		mmif.getSLen(),NULL,mmif.getHelp()) );
	mini = mmif.mini;
	maxi = mmif.maxi;

	return *this;
}

// fit the integer coming into val to min/max
mmifield& mmifield::operator=( int val )
{
	ivfield::operator=(val);
	if( val > maxi )
		val = maxi;

	if( val < mini )
		val = mini;

	return *this;
}

/* END OF MMIFIELD */

/* START OF DVFIELD */
// empty dvfield
dvfield::dvfield() : hfield()
{
	vd = NULL;
	initV = 0;
	dplace = 2;
}

// data filled dvfield
dvfield::dvfield(int r, int c, int flen, int places, double init,
     int slen, bool (*valid)(double &), void (*help)(void *) )
	: hfield(r,c,flen,"",slen,help)
{
	initV = init;
	dplace = places;
	int i = dplace;
	bool fits = false;

	/* if our display length is greater than 0 than we want
	   to check and see if we can fit the double into 
	   the scrollable length */
	if( flen > 0 )
		if( digInt( (int)initV ) + dplace + 1 > getSLen() )
		{
			if( digInt( (int)initV ) > getSLen() )
				initV = 0;
			else
			{
				do
				{
					if( digInt( (int)initV + i <= getSLen() ) )
						fits = true;
				}while( --i > 0 && !fits );
			}
			if( i > getSLen() )
				i = 0;
			sprintf((char*)hfield::data(),"%.*lf",i,initV);
		}else
			sprintf((char*)hfield::data(),"%.*lf",dplace,initV);

	vd = valid;
}

// display a dvfield
void dvfield::display()
{
	bool fits = false;
	int i = dplace;
	double orgVal = initV;
	/* make sure the data fits into the display parameters */
	if( getLen() > 0 )
		if( digInt( (int)initV ) + dplace + 1 > getSLen() )
		{
			if( digInt( (int)initV ) > getSLen() )
				initV = 0;
			else
			{
				do
				{
					if( digInt( (int)initV + i <= getSLen() ) )
						fits = true;
				}while( --i > 0 && !fits );
			}
			if( i > getSLen() )
				i = 0;
			sprintf((char*)hfield::data(),"%.*lf",i,initV);
		}else
			sprintf((char*)hfield::data(),"%.*lf",dplace,initV);

	hfield::display();
}

// edit the dvfield
int dvfield::edit()
{
	int key;
	bool validate;
	bool fits = false;
	int dotocc, i = dplace;

	double orgVal = initV;
	/* make sure we fit the display parameters */
	if( getLen() > 0 )
		if( digInt( (int)initV ) + dplace + 1 > getSLen() )
		{
			if( digInt( (int)initV ) > getSLen() )
				initV = 0;
			else
			{
				do
				{
					if( digInt( (int)initV + i <= getSLen() ) )
						fits = true;
				}while( --i > 0 && !fits );
			}
			if( i > getSLen() )
				i = 0;
			sprintf((char*)hfield::data(),"%.*lf",i,initV);
		}else
			sprintf((char*)hfield::data(),"%.*lf",dplace,initV);

	do
	{
		/* get user input as character of digits
		   if F1 was pressed, do checking to see if the display
		   needs to be updated */
		validate = true;
		key = sfield::edit();

		if( key == F1_KEY && getHelp() )
		{
			i = dplace;
			pt2FuncVOID help = getHelp();
			help( data() );
			validate = false;

			if( orgVal != initV || strlen((char*)hfield::data()) 
				!= digInt( (int)initV ) + dplace )
			{
				if( digInt( (int)initV ) + dplace + 1 > getSLen() )
				{
					if( digInt( (int)initV ) > getSLen() )
						initV = 0;
					else
					{
						do
						{
							if(digInt((int)initV + i
								<= getSLen()))  
							fits = true;
						}while( --i > 0 && !fits );
					}
					if( i > getSLen() )
						i = 0;
					sprintf((char*)hfield::data(),
						"%.*lf",i,initV);
				}else
				sprintf((char*)hfield::data(),"%.*lf",
					dplace,initV);
			}

		}
		dotocc = 0;

		if( key == ESC_KEY )
			initV = orgVal;
		else
		{
			char* editor = (char*)hfield::data();
			for(int i=0; i < strlen(editor); i++)
			{
				// count the occurances of . and -
				if( editor[i] == '.' )
					dotocc++;

				// get a valid double
				if( ( editor[i] > '9' || editor[i] < '0' )
					&& editor[i] != '.'
					&& dotocc < 2 )
				{
					if( i == 0 && editor[i] == '-' )
					       ;
					else
						validate = false;
				}else if( editor[i] == '.' && dotocc > 1 )
					validate = false;
			}

			// if we have passed initial validation
			// then we can do external validating
			if( validate )
			{
				initV = strtod( editor,NULL );

				if( vd != NULL )
					validate = vd(initV);
			}
		}
	}while( !validate );

	return key;
}

// clone to a dvfield
dvfield* dvfield::cloneDV() const
{
	dvfield* dvf;
	dvf = new dvfield(getRow(),getCol(),getLen(),
		dplace,initV,getSLen(),vd,getHelp());

	return dvf;
}

// clone to an sfield
dvfield::dvfield( const sfield& sf )
{
	hfield::operator=( hfield(sf.getRow(),sf.getCol(),
		sf.getLen(),"",sf.getSLen(),NULL ) );
	initV = 0;
	dplace = 2;
	vd = NULL;      
}

// assign from an sfield
dvfield& dvfield::operator=( const sfield& sf )
{
	hfield::operator=( hfield(sf.getRow(),sf.getCol(),
		sf.getLen(),"",sf.getSLen(),NULL ) );
	initV = 0;
	dplace = 2;
	vd = NULL;
	
	return *this;
}

// copy from an hfield
dvfield::dvfield( const hfield& hf )
{
	hfield::operator=( hfield(hf.getRow(),hf.getCol(),
		hf.getLen(),"",
		hf.getSLen(), hf.getHelp() ) );
	initV = 0;
	dplace = 2;
	vd = NULL;
}

// assign from an hfield
dvfield& dvfield::operator=( const hfield& hf )
{
	hfield::operator=( hfield(hf.getRow(),hf.getCol(),
		hf.getLen(),"",
		hf.getSLen(), hf.getHelp() ) );

	vd = NULL;
	dplace = 2;
	initV = 0;
	return *this;
}

dvfield::dvfield( const dvfield& dvf ) // copy constructor
{

	int i = dplace;
	bool fits = false;

	hfield::operator=( hfield(dvf.getRow(),dvf.getCol(),
		dvf.getLen(), (char*)dvf.hfield::getData(),
		dvf.getSLen(),dvf.getHelp()) );

	vd = dvf.vd;
	dplace = dvf.dplace;
	initV = dvf.initV;
	if( getLen() > 0 )
		if( digInt( (int)initV ) + dplace + 1 > getSLen() )
		{
			if( digInt( (int)initV ) > getSLen() )
				initV = 0;
			else
			{
				do
				{
					if( digInt( (int)initV + i <= getSLen() ) )
						fits = true;
				}while( --i > 0 && !fits );
			}
			if( i > getSLen() )
				i = 0;
			sprintf((char*)hfield::data(),"%.*lf",i,initV);
		}else
			sprintf((char*)hfield::data(),"%.*lf",dplace,initV);
}

dvfield& dvfield::operator=( const dvfield& dvf ) // assign op
{
	int i = dplace;
	bool fits = false;
	hfield::operator=( hfield(dvf.getRow(),dvf.getCol(),
		dvf.getLen(),(char*)dvf.hfield::getData(),
		dvf.getSLen(),dvf.getHelp() ) );

	vd = dvf.vd;
	dplace = dvf.dplace;
	initV = dvf.initV;
	if( getLen() > 0 )
		if( digInt( (int)initV ) + dplace + 1 > getSLen() )
		{
			if( digInt( (int)initV ) > getSLen() )
				initV = 0;
			else
			{
				do
				{
					if( digInt( (int)initV + i <= getSLen() ) )
						fits = true;
				}while( --i > 0 && !fits );
			}
			if( i > getSLen() )
				i = 0;
			sprintf((char*)hfield::data(),"%.*lf",i,initV);
		}else
			sprintf((char*)hfield::data(),"%.*lf",dplace,initV);

	return *this;
}

// clone to a field
field* dvfield::clone() const
{
	dvfield* dvf;
	dvf = new dvfield(getRow(),getCol(),getLen(),
		dplace,initV,getSLen(),vd,getHelp());

	return dvf;
}

// clone to an sfield
sfield* dvfield::cloneS() const
{
	sfield* sf;
	sf = new sfield( getRow(),getCol(),getLen(),
		(char*)hfield::getData(), getSLen() );

	return sf;
}

// clone to an hfield
hfield* dvfield::cloneH() const
{
	hfield* hf;
	hf = new hfield( getRow(),getCol(),getLen(),
		(char*)hfield::getData(),getSLen(), getHelp() );

	return hf;
}

// get the data as a void*
void* dvfield::data()
{
	return (void*)&initV;
}

// get the data as a void*, no touchy
void* dvfield::getData() const
{
	return (void*)&initV;
}

// assign from a double
dvfield& dvfield::operator=( double val )
{
	initV = val;
	return *this;
}

// cast to a double
dvfield::operator double()
{
	if( digInt( (int)initV ) + dplace > getSLen() )
		initV = 0;

	return initV;
}

// the the value of the dvfield
double dvfield::getVal() const { return initV; }
/* END OF DVFIELD */

/* START OF VFIELD */
// empty ivfield
ivfield::ivfield() : hfield()
{
	vd = NULL;
	initV = 0;
}

// data filled ivfield
ivfield::ivfield(int r, int c, int flen, int init, int slen,
	bool (*valid)(int &), void (*help)(void *))
	: hfield(r,c,flen,"",
	slen <= 0 ? flen : slen,help)
{
	initV = init;
	if( digInt( initV ) > getSLen() )
		initV = 0;

	if( flen > 0 )
		sprintf((char*)hfield::data(),"%d",initV);

	vd = valid;
}
// display the ivfield
void ivfield::display()
{
	int orgVal = initV;
	if( digInt( initV ) > getSLen() )
		initV = 0;

	if( getLen() > 0 )
		sprintf((char*)hfield::data(),"%d",initV);

	hfield::display();
}

// edit the ivfield
int ivfield::edit()
{
	int key;
	bool validate;

	int orgVal = initV;
	// make sure value fits
	if( digInt( initV ) > getSLen() )
		initV = 0;

	if( getLen() > 0 )
		sprintf((char*)hfield::data(),"%d",initV);
      
	do
	{
		validate = true;
		key = sfield::edit();

		// if the F1 key was pressed we much check to 
		// see if we need to update the display field
		if( key == F1_KEY && getHelp() )
		{
			pt2FuncVOID help = getHelp();
			help( data() );
			validate = false;
			if( digInt( initV ) > getSLen() )
				initV = 0;

			if( orgVal != initV ||
				strlen((char*)hfield::data()) != digInt( initV ) )
				sprintf((char*)hfield::data(),"%d",initV);
		}

		if( key == ESC_KEY )
			initV = orgVal;
		else
		{
			char* editor = (char*)hfield::data();
			for(int i=0; i < strlen(editor); i++)
			{

				// initial validation of the display 
				// field to make sure we have an integer
				if( editor[i] > '9' || editor[i] < '0' )
					if( i == 0 && editor[i] == '-' )
						validate = true;
					else
						validate = false;
			}

			// if we have an integer validate the integer
			if( validate )
			{
				initV = atoi( editor );

				if( vd != NULL )
					validate = vd(initV);
			}
		}
	}while( !validate );


	return key;
}

// clone to an ivfield
ivfield* ivfield::cloneIV() const
{
	ivfield* ivf;
	ivf = new ivfield(getRow(),getCol(),getLen(),initV,
		getSLen(),vd,getHelp());

	return ivf;
}

// clone to an sfield
ivfield::ivfield( const sfield& sf )
{
	hfield::operator=( hfield(sf.getRow(),sf.getCol(),
		sf.getLen(),(char*)sf.sfield::getData(),sf.getSLen(),NULL ) );
	initV = 0;
	vd = NULL;      
}

// assign from sfield
ivfield& ivfield::operator=( const sfield& sf )
{
	hfield::operator=( hfield(sf.getRow(),sf.getCol(),
		sf.getLen(),(char*)sf.sfield::getData(),sf.getSLen(),NULL ) );
	initV = 0;
	vd = NULL;
	
	return *this;
}

// copy from hfield
ivfield::ivfield( const hfield& hf )
{
	hfield::operator=( hfield(hf.getRow(),hf.getCol(),
		hf.getLen(),(char*)hf.hfield::getData(), hf.getSLen(),
		hf.getHelp() ) );
	initV = 0;
	vd = NULL;
}

// assign from hfield
ivfield& ivfield::operator=( const hfield& hf )
{
	hfield::operator=( hfield(hf.getRow(),hf.getCol(),
		hf.getLen(),(char*)hf.hfield::getData(),
		hf.getSLen(), hf.getHelp() ) );

	initV = 0;
	vd = NULL;
	return *this;
}

ivfield::ivfield( const ivfield& ivf ) // copy constructor
{
	hfield::operator=( hfield(ivf.getRow(),ivf.getCol(),
		ivf.getLen(), (char*)ivf.hfield::getData(),
		ivf.getSLen(), ivf.getHelp()) );

	vd = ivf.vd;    
	initV = ivf.initV;
	if( digInt( initV ) > getSLen() )
		initV = 0;

	sprintf((char*)hfield::data(),"%d",initV);
}
ivfield& ivfield::operator=( const ivfield& ivf ) // assign op
{
	hfield::operator=( hfield(ivf.getRow(),ivf.getCol(),
		ivf.getLen(), (char*)ivf.hfield::getData(),
		ivf.getSLen(),ivf.getHelp() ) );

	vd = ivf.vd;
	initV = ivf.initV;
	int orgVal = initV;
	if( digInt( initV ) > getSLen() )
		initV = 0;

	sprintf((char*)hfield::data(),"%d",initV);


	return *this;
}

// clone to a field
field* ivfield::clone() const
{
	ivfield* ivf;
	ivf = new ivfield(getRow(),getCol(),getLen(),initV,
		getSLen(),vd,getHelp());

	return ivf;
}

// clone to an sfield
sfield* ivfield::cloneS() const
{
	ivfield* ivf;
	ivf = new ivfield(getRow(),getCol(),getLen(),initV,
		getSLen(),vd,getHelp());

	return ivf;
}

// clone to an hfield
hfield* ivfield::cloneH() const
{
	ivfield* ivf;
	ivf = new ivfield(getRow(),getCol(),getLen(),initV,
		getSLen(),vd,getHelp());

	return ivf;
}

// get the data
void* ivfield::data()
{
	return (void*)&initV;
}

// get the data, no touchy
void* ivfield::getData() const
{
	return (void*)&initV;
}

// assign from an int
ivfield& ivfield::operator=( int val )
{
	initV = val;
	return *this;
}

// cast to an int
ivfield::operator int()
{
	if( digInt( initV ) > getSLen() )
		initV = 0;

	return initV;
}

// get the value of ivfield
int ivfield::getVal() const { return initV; }   

/* END OF IVFIELD */

/* START OF VFIELD */
// empty vfield
vfield::vfield() : hfield()
{
	vd = NULL;
}

// data filled vfield
vfield::vfield(int r, int c, int flen, const char *initstr,
	int slen, bool (*valid)(char *),
	void (*help)(void *) ) : hfield(r,c,flen,initstr,slen,help)
{
	vd = valid;
}

// edit a vfield and do validation
int vfield::edit()
{
	int key, validate = 0;
	char* origString;
	origString = new char[ strlen((char*)hfield::data()) + 1 ];
	strcpy(origString, (char*)hfield::data());
	do
	{
		key = hfield::edit();
		if( key == ESC_KEY || vd == NULL)
		{
			validate = true;
			if( key == ESC_KEY )
				strcpy((char*)hfield::data(),origString);
		}
		else
			validate = vd((char*)hfield::data());

	}while( !validate );

	return key;
}

// clone to a vfield
vfield* vfield::cloneV() const
{
	vfield* vf;
	vf = new vfield(getRow(),getCol(),getLen(),
		(char*)hfield::getData(),getSLen(),vd,getHelp());

	return vf;
}

// clone to an sfield
vfield::vfield( const sfield& sf )
{
	hfield::operator=( hfield(sf.getRow(),sf.getCol(),
		sf.getLen(),(char*)sf.sfield::getData(),
		sf.getSLen(),NULL ) );

	vd = NULL;
}

// assign from an sfield
vfield& vfield::operator=( const sfield& sf )
{
	hfield::operator=( hfield(sf.getRow(),sf.getCol(),
		sf.getLen(),(char*)sf.sfield::getData(),
		sf.getSLen(),NULL ) );

	vd = NULL;
	
	return *this;
}

vfield::vfield( const hfield& hf ) // copy from hfield
{
	hfield::operator=( hfield(hf.getRow(),hf.getCol(),
		hf.getLen(),(char*)hf.hfield::getData(),
		hf.getSLen(), hf.getHelp() ) );
	vd = NULL;
}

vfield& vfield::operator=( const hfield& hf ) // assign from hfield
{
	hfield::operator=( hfield(hf.getRow(),hf.getCol(),
		hf.getLen(),(char*)hf.hfield::getData(),
		hf.getSLen(), hf.getHelp() ) );

	vd = NULL;
	return *this;
}

vfield::vfield( const vfield& vf ) // copy constructor
{
	hfield::operator=( hfield(vf.getRow(),vf.getCol(),
		vf.getLen(), (char*)vf.hfield::getData(),
		vf.getSLen(), vf.getHelp()) );

	vd = vf.vd;
}
vfield& vfield::operator=( const vfield& vf ) // assign op
{
	hfield::operator=( hfield(vf.getRow(),vf.getCol(),
		vf.getLen(),(char*)vf.hfield::getData(),
		vf.getSLen(), vf.getHelp() ) );

	vd = vf.vd;

	return *this;
}

// clone to a field
field* vfield::clone() const
{
	vfield* vf;
	vf = new vfield(getRow(),getCol(),getLen(),
		(char*)hfield::getData(),getSLen(),vd,getHelp());

	return vf;
}

// clone to an sfield
sfield* vfield::cloneS() const
{
	vfield* vf;
	vf = new vfield(getRow(),getCol(),getLen(),
		(char*)hfield::getData(),getSLen(),vd,getHelp());

	return vf;
}

// clone to an hfield
hfield* vfield::cloneH() const
{
	vfield* vf;
	vf = new vfield(getRow(),getCol(),getLen(),
		(char*)hfield::getData(),getSLen(),vd,getHelp());

	return vf;
}

/* END OF VFIELD */

/* START OF HFIELD */
// empty hfield
hfield::hfield() : sfield()
{
	helper = NULL;
}

// data filled hfield
hfield::hfield(int r, int c, int flen, const char* initstr,
	int slen, void (*help)(void *)) :
	sfield(r,c,flen,initstr,slen)
{
	helper = help;
}

// edit an hfield
int hfield::edit()
{
	int key;
	char* origString;

	origString = new char[ strlen((char*)sfield::data()) + 1 ];
	strcpy(origString, (char*)sfield::data());

	// perform help when F1 is pressed
	do
	{
		key = sfield::edit();
		if( key == F1_KEY && helper != NULL )
		{
			helper(data());
		}
		else if( key == ESC_KEY )
			strcpy((char*)sfield::data(),origString);
		

	}while( key == F1_KEY && helper != NULL );

	return key;
}

// clone to an hfield
hfield* hfield::cloneH() const
{
	hfield* hf;
	hf = new hfield(getRow(),getCol(),getLen(),
		(char*)sfield::getData(),getSLen(),helper);
	
	return hf;
}

hfield::hfield( const sfield& sf) // copy from sfield
{
	sfield::operator=( sfield(sf.getRow(),sf.getCol(),
		sf.getLen(),(char*)sf.getData(), sf.getSLen()) );
	helper = NULL;
}

hfield& hfield::operator=( const sfield& sf) // assign from sfield
{
	sfield::operator=( sfield(sf.getRow(),sf.getCol(),
		sf.getLen(),(char*)sf.getData(), sf.getSLen()) );

	helper = NULL;
	return *this;
}

// copy constructor
hfield::hfield( const hfield& hf )
{
	sfield::operator=( sfield(hf.getRow(),hf.getCol(),
		hf.getLen(), (char*)hf.sfield::getData(), hf.getSLen())  );

	helper = hf.helper;
}

hfield& hfield::operator=( const hfield& hf ) // assign op
{
	sfield::operator=( sfield(hf.getRow(),hf.getCol(),
		hf.getLen(), (char*)hf.sfield::getData(), hf.getSLen()) );

	helper = hf.helper;
	return *this;
}

// clone to an sfield
sfield* hfield::cloneS() const
{
	hfield* hf;
	hf = new hfield(getRow(),getCol(),getLen(),
		(char*)sfield::getData(),getSLen(),helper);
	
	return hf;
}

// clone to a field
field* hfield::clone() const
{
	hfield* hf;
	hf = new hfield(getRow(),getCol(),getLen(),
		(char*)sfield::getData(),getSLen(),helper);
	
	return hf;
}

/* END OF HFIELD */

/* START OF SFIELD */
// empty sfield
sfield::sfield() : field()
{
	stlen = 0;
	content = new char[1];
	content[0] = '\0';

	ppos = poff = 0;
}

// destruct the dynamic memory
sfield::~sfield()
{
	if( content ) delete [] content;
	content = NULL;
}

// data filled sfield
sfield::sfield( int r, int c, int flen,
	const char *s, int slen ) : field(r,c,flen)
{
	ppos = 0;
	poff = 0;
	stlen = slen;
	flen = field::getLen();
	content = NULL;
	if( stlen <= 0 )        
		stlen = field::getLen();

	content = new char[stlen+1];
	if( strlen(s) > stlen )
		strncpy(content,s,stlen);
	else
		strcpy(content,s);

	content[stlen] = '\0';
}

// copy constructor
sfield::sfield( const sfield& sf )
{
	content = NULL;
	field::operator=( sf );
	stlen = sf.stlen;
	content = new char[stlen+1];
	strcpy(content,sf.content);
}

// copy from field
sfield::sfield( const field& f )
{
	content = NULL;
	*this = sfield( f.getRow(),f.getCol(),f.getLen(),
		(char*)f.getData(),0);
}

// assign from a field
sfield& sfield::operator=( const field& f )
{
	ppos = 0;
	poff = 0;
	stlen = f.getLen();

	// get the proper sizes into the field
	field::operator=( field(f.getRow(),f.getCol(),f.getLen()) );

	if( content ) delete [] content;
	content = new char[stlen+1];

	strncpy(content,(char*)f.getData(),stlen);

	return *this;
}

// assignment operator
sfield& sfield::operator=( const sfield& sf )
{
	ppos = 0;
	poff = 0;
	field::operator=( sf );
	stlen = sf.stlen;

	set( sf.content );

	return *this;
}

// set the content
void sfield::set( const char* s )
{
	if( content ) delete [] content;
	content = new char[stlen+1];

	if( strlen(s) > stlen )
		strncpy(content,s,stlen);
	else
		strcpy(content,s);

	content[stlen] = '\0';
}

// display the sfield 
void sfield::display()
{
	if( content )
// if we are using borland, we cannot place anything to the bottom right
// hand corner, try to display a shorter field if this is the case
// or if we are using ONLY the bottom right hand corner, dont display anything
#if PLATFORM == BORLAND
		if( field::getRow() == dt_rows() -1
			&& field::getCol() + field::getLen()
			>= dt_columns() -1)
		{
			if( field::getLen()-1 > 0 )
				dt_display(content,field::getRow(),
					field::getCol(),field::getLen()-1);
		}else
#endif
			if( poff > 0 )
				dt_display(content + poff -1,field::getRow(),
					field::getCol(),field::getLen());
			else
				dt_display(content,field::getRow(),
					field::getCol(),field::getLen());
			
}

// edit the field
int sfield::edit()
{
	int rc = 0;
	if( !content )
	{
		content = new char[stlen+1];
		content[0] = '\0';
	}

	if( strlen(content) || stlen )
		rc = dt_edit(content, field::getRow(), field::getCol(),
			field::getLen(), stlen, &ppos, &poff);

	return rc;
}

// get the address of the string
void* sfield::data()
{
	return (void*)content;
}

// get the address of the string, no touchy
void* sfield::getData() const
{
	return (void*)content;
}

// clone to an sfield
sfield* sfield::cloneS() const
{
	sfield* sf;
	sf = new sfield( getRow(),getCol(),getLen(),content,stlen );
	
	return sf;
}

// clone to a field
field* sfield::clone() const
{
	sfield* sf;
	sf = new sfield( getRow(),getCol(),getLen(),content,stlen );
	
	return sf;
}

// get the scrollable length
int sfield::getSLen() const { return stlen; }
/* END OF SFIELD */

/* START OF LABEL */
// construct a label size of label is the lenght of content
// if the length was given as 0
label::label( int row, int col, const char* s, int len )
	: field(row,col,
		len <= 0 ? strlen(s) : len,
		s,false)
{};

// clone to a field
field* label::clone() const
{
	label* l;
	l = new label( getRow(),getCol(),(char*)getData(),getLen() );
//        l = new label( getRow(),getCol(),getLen(),(char*)getData() );

	return l;
}

// clone to a label
label* label::cloneL() const
{
	label* l;
	l = new label( getRow(),getCol(),(char*)getData(),getLen() );
	
	return l;
}


/* START OF FIELD */

// field destructor, ensure content is clean
field::~field()
{
	if( content ) delete [] content;
	content = NULL;
}

// set to default empty
// used with the constructor
void field::set()
{

	row = 0;
	col = 0;
	len = 0;
	ppos = 0;
	poff = 0;

	// if we have content, kill the content
	if( content ) delete [] content;
	content = NULL;
	editAble = true;
}

// set to spot and length
void field::set(int r, int c, int l)
{
	set();

	if( r < 0 )
		r = 0;
	else if( r >= dt_rows() )
		r = dt_rows() -1;

	if( c < 0 )
		c = 0;
	else if( c >= dt_columns() )
		c = dt_columns() -1;

	if( l < 0 )
		l = 0;
	else if( c + l >= dt_columns() )
		l = dt_columns() -c;

	row = r;
	col = c;
	len = l;
	content = new char[len+1];
	content[0] = '\0';
}

// set to spot, length, and contents
void field::set(int r, int c, int l, const char* s)
{
	set(r,c,l);
 
	if( s )
	{
		strncpy(content, s, len);
		content[len] = '\0';
	}
	else
		content[0] = '\0';
}

// set spot, length, contents, ability to edit
void field::set(int r, int c, int l, const char* s, bool e)
{
	set(r,c,l,s);
// Borland should not use the bottom right hand corner of the window
// if we have a field that is ONLY the bottom right hand corner
// we want to make sure we cannot edit this field no matter what
#if PLATFORM == BORLAND
	if( row == dt_rows() -1 && col + len >= dt_columns() -1 && len-1 == 0)
		editAble = false;
	else
#endif
		editAble = e;
}

// empty constructor
field::field()
{
	content = NULL;
	set();
}

// constructor for spot, length
field::field(int r, int c, int l)
{
	content = NULL;
	set(r,c,l);
}

// constructor for spot, length, and contents
field::field(int r, int c, int l, const char* s)
{
	content = NULL;
	set(r,c,l,s);
}

// constructor for spot, length, contents and ability to edit
field::field(int r, int c, int l, const char* s, bool e)
{
	content = NULL;
	set(r,c,l,s,e);
}

// copy constructor
field::field( const field& f )
{
	content = NULL;
	set(f.row, f.col, f.len, f.content, f.editAble);
}

// assignment operator
field& field::operator=( const field& f )
{
	if( content ) delete [] content;
	content = NULL;

	set(f.row, f.col, f.len, f.content, f.editAble);

	return *this;
}

// display the field 
void field::display()
{
	if( content )
// if we are using borland, we cannot place anything to the bottom right
// hand corner, try to display a shorter field if this is the case
// or if we are using ONLY the bottom right hand corner, dont display anything
#if PLATFORM == BORLAND
		if( row == dt_rows() -1 && col + len >= dt_columns() -1 )
		{
			if( len-1 > 0 )
				dt_display(content,row,col,len-1);
		}else
#endif
			dt_display(content,row,col,len);
}

// edit the field
int field::edit()
{
	int rc = 0;

	if( editAble )
	{
		if( !content )
		{
			delete [] content;
			content = new char[len];
			content[0] = '\0';

		}

		if( strlen(content) || len )
			rc = dt_edit(content, row, col, len, len, &ppos, &poff);

	}else
		display();

	return rc;
}

// check to see if the field is editable
bool field::editable() const
{
	bool rc = editAble;
	return rc;
}

// get the address of the string
void* field::data()
{
	return (void*)content;
}

// get the address of the string, ensure that we do not change the string
// NEEDED FOR MULTIPLE SCREENS
void* field::getData() const
{
	return (void*)content;
}

// clone a field, dynamic memory is deleted by user
field* field::clone() const
{
	field* f = new field;
	f->set(row,col,len,content,editAble);
	
	return f;
}

// NEEDED FOR MULTIPLE SCREENS
// get the col row and len of the field
int field::getRow() const { return row; }
int field::getCol() const { return col; }
int field::getLen() const { return len; }

/* END OF FIELD */

/* START OF SCREEN */

/* morescreens is initialized to -1 becuase
 * upon instanciation morescreens is immediately increased
 * this means our first instanciation is 0 or 'no-more-screens'
 * or even !morescreens and our second instanciation means 1morescreen 
 * or simply morescreens
*/

// if this program is to handle the screen then morescreens
// represents "how many more screens" there are to shut off
// before we call dt_stop() ie 1 screen has "no more screens"
#ifdef SCREEN_NOT_HANDLED_BYMAIN
	int screen::morescreens = -1;
#endif

// set the screen to empty and default
void screen::set()
{
	posf = 0;
	fieldCount = 0;
	upRow = upCol = 0;
	lowRow = dt_rows();
	lowCol = dt_columns();
	for(int i = 0; i < NUM_FLD; i++)
	{
		if( dataFields[i] ) delete dataFields[i];
		dataFields[i] = NULL;
	}
 
	standard = true;
}

// set the screen to a certain upper left and lower
// right corners make sure these corners are on the screen
// and that there is no cross over
void screen::set(int ur, int uc, int lr, int lc)
{
	set();
	if( ur < 0 )
		ur = 0;
	else if( ur >= dt_rows() )
		ur = dt_rows() -1;

	if( uc < 0 )
		uc = 0;
	else if( uc >= dt_columns() )
		uc = dt_columns() -1;

	if( ur < 0 )
		ur = 0;
	else if( ur >= dt_rows() )
		ur = dt_rows() -1;

	if( lr < 0 )
		lr = 0;
	else if( lr >= dt_rows() )
		lr = dt_rows() -1;

	if( lc < 0 )
		lc = 0;
	else if( lc >= dt_columns() )
		lc = dt_columns() -1;

	if( lr < ur )
		lr = ur;
	if( lc < uc )
		lc = uc;

	upRow = ur;
	upCol = uc;
	lowRow = lr;
	lowCol = lc;
	standard = false;
}

// default constructor
screen::screen()
{
#ifdef SCREEN_NOT_HANDLED_BYMAIN
	morescreens++;
	if(!morescreens)
		dt_start();
#endif
	// lets make sure it points to null first thing
	for(int i = 0; i < NUM_FLD; i++)
		dataFields[i] = NULL;
	set();
}

// constructor for screens of a certain dimention
screen::screen(int ur, int uc, int lr, int lc)
{
// count one more screen
#ifdef SCREEN_NOT_HANDLED_BYMAIN
	morescreens++;
	if(!morescreens)
		dt_start();
#endif
	// lets make sure it points to null first thing
	for(int i = 0; i < NUM_FLD; i++)
		dataFields[i] = NULL;

	set(ur,uc,lr,lc);
}

// screen destructor
screen::~screen()
{
// if there are no more fields, call dt_stop()
#ifdef SCREEN_NOT_HANDLED_BYMAIN
	if( !morescreens )
		dt_stop();

	morescreens--;
#endif

	for(int i=0; i < NUM_FLD; i++)
		if( dataFields[i] )
		{ 
			delete dataFields[i];
			dataFields[i] = NULL;
		}
}

// clear the screen from upper left to lower right
// standard screens simply clear the entire terminal
void screen::cls()
{
	if( !standard )
		for(int i = upRow; i < lowRow; i++)
			dt_display("",i,upCol,lowCol - upCol);
	else
		dt_clear();
}

// add a field with no content, but of certain position and length
// check/resize the field fitting on the screen
int screen::add(int r, int c, int l)
{       
	r += upRow;
	c += upCol;
	if( r >= lowRow ) r = lowRow -1;
	if( c >= lowCol ) c = lowCol -1;
	if( c + l >= lowCol ) l = lowCol -c +1;
	dataFields[fieldCount] = new field(r,c,l);
	++fieldCount;
	return fieldCount;
}

// add a field to the screen with content
// check/resize the field fitting on the screen
int screen::add(int r, int c, int l, const char* s)
{

	r += upRow;
	c += upCol;
	if( r >= lowRow ) r = lowRow -1;
	if( c >= lowCol ) c = lowCol -1;
	if( c + l >= lowCol ) l = lowCol - c +1;
	dataFields[fieldCount] = new field(r,c,l,s);
	++fieldCount;
	return fieldCount;
}

// add a complete field to the screen
// check/resize the field to fit on the screen
int screen::add(int r, int c, int l, const char* s, bool e)
{

	r += upRow;
	c += upCol;
	
	if( r >= lowRow ) r = lowRow -1;
	if( c >= lowCol ) c = lowCol -1;
	if( c + l > lowCol) l = lowCol - c +1;
	dataFields[fieldCount] = new field(r,c,l,s,e);
	++fieldCount;
	return fieldCount;
}

// add a copy of an existing field to the screen
// we do not change the existing field
// however we change the dimentions of
// the copy to fit on the screen
int screen::add( const field &f )
{
	int length, rowA, colA;

	// if we have a standard screen, we can just clone the field
	// otherwise we must find correct positioning and create
	// a new field
	if( standard )
		dataFields[fieldCount] = f.clone();
	else
	{
		// get the correct position and length
		// to place on the screen
		length = f.getLen();
		length = (length > lowCol - upCol)
			? (lowCol - upCol +1) : length;

		rowA = f.getRow();
		rowA = ( rowA >= lowRow )
			? (lowRow -1) : rowA;

		colA = f.getCol();
		colA = ( colA >= lowCol )
			? (lowCol -1) : colA;

		dataFields[fieldCount] = new field(rowA + upRow,
			colA + upCol, length, (char*)f.getData(),
			f.editable());
	}

	++fieldCount;
	return fieldCount;
}

// display the screen
// characters will remain 'below' what is displayed
void screen::display()
{
	for(int i=0;i < fieldCount; i++)
		dataFields[i]->display();
}

// clear the screen then display
void screen::display(bool clear)
{       
	if( clear ) cls();
	display();
}

// our main method of editing the screen until
// certain conditions are met to exit from editing
// this method is used in the edit methods
// and should not be called by objects outside the
// screen
int screen::editRotation(int *i)
{
	int rc = 0;
	bool exit = false;

	if( *i < fieldCount )
		do
		{
			if( dataFields[*i]->editable() )
				rc = dataFields[*i]->edit();

			switch( rc )
			{
			// ENTER TAB and DOWN all do the same thing
			case ENTER_KEY:
			case TAB_KEY:
			case DOWN_KEY:
				(*i)++;
				break;
			case UP_KEY:
				(*i)--;
				break;
			default:
				exit = true;
			}


			if( *i < 0 ) *i = fieldCount -1;
			if( *i == fieldCount ) *i = 0;
		// enter key in last field exits, an exitcode
		}while( (rc != ENTER_KEY || (*i)) && !exit);

	return rc;
}

// normal editing of a screen
// get the first editable field and send that number
// to editRotation
// no editable fields returns 0
// otherwise we return the field we were working on
int screen::edit()
{
	int rc = fieldCount;

	display();

	while( !dataFields[posf]->editable() && posf < fieldCount-1 )
		posf++;

	if( dataFields[posf]->editable() )
		rc = editRotation(&posf);

	return rc;
}

// clear the screen and then edit the screen
int screen::edit(bool clear)
{
	if( clear ) cls();
	return edit();
}

// edit from *pfield if *pfield is editable
// if not rotate around until we find the first editable field
// or until we have decided there arn't any
// no editable fields returns 0
// otherwise we return the field we were working on
int screen::edit(int *pfield)
{
	int i = ( pfield == NULL || *pfield <= 0 ) ? 0 : *pfield;
	int j = i;
	int rc = 0;
	bool searched = false;

	display();

	while( !dataFields[i]->editable() && i < fieldCount-1 || ( (j == i) && (searched) ))
	{
		i++;
		if( i >= fieldCount -1 )
		{
			i = 0;
			searched = true;
		}
	}

	if( dataFields[i]->editable())
	{
		rc = editRotation(&i);
		*pfield = i;
	}

	return rc;
}

// clear the screen and begin editing from a desired field
int screen::edit(int *pfield, bool clear)
{
	if( clear ) cls();
	return edit(*pfield);
}

// get the address of the screen
field* screen::getfield(int i)
{
	field* p = NULL;
	if( i < fieldCount )
		p = dataFields[i];

	return p;
}

// remove a field from the screen
// re-number the fields if necessary
// and delete the now redundant field
int screen::remove(int i)
{
	if( i < fieldCount && i >= 0 )
	{
		delete dataFields[i];
		while (i < fieldCount - 1)
		{
			dataFields[i] = dataFields[i + 1];
			i++;
		}
		dataFields[i] = NULL;
		fieldCount--;
/*
		while(i < fieldCount - 1)
		{
			*dataFields[i] = *dataFields[i+1];
			++i;
		}

		if( dataFields[--fieldCount] )
			delete dataFields[fieldCount];

		dataFields[fieldCount] = NULL;
*/
	}

	return fieldCount;
}

// copy constructor
screen::screen( const screen& sc )
{
	int length, rowA, colA;

	// lets make sure it points to null first thing

	for(int i = 0; i < NUM_FLD; i++)
		dataFields[i] = NULL;

	if(sc.standard)
		set();
	else
		set(sc.upRow,sc.upCol,sc.lowRow,sc.lowCol);

	if(sc.standard)
	{
		for(int i = 0; i < sc.fieldCount; i++)
			add(*sc.dataFields[i]);
	}else
	{
		for(int i = 0; i < sc.fieldCount; i++)
		{
			length = sc.dataFields[i]->getLen();
			length = (length > lowCol - upCol)
				? (lowCol - upCol +1) : length;

			rowA = sc.dataFields[i]->getRow();
			rowA = ( rowA >= lowRow )
				? (lowRow -1) : rowA;

			colA = sc.dataFields[i]->getCol();
			colA = ( colA >= lowCol )
				? (lowCol -1) : colA;
	
			add(rowA - upRow, colA - upCol,
				length, (char*)sc.dataFields[i]->getData(),
				sc.dataFields[i]->editable());
		}
	}
}

// assignment operator
screen& screen::operator=( const screen& sc )
{
	int length, rowA, colA;
	if(sc.standard)
		set();
	else
		set(sc.upRow,sc.upCol,sc.lowRow,sc.lowCol);

	if(sc.standard)
	{
		for(int i = 0; i < sc.fieldCount; i++)
			add(*sc.dataFields[i]);
	}else
	{
		for(int i = 0; i < sc.fieldCount; i++)
		{
			length = sc.dataFields[i]->getLen();
			length = (length > lowCol - upCol)
				? (lowCol - upCol +1) : length;

			rowA = sc.dataFields[i]->getRow();
			rowA = ( rowA >= lowRow )
				? (lowRow -1) : rowA;

			colA = sc.dataFields[i]->getCol();
			colA = ( colA >= lowCol )
				? (lowCol -1) : colA;
	
			add(rowA - upRow, colA - upCol,
				length, (char*)sc.dataFields[i]->getData(),
				sc.dataFields[i]->editable());
		}
	}
	return *this;
}

// += allows us to easily add an existing
// field to the screen
screen& screen::operator+=( const field& f )
{
	add(f);
	return *this;
}

// clone the screen, deleting dynamic memory is the
// responsibility of the user
screen* screen::clone() const
{
	screen* s;
	int rowA, colA, length;
	if( standard)
		s = new screen();
	else
		s = new screen(upRow,upCol,lowRow,lowCol);

	if( standard )
	{
		for(int i = 0; i < fieldCount; i++)
			s->add(*dataFields[i]);
	}else
	{
		for(int i = 0; i < fieldCount; i++)
		{
			length = dataFields[i]->getLen();
			length = (length > lowCol - upCol)
				? (lowCol - upCol +1) : length;

			rowA = dataFields[i]->getRow();
			rowA = ( rowA >= lowRow )
				? (lowRow -1) : rowA;

			colA = dataFields[i]->getCol();
			colA = ( colA >= lowCol )
				? (lowCol -1) : colA;
	
			s->add(rowA - upRow, colA - upCol,
				length, (char*)dataFields[i]->getData(),
				dataFields[i]->editable());
		}
	}
	return s;
}

/* END OF SCREEN */
