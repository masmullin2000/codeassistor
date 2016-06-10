/**********************************************************
* screen.h - Header file for a wrapper around the direct  *
*            terminal i/o functions                       *
*            additional features to input such as         *
*            scrolling and double/integer type fields are *
*            provided through an inheritance heirarchy    *
* Author: Michael Mullin                                  *
* Date Modified: Thursday November 25th 2004              *
**********************************************************/

#ifndef _screen_h_
#define _screen_h_

#include <string.h>

// this 'commented-out' line can be un-commented in order
// to remove the responsibility of calling dt_start and dt_stop
// from the program using this class
// #define SCREEN_NOT_HANDLED_BYMAIN 
// we are currently handling the string by main in a3main.cpp and a2main.cpp

#define NUM_FLD 50 // number of fields in a screen

extern "C" {
	#include "dtio.h"
}

// function pointers have their own 'type'
// so they are much easier to work with
typedef void(*pt2FuncVOID)(void*);
typedef bool(*pt2FuncBOCH)(char*);
typedef bool(*pt2FuncBOIN)(int &);
typedef bool(*pt2FuncBODO)(double &);

/* START OF FIELD */

// the field class lets us display and edit a string
// with the terminal
class field
{
	int col,row,len,ppos,poff; // the position of the field
				   // and position of the cursor
				   // within the field

	bool editAble; // editability of the field
	char* content; // the string to be displayed and edited

	/* the set functions run the main 'work' of the constructors */
	void set();
	void set(int, int, int);
	void set(int, int, int, const char*);
	void set(int, int, int, const char*, bool);
	
public:
	field(); // empty, non-positioned field
	virtual ~field(); // destruct a field
	field(int, int, int); // constructor with row, col, and length
	field(int, int, int, const char*); // row, col, length, and string
	field(int, int, int, const char*, bool); // data and editablity

	field( const field& ); // copy constructor
	field& operator=( const field& ); // assignment operator

	virtual void display(); // display the field
	virtual int edit(); // display then edit the field
	virtual bool editable() const; // returns whether field is editable
	virtual void* data(); // return the address of the string
	virtual void* getData() const; // same as data, but ensures no change
	virtual field* clone() const; // copy and return the address of this new field

	/* getRow, getCol, getLen allow outside objects to see the dimentions
	 * of the field */
	virtual int getRow() const;
	virtual int getCol() const;
	virtual int getLen() const;
};

/* END OF FIELD */

/* START OF SFIELD */
// an sfield (scrolling field) is a field where a string
// can be larger than the display dimention of the field
// scrolling is done to view the entirety of the data
class sfield : public field
{
	int ppos, poff; // length positioning
	int stlen; // length of the entire string
	char* content; // field cannot hold as much data as we need
	void set(const char*); // set the content
public:
	sfield();
	~sfield();

	// promotion ops
	sfield( const field& ); // copy from field
	sfield& operator=( const field& ); // assign from field

	// deep copies
	sfield( const sfield& ); // copy constructor
	sfield& operator=( const sfield& ); // assignment operator

	sfield( int r, int c, int flen,
		const char *s = "", int slen = 0); // constructor with data
	virtual void display(); // display the sfield
	virtual int edit(); // edit the sfield
	virtual void* data(); // get at data in content
	virtual void* getData() const; // get at data but no change

	// the following clone functions allow the 'downcloning'
	// to various branches of the inheritence hierarchy
	virtual field* clone() const;
	virtual sfield* cloneS() const;
	int getSLen() const; // gets the field length
};

/* END OF SFIELD */

/* START OF HFIELD */
// hfield is a help enabled field
// F1 will trigger the help
class hfield : public sfield
{
	pt2FuncVOID helper; // the helper function
public:
	// empty constructor & constructor with data
	hfield();
	hfield( int r, int c, int flen, const char* initstr = "",
		int slen = 0, void (*help)(void *) = NULL);

	// promotion op, field -> sfield so no field promotion is
	// necessary
	hfield( const sfield& ); // copy from sfield & field
	hfield& operator=( const sfield& ); // assign from sfield & field

	// deep copies
	hfield( const hfield& ); // copy constructor
	hfield& operator=( const hfield& ); // assign op

	// the following clone functions allow the 'downcloning'
	// to various branches of the inheritence hierarchy
	virtual field* clone() const;
	virtual sfield* cloneS() const;
	virtual hfield* cloneH() const;
	virtual int edit();

	// return the function pointer
	pt2FuncVOID getHelp() const { return helper; };
};

/* END OF HFIELD */

/* START OF VFIELD */
// a vfield (validation field) is a string validation field
// a vfield is help enabled
class vfield : public hfield
{
	pt2FuncBOCH vd; // the validation function
public:
	// empty and data constructors
	vfield();
	vfield( int r, int c, int flen, const char *initstr = "",
	     int slen = 0, bool (*valid)(char*) = NULL,
	     void (*help)(void *) = NULL);

	// promotion ops
	vfield( const sfield& ); // copy from sfield & field
	vfield& operator=( const sfield& ); // assign from sfield & field
	vfield( const hfield& ); // copy from hfield
	vfield& operator=( const hfield& ); // assign from hfield

	// deep copies
	vfield( const vfield& ); // copy constructor
	vfield& operator=( const vfield& ); // assign op

	// the following clone functions allow the 'downcloning'
	// to various branches of the inheritence hierarchy
	virtual field* clone() const;
	virtual sfield* cloneS() const;
	virtual hfield* cloneH() const; 
	virtual vfield* cloneV() const;

	// edit the vfield
	virtual int edit();

	// get pointer to function string validation
	pt2FuncBOCH getValid() const { return vd; };
};
/* END OF VFIELD */

/* START OF IVFIELD */
// an ivfield (integer validation field) is a help enabled field
// where data is stored in an integer format
// validation can be done upon the data
// ivfield can act like an integer in some situations
class ivfield : public hfield
{
	pt2FuncBOIN vd; // integer validiation function
	int initV; // 'value' of the ivfield
public:
	// empty and data constructors
	ivfield();
	ivfield(int r, int c, int flen, int init = 0, int slen = 0,
		bool (*valid)(int &) = NULL, void (*help)(void *) = NULL);

	// promotion ops
	ivfield( const sfield& ); // copy from sfield & field
	ivfield& operator=( const sfield& ); // assign from sfield & field
	ivfield( const hfield& ); // copy from hfield
	ivfield& operator=( const hfield& ); // assign from hfield

	// deep copies
	ivfield( const ivfield& ); // copy constructor
	ivfield& operator=( const ivfield& ); // assign op

	// this style of field should be used as 
	// an integer
	ivfield& operator=( int );
	operator int();

	// get at the integer data rather than the 
	// display strings data
	virtual void* data();
	virtual void* getData() const;

	// the following clone functions allow the 'downcloning'
	// to various branches of the inheritence hierarchy
	virtual field* clone() const;
	virtual sfield* cloneS() const;
	virtual hfield* cloneH() const; 
	virtual ivfield* cloneIV() const;

	// change the display data to the value in initV
	// before doing editing and displaying
	virtual void display(); // display the field
	virtual int edit(); 

	// get validation function
	pt2FuncBOIN getValid() const { return vd; };
	int getVal() const;
};

/* END OF IVFIELD */

/* START OF DVFIELD */
// a dvfield (double validation field) is a type of help enabled
// field where data is stored as a double to a certain precision
// validation can be done upon the data
// dvfield can act like a double in some situations
class dvfield : public hfield
{
	pt2FuncBODO vd;
	double initV;
	int dplace;
public:
	// empty and data constructors
	dvfield();
	dvfield(int r, int c, int flen, int places = 2, double init = 0,
	     int slen = 0, bool (*valid)(double &) = NULL,
	     void (*help)(void *) = NULL);

	// promotion ops
	dvfield( const sfield& ); // copy from sfield & field
	dvfield& operator=( const sfield& ); // assign from sfield & field
	dvfield( const hfield& ); // copy from hfield
	dvfield& operator=( const hfield& ); // assign from hfield

	// deep copies
	dvfield( const dvfield& ); // copy constructor
	dvfield& operator=( const dvfield& ); // assign op

	// this style of field must act like a double
	dvfield& operator=( double );
	operator double();

	// get at the initV value rather than the display
	// fields value
	virtual void* data();
	virtual void* getData() const;

	// the following clone functions allow the 'downcloning'
	// to various branches of the inheritence hierarchy
	virtual field* clone() const;
	virtual sfield* cloneS() const;
	virtual hfield* cloneH() const;
	virtual dvfield* cloneDV() const;

	// change the display data to the value in initV
	// before doing editing and displaying
	virtual void display(); // display the field
	virtual int edit();

	// get validation function for double
	pt2FuncBODO getValid() const { return vd; };
	double getVal() const;
};

/* END OF DVFIELD */

/* START OF MMIFIELD */
// an mmifield (maximum, minimum integer field) is a a type
// of valid integer field w/o validation.  Validation is instead
// performed based on a maximum and minimum value
class mmifield : public ivfield
{       
	int mini,maxi; // maximum and minimum values of mmifield
public:
	// empty and data constructors
	mmifield();
	mmifield(int r, int c, int flen, int init = 0, int slen = 0,
		int min = 0, int max = 0, void (*help)(void *) = NULL);

	// promotion ops
	mmifield( const sfield& ); // copy from sfield & field
	mmifield& operator=( const sfield& ); // assign from sfield & field
	mmifield( const hfield& ); // copy from hfield
	mmifield& operator=( const hfield& ); // assign from hfield     
	mmifield( const ivfield& ); // copy from hfield
	mmifield& operator=( const ivfield& ); // assign from hfield

	// deep copies
	mmifield( const mmifield& ); // copy constructor
	mmifield& operator=( const mmifield& ); // assign op

	// the following clone functions allow the 'downcloning'
	// to various branches of the inheritence hierarchy
	virtual field* clone() const;
	virtual sfield* cloneS() const;
	virtual hfield* cloneH() const; 
	virtual ivfield* cloneIV() const;
	virtual mmifield* cloneMMI() const;
	virtual int edit();

	// validate int wrapper
	mmifield& operator=( int );

	// show universe the max and min of the object
	int getMin() const { return mini; };
	int getMax() const { return maxi; };
};

/* END OF MMIFIELD */

/* START OF BOX */
// a box is a 2 dimentional object on the screen
// with a border specified
class box : public field
{
	int hi, wide; // height and width of box
	char edge[9]; // the border
public:
	box(); // empty, non-positioned field
	box(int row, int col, int width, int height,
		const char *s = NULL); // constructor with row, col, and length

	box( const box& ); // copy constructor
	box& operator=( const box& ); // assignment operator

	virtual void display(); // display the field
	virtual int edit(); // display then edit the field
	virtual void* data(); // return the address of the string
	virtual void* getData() const; // same as data, but ensures no change

	virtual field* clone() const; // copy and return the address of this new field
	virtual box* cloneB() const; // clone to a box

	// get the height and width of the object
	virtual int getHi() const { return hi; };
	virtual int getWide() const { return wide; };
};

/* END OF BOX */

/* START OF LABEL */
// a Label is a non-editable field that is used for
// only display purposes
class label : public field
{
public:
	// empty label is an empty non-editable field
	label() : field(0,0,0,"",false) {};
	// a label is a non editable field
	label( int row, int col, const char* s, int len = 0 );

	// to another label or to a field
	field* clone() const;
	label* cloneL() const;
};

/* END OF LABEL */

/* START OF SCREEN */

// the screen class manages NUM_FLD amount of fields
// screen has the ability to fill the entire terminal
// or only part of the terminal
// a partial terminal screen (PTS) is defined by a square
// we give the co-ordinates of the upper left and lower right
// corners of the square for dimentions
// when adding a field to a PTS we position
// according to where the upper left corner of the screen
// is.  eg a screen at (5,5) adding a field at (5,5) will
// place the field at (10,10)
class screen
{
	int posf;
	field* dataFields[NUM_FLD]; // the fields on the screen
	int fieldCount; // amount of fields used
	int upRow,upCol,lowRow,lowCol; // dimentions of a PTS
	bool standard; // PTS is not standard

	/* set does the work of the constructors */
	void set();
	void set(int, int, int, int);

	int editRotation(int *i); // engine of the edit methods

#ifdef SCREEN_NOT_HANDLED_BYMAIN
	static int morescreens;
#endif

public:
	screen( const screen& ); // copy constructor
	screen& operator=( const screen& ); // assignment operator

	screen(); // constructor for standard screens
	screen(int, int, int, int); // constructor for PTS
	virtual ~screen(); // destructor

	void cls(); // clear the screen

	int add(int, int, int); // add a field at position with length
	int add(int, int, int, const char *); // add a field with string data
	int add(int, int, int, const char *, bool); // field + editablity

	screen& operator+=( const field& f ); // easy way to add a field

	int add( const field &f); // add an existing field to screen

	void display(); // display the screen w/o clearing
	void display(bool clear); // display the screen w/ clearing

	int edit(); // edit the screen w/o clearing
	int edit(bool clear); // edit the screen w/ clearing
	int edit(int *pfield); // edit the desired field on the screen
	int edit(int *pfield, bool clear); //edit desired field on the screen
					   // w/ clearing
	field* getfield(int i); // get the mem-address of a field on the screen
	int remove(int i); // remove a field from the screen
	screen* clone() const; // make a copy and return the address 
			       // of this new field

	/* TODO SCREEN MOVEMENT NEXT VERSION */

};
/* END OF SCREEN */

#endif 
/* end _screen_h_ */
