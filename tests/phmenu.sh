#!/bin/bash
#	MICHAEL MULLIN 029-053-030
#	ASSIGNMENT #1
#	Script name: phmenu

# main program


# get the location of the corp_phones file before any processing is done
. ./changeloc
# export the phonefile so all scripts can use
export phonefile
# loop while we do not wish to quit
loop=y

# i edited the file
if
blah
else
blah
if
blah
fi
fi

while [ "$loop" = y ]
do
	clear
	# print menu
	tput cup 3 12; echo "Corporate Phone Reporting Menu"
	tput cup 4 12; echo "=============================="
	tput cup 6 9; echo "P - Print Phone List"
	tput cup 7 9; echo "A - Add New Phones"
	tput cup 8 9; echo "S - Search for Phones"
	tput cup 9 9; echo "V - View Phone List"
	tput cup 10 9; echo "D - Delete Phone"
	tput cup 11 9; echo "C - Change Phone number"
	tput cup 12 9; echo "L - Change File Location"
	tput cup 14 9; echo "Q - Quit:"
	tput cup 14 19
	# get user choice and perform requested script
	read choice || continue
	case "$choice" in
		[Pp]) ./phlist1
		      echo "Press enter to continue...." ; read ;;
		[Aa]) ./phoneadd ;;
		[Ss]) ./phonefind 
		      echo "Press enter to continue...." ; read ;;
		[Vv]) clear; more "$phonefile"; read ;;
		[Dd]) ./deleterec ;;
		[Cc]) ./changepho ;;
		[Ll]) . ./changeloc ;;
		[Qq]) clear; exit ;;
		*) tput cup 14 4; echo "Invalid Code"; read choice ;;
	esac
done
