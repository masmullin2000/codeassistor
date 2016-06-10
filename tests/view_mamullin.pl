#!/usr/bin/perl

# MAIN.PL
# AUTHOR: MICHAEL MULLIN
# DATE LAST MODIFIED: November 12/ 2004

use warnings;
use strict;
use DBI;
use CGI ":standard";
use CGI::Pretty;
use CGI::Carp "fatalsToBrowser";

require "subs_mamullin.lib";

if( &logged_on() ) {

	# get auth cookie to pass value into top menu (username display)
	my $cookie = cookie("auth");
	my @auth = split(":", $cookie);

	# connect to database
	my $datah = DBI->connect("DBI:mysql:database=b3201a21;host=zenit-db",
		"b3201a21", "quest1", {RaiseError=>1});

	# get all the non-deleted records for display
	my $sql = "SELECT * FROM product WHERE deleted = 0 ORDER BY pid";
	my $stmth = $datah->prepare($sql);
	$stmth->execute();

	# put the necessary header and top menu
	&heading("Michael\'s Space Emporium : View/Update");
	print "<body>\n";
	&top($auth[0],"View");


	print "<table border=\"1\">";

	my $line;
	my $count = 0;
	my $addmod;
	my $linkColor = "dark";
	my $price;
	my $catagory;
	my $supply;

	print "<form action=\"delete_mamullin.pl\" method=\"post\">";

	# if we have data to display, do so
	while( $line = $stmth->fetchrow_hashref ) {

		# get the full catagory word from three letter database store		
		$catagory = &getCat($line->{catagory});

		# make sure the price is formatted correctly
		$price = sprintf("%.02lf", $line->{price});
		$price = "\$" . $price;

		# make sure that if we chose to modify a record, we select the correct
		# record via get principals
		$addmod = "addmod_mamullin.pl?pid=" . $line->{pid};

		# mark the record if number on hand is less than re-order level
		if( int($line->{rol}) <= int($line->{noh}) ) {
			print "<table border=\"1\" "
				. "style=\"text-align:left;background-color:#00BB00;"
				. "width:100%;\">";
			$supply = "We have adaquate supply";
			$linkColor = "";
		}else {
			print "<table border=\"1\" style=\"color:#FFFFFF;"
				. "text-align:left;background-color:#226622;width:100%;\">";
			$supply = "We need more of this item";
			$linkColor = "dark";
		}
		# print out the table containing info
###START HERE###
		print<<END;
		<tr>
			<td style="width:15%"><strong>Product ID:</strong>
			<a href="$addmod" class="$linkColor"> $line->{pid} </a></td>
				<td style="width:25%"><strong>Product Name:</strong> $line->{name}</td>
			<td style="width:18%"><strong>Current Supply:</strong> $line->{noh}</td>
			<td style="width:18%"><strong>Re-order Level:</strong> $line->{rol}</td>
			<td style="width:24%"><strong>Catagory:</strong> $catagory </td>
		</tr>
		<tr>
			<td><strong>Model:</strong> $line->{model}</td>
			<td><strong>Price:</strong> $price</td>
			<td colspan=\"2\"><strong> $supply</strong></td>
			<td><strong>Mark for Delete</strong>
				<input name="$count" type="checkbox" value="$line->{pid}" />
			</td>
		</tr>
		<tr>
			<td colspan=\"2\" style=\"width:20%\"><strong>Description:</strong>
				$line->{description}</td>
		</tr>
		</table><br />
END
###END HERE###
		$count++;

	} #end of while
	# clicking delete will logically delete all marked records
	# reset will un-mark any marked records
	print "<input type=\"submit\" name=\"submit\" value=\"Delete\" />\n";
	print "<input type=\"reset\" name=\"reset\" value=\"Reset\" />\n";
	print "</form>";
	print "<a href=\"logout_mamullin.pl\">Log Out</a>\n";
	&finish();
# authentication has failed
} else {
	# goto the login page
	print header(-location=>"login_mamullin.pl");
}

# subroutine gets the full catagory word from database
# which only stores three letters
sub getCat() {
	my $rc;
	my $cat = shift @_;
	if( $cat eq "OTH" ) {
		$rc = "OTHER";
	}elsif ( $cat eq "Shp" ) {
		$rc = "Space Ship";
	}elsif ( $cat eq "prb" ) {
		$rc = "Probe";
	}elsif ( $cat eq "bwp" ) {
		$rc = "Balistic Weapon";
	}elsif ( $cat eq "lwp" ) {
		$rc = "Laser/Phaser Weapon";
	}elsif ( $cat eq "cht" ) {
		$rc = "Star Chart";
	}elsif ( $cat eq "stn" ) {
		$rc = "Space Station";
	}elsif ( $cat eq "sat" ) {
		$rc = "Satelite";
	}else { $rc = "UNKNOWN"; }

	return $rc;
}