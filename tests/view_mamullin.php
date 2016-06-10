<?php include("func.inc");

// AUTHOR: Michael Mullin
// DATE LAST MODIFIED: DEC 1/ 2004
// Script Name: view_mamullin.php
//
// This script shows the items contained in the database
// Logically deleted items are not shown

session_start();

import_request_variables("g","g_");
import_request_variables("p","p_");
import_request_variables("c","c_");

if ( $c_PHPSESSID == logged($_SESSION['user']) && isset($c_PHPSESSID) ) { // check to see if we are logged on
	head("View");
	topmenu($_SESSION['user'],"View");
	$dblog = my_sql_login();
	$result = @mysql_query("SELECT * FROM productX") or die("Could not run query from Database");
	$count = 0;

	echo "<form method=\"POST\" action=\"delete_mamullin.php\">";
	while ($row = mysql_fetch_array($result)) { // while we have rows to show, show them
		if( $row[deleted] != 1 ) { // make sure we do not show deleted items
			form($row[pid],$row[name],$row[noh],$row[rol],
				$row[catagory],$row[model],$row[price],
				$row[description],$count);
			++$count;
		}
	}
	mysql_free_result($result);
	mysql_close($dblog);
	bottom();
} else { // we are not successfully logged on
	header("Location: login_mamullin.php");
}
?>

<?php function form($pid,$name,$noh,$rol,$catagory,$model,$price,$description,$count)
{ // the form is a collection of tables containing the items data
	$background = "#00BB00;";
	$colour = "";
	$link = "";
	if( $rol > $noh ) { // number on hand is less than re-order level
		$link = "dark";
		$background = "#226622;";
		$colour = "color:#FFFFFF;";
	}

	switch($catagory)
	{ // our catagory drop down box will contain full words
	case "OTH":
		$catagory = "Other";
		break;
	case "Shp":
		$catagory = "Ship";
		break;
	case "prb":
		$catagory = "Probe";
		break;
	case "bwp":
		$catagory = "Balistic Weapon";
		break;
	case "lwp":
		$catagory = "Laser/Phaser Weapon";
		break;
	case "cht":
		$catagory = "Star Chart";
		break;
	case "stn":
		$catagory = "Space Station";
		break;
	case "sat":
		$catagory = "Satelite";
		break;
	default:
		$catagory = "UNKNOWN";
	}
?>
<table border="1" style="<?=$colour ?>text-align:left;background-color:<?=$background ?>
	width:100%;">
<tr>
	<td style="width:15%"><strong>Product ID:</strong>
	<a href="addmod_mamullin.php?pid=<?=$pid ?>" class="<?=$link ?>"> <?=$pid ?></a></td>
	<td style="width:25%"><strong>Product Name:</strong><?=$name ?></td>
	<td style="width:18%"><strong>Current Supply:</strong><?=$noh ?></td>
	<td style="width:18%"><strong>Re-order Level:</strong><?=$rol ?></td>
	<td style="width:24%"><strong>Catagory:</strong><?=$catagory ?></td>
</tr>
<tr>
	<td><strong>Model:</strong><?=$model ?></td>
	<td><strong>Price:</strong>$<?php printf("%.2f",$price) ?></td>
<?php
if( $noh >= $rol )
{ // put the positive message
?>
	<td colspan="2"><strong> We have adaquate supply</strong></td>
<?php
}else { // put the negative message
?>
	<td colspan="2"><strong> We need more of this item</strong></td>
<?php
}
?>
	<td><strong>Mark for Delete</strong>
	<input name="<?=$count?>" type="checkbox" value="<?=$pid?>" /></td>
</tr>
<tr>
	<td colspan="2" style="width:20%"><strong>Description:</strong>
	<?=$description ?>
	</td>
</tr>
</table><br />
<?php
}
?>

<?php function bottom()
{ // the bottom of the page
?>
<input type="submit" name="submit" value="Delete" />
<input type="reset" name="reset" value="Reset" />
</form>
<a href="logout_mamullin.php">Log Out</a>
<a href="source.html">Source Code</a>
<?php
}
?>
