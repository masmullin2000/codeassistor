// Bookstore Order Processor
// BTP200041 - Assignment 4
// a4main.cpp
// Chris Szalwinski
// March 5 2004
// Modifications by Michael Mullin March 24, 2004


#define MAXORDERS 100
#include <iostream.h>
#include "ISBNPrefix.h"
#include "Order.h"
char menu();
char style();
int findOrder(Order* order[], int noOrders, const ISBNPrefix& rangeList);

int main()
{
	char c, choice;
	int i, noOrders = 0;
	ISBNPrefix rangeList("prefixRanges.txt");
	Order* order[MAXORDERS];

	cout << "Bookstore Order Processor\n"
		 << "=========================\n";
	do {
		choice = menu();

		if (choice == 'P') {
			if (noOrders < MAXORDERS) {
				order[noOrders] = new Order();
				choice = 'X';
				if(order[noOrders]->placeOrder(cin, rangeList))
				{
					// MODIFICATION
					char working[11]; 
					for( int i=0; i < noOrders; i++ )
					{
						order[noOrders]->isbnToStr(working);
						if( order[i]->has( working ) )
						{
							order[i]->add( order[noOrders] );
							delete [] order[noOrders];
							noOrders--;
						}
					}
					// END MODIFICATION
					noOrders++;
				}else 
					delete [] order[noOrders];
			} else
				cout << "No more room for orders!" << endl;
		}

		switch(choice) {
			case 'D':
				i = findOrder(order, noOrders, rangeList);
				if (i >= 0)
					order[i]->recordDelivery(cin);
				else
					cout << "No delivery recorded!" << endl;
				break;
			case 'V':
				cout << "         ISBN  Ordered  Delivered\n";
				cout << "---------------------------------" << endl;
				for (int i = 0; i < noOrders; i++) {
					order[i]->display();
					cout << endl;
				}
				break;
			case 'C':
				c = style();
				for (int i = 0; i < noOrders; i++) {
					order[i]->style(c);
				}
				break;
		}
	} while (choice != 'Q');

	cout << "\nSigning off ... " << endl;
	// deallocating order memory
	for (int i = 0; i < noOrders; i++)
		delete order[i];
	return 0;
}

char menu() {
	char c;
	bool ok = false;
	cout << '\n';
	cout << "Please select from the following options :\n";
	cout << " P - Place an order with a publisher\n";
	cout << " D - Record a delivery from a publisher\n";
	cout << " V - View status of books on order\n";
	cout << " C - Configure order display\n";
	cout << " Q - Quit\n";
	do {
		cout << " Your selection : ";
		c = ' ';
		cin.get(c);
		if (c >= 'a' && c <= 'z')
			c -= 'a' - 'A';
		if (cin.fail()) {
			cin.clear();
			cin.ignore(2000, '\n');
			cout << " Invalid input.  Try again." << endl;
		} else if (c == '\n') {
			;
		} else if (c != 'P' && c != 'D' && c != 'V' && c != 'C' && c != 'Q') {
			cin.ignore(2000, '\n');
			cout << " Invalid input.  Try again." << endl;
		} else if (cin.get() != '\n') {
			cin.ignore(2000, '\n');
			cout << " Trailing Characters.  Try Again. " << endl;
		} else if (c == 'P' || c == 'D' || c == 'V' || c == 'C' || c == 'Q')
			ok = true;
	} while (!ok);
	cout << endl;
	return c;
}

// findOrder prompts for and accepts the ISBN of the order to be updated,
// searches the array of pointers order[noOrders] for a match and returns the
// index of the matching order, -1 if not found or abandonned
int findOrder(Order* order[], int noOrders, const ISBNPrefix& rangeList) {
	int i;
	bool accepted, found = false;

	i = 0;
	ISBN search;

	if( accepted = search.accept(cin,rangeList) )
		while( i < noOrders && !found )
		{
			if( order[i]->has(search) )
				found = true;
			else
				++i;
		}

	if( !found )
		i = -1;

	return i;
}

// style prompts for and accepts a character identifying the output style
// to be used and returns the character itself
char style() {
	char c;
	bool ok;

	do 
	{
		ok = true;
		cout << "Please enter delimiter (\"-\" \" \" ENTERKEY) : ";
		c = cin.get();
		if( c == '\n' ) // if ENTERKEY then 'nospace'
			c = '\0';
		if ( c != '-' && c != ' ' && c != '\0' )
		{
			ok = false;
			cin.ignore(2000, '\n');
			cout << "Invalid character.  Try Again." << endl;
		} else if( c != '\0' )
			if( char(cin.get()) != '\n' )
			{
				ok = false;
				cin.ignore(2000, '\n');
				cout << "Trailing characters.  Try Again." << endl;
			}
	} while(!ok);

	return c;
}