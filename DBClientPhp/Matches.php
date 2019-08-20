<?php 

include("sql_function.php");

function sql_select_matches($connection)
{
	$query = "SELECT mid, hteam, ateam, mdate, tgroup, mg1, mg2, Stid FROM [fifaDataBase].[dbo].[MatchViewWithGroup]";
	if (!$queryResult = sql_exec_query($connection, $query))
	{
		return false;
	}
	$i = 0;
	$result = [];

	while(odbc_fetch_row($queryResult))
	{
		$result[$i] = [
					"id" => odbc_result($queryResult, "mid"), 
					"group" => odbc_result($queryResult, "tgroup"), 
					"hteam" => odbc_result($queryResult, "hteam"), 
					"ateam" => odbc_result($queryResult, "ateam"), 
					"mdate" => odbc_result($queryResult, "mdate"), 
					"gh" => odbc_result($queryResult, "mg1"), 
					"ga" => odbc_result($queryResult, "mg2"),
					"stid" => odbc_result($queryResult, "Stid")
					];
		$i++;
	}
	odbc_free_result($queryResult);

	return $result;
}

function sql_change_match($connection, $id, $array)
{
	$query = "UPDATE [fifaDataBase].[dbo].[MatchesView] SET ";
	
	$intField = ['/[\d]{1,6}/', 6, false, false];
	$fields = ['stid' => $intField, 'gH' => $intField, 'gA' => $intField, 'matchDate' => ['/[\d]{4}-[\d]{2}-[\d]{2}/', 10, true, false]];

	if (!array_key_exists('matchDate', $array))
	{
		echo "Enter date and repeat query.\n";
		return true;
	}
	if (date_parse($array['matchDate'])['error_count'] != 0)
	{
		echo "Error of date format!\n";
		return true;
	}
	else
	{

	}
	if (array_key_exists('stid', $array) && sql_is_used_stadium($connection, $array['stid'], $array['matchDate']) > 0)
	{
		echo "Stadium is used!\n";
		return true;
	}

	$additingQuery = get_update_fileds($array, $fields);
	if ($additingQuery === false)
	{
		return false;
	}
	$query = $query.$additingQuery."WHERE mid = ".$id;
	return sql_exec_prepare_query($connection, $query);
}

function sql_insert_match($connection, $array)
{
	$query = "EXEC [fifaDataBase].[dbo].[InsertNewMatch] ";
	
	$intField = ['/[\d]{1,6}/', 6, false, false];
	$fields = ['hteam' => $intField, 'ateam' => $intField, 'stid' => $intField, 'gh' => $intField, 'ga' => $intField, 'mdate' => ['/[\d]{4}-[\d]{2}-[\d]{2}/', 10, true, false]];
		
	if (get_update_fileds($array, $fields) === false || !contains_fields($array, $fields))
	{
		return false;
	}

	if (date_parse($array['mdate'])['error_count'] != 0)
	{
		echo "Error of date format\n";
		return true;
	}

	if (sql_is_used_team($connection, $array['hteam'], $array['mdate']) > 0 || sql_is_used_team($connection, $array['ateam'], $array['mdate']) > 0  || sql_is_used_stadium($connection, $array['stid'], $array['mdate']) > 0)
	{
		echo "Team or stadium is used!\n";
		return true;
	}

	$query = $query.$array['hteam'].", ".$array['ateam'].", ".$array['stid'].", ".$array['gh'].", ".$array['ga'].", '".$array['mdate']."', 0";
	return sql_exec_prepare_query($connection, $query);
}

function sql_is_used_team($connection, $id, $date)
{
	$query = "SELECT * FROM [fifaDataBase].[dbo].[Matches] WHERE (tid1 = ".$id." OR tid2 = ".$id.") AND mdate = '".$date."'";
	return sql_result_count($connection, $query);
}

function sql_is_used_stadium($connection, $id, $date)
{
	$query = "SELECT * FROM [fifaDataBase].[dbo].[Matches] WHERE mdate = '".$date."' AND Stid = ".$id;
	return sql_result_count($connection, $query);
}

function print_selected_matches($array)
{
	printf("%'=110s \n%-5s%-5s%32s%2s%-5s%-32s%-20s%-5s \n%'=110s \n", " ", "Id", "Group", "Command Home", " ", " ", "Command Away", "Date", "Stadium", " ");
	foreach ($array as $key => $value) 
	{
		printf("%-5s%-5s%32s%2s : %-2s%-32s%-20s%-5s\n" , $value['id'], $value['group'], $value['hteam'], $value['gh'], $value['ga'], $value['ateam'], $value['mdate'], $value['stid']);
	}
	printf("%'=110s \n", " ");
}

function delete_match($connection, $id)
{
	$query = 'DELETE FROM [fifaDataBase].[dbo].[MatchesView] WHERE mid = '.$id;
	return sql_exec_prepare_query($connection, $query);
}

?>