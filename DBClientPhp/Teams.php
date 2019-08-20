<?php

include_once("sql_function.php");

function sql_select_teams($connection)
{
	$query = 'SELECT Id, TeamName, TeamGroup, Coach, Games, Wons, Drawns, Loses, Ga, Gf, Points FROM [fifaDataBase].[dbo].[TeamsView]';
	if (!$queryResult = sql_exec_query($connection, $query))
	{
		return false;
	}

	$i = 0;
	$result = [];
	while(odbc_fetch_row($queryResult))
	{
		$result[$i] = [
			'id' => odbc_result($queryResult, "Id"), 
			'TeamName' => odbc_result($queryResult, "TeamName"), 
			'Coach' => odbc_result($queryResult, "Coach"), 
			'Games' => odbc_result($queryResult, "Games"), 
			'Wons' => odbc_result($queryResult, "Wons"), 
			'Drawns' => odbc_result($queryResult, "Drawns"), 
			'Loses' => odbc_result($queryResult, "Loses"), 
			'Ga' => odbc_result($queryResult, "Ga"),
			'Gf' => odbc_result($queryResult, "Gf"),
			'Points' => odbc_result($queryResult, "Points"),
			'TeamGroup' => odbc_result($queryResult, "TeamGroup"),
			];
		$i++;
	}
	odbc_free_result($queryResult);

	return $result;
}

function delete_team($connection, $id)
{
	$query = 'DELETE FROM [fifaDataBase].[dbo].[TeamsView] WHERE Id = '.$id;
	return sql_exec_prepare_query($connection, $query);
}

function sql_insert_team($connection, $array)
{
	$query = "EXEC [fifaDataBase].[dbo].[InsertNewTeam] ";
	
	$intField = ['/[0-9]{1,4}/', 4, false, false];
	$fields = ['TeamName' => ['/[\d\w]{1,30}/', 30, true, true], 'Coach' => ['/[\d\w\.]{1,30}/', 30, true, true], 'TeamGroup' => ['/[A-Z]/', 1, true, false], 'Games' => ['/[\d]{1,4}/', 4, false, false], 'Wons' => $intField, 'Drawns' => $intField, 'Loses' => $intField, 'Gf' => $intField, 'Ga' => $intField, 'Points' => $intField];
	
	if (get_update_fileds($array, $fields) === false || !contains_fields($array, $fields))
	{
		return false;
	}

	$query = $query."'".$array['TeamName']."', '".$array['Coach']."', '".$array['TeamGroup']."', ".$array['Games'].", ".$array['Wons'].", ".$array['Drawns'].", ".$array['Loses'].", ".$array['Gf'].", ".$array['Ga'].", ".$array['Points'].', 0';

	return sql_exec_prepare_query($connection, $query);
}

function sql_change_team($connection, $id, $array)
{
	$query = "UPDATE [fifaDataBase].[dbo].[TeamsView] SET ";
	
	$intField = ['/[0-9]{1,4}/', 4, false, false];
	$fields = ['TeamName' => ['/[\d\w]{1,30}/', 30, true, true], 'Coach' => ['/[\d\w\.]{1,30}/', 30, true, true], 'TeamGroup' => ['/[A-Z]/', 1, true, false], 'Games' => ['/[\d]{1,4}/', 4, false, false], 'Wons' => $intField, 'Drawns' => $intField, 'Loses' => $intField, 'Gf' => $intField, 'Ga' => $intField, 'Points' => $intField];
	$additingQuery = get_update_fileds($array, $fields);
	if ($additingQuery === false)
	{
		return false;
	}
	$query = $query.$additingQuery."WHERE Id = ".$id;

	return sql_exec_prepare_query($connection, $query);
}

function print_selected_teams($array)
{
	printf("%'=160s \n%-8s%-32s%-32s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s \n%'=160s \n", " ", "Id", "TeamName", "Coach", "TeamGroup","Games", "Wons", "Drawns", "Loses", "Ga", "Gf", "Points", " ");
	foreach ($array as $key => $value) 
	{
		printf("%-8s%-32s%-32s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s \n" , $value['id'], $value['TeamName'], $value['Coach'], $value["TeamGroup"], $value["Games"], $value['Wons'], $value['Drawns'], $value['Loses'], $value['Ga'], $value['Gf'], $value['Points']);
	}
	printf("%'=160s \n", " ");
}


?>