<?php

include("sql_function.php");

function sql_select_table($connection)
{
	$query = 'SELECT Id, TeamName, TeamGroup, Coach, Games, Wons, Drawns, Loses, Ga, Gf, Points FROM [fifaDataBase].[dbo].[TeamsView] ORDER BY TeamGroup, Points DESC, Games DESC, Wons DESC, Drawns DESC, Loses DESC, Ga DESC, Gf DESC';
	if (!$queryResult = sql_exec_query($connection, $query))
	{
		return false;
	}

	//$i = 0;
	$result = [];
	while(odbc_fetch_row($queryResult))
	{
		$team = [
			'TeamName' => odbc_result($queryResult, "TeamName"), 
			'Games' => odbc_result($queryResult, "Games"), 
			'Wons' => odbc_result($queryResult, "Wons"), 
			'Drawns' => odbc_result($queryResult, "Drawns"), 
			'Loses' => odbc_result($queryResult, "Loses"), 
			'Ga' => odbc_result($queryResult, "Ga"),
			'Gf' => odbc_result($queryResult, "Gf"),
			'Points' => odbc_result($queryResult, "Points"),
			];
		if (!array_key_exists(odbc_result($queryResult, "TeamGroup"), $result))
		{
			$result[odbc_result($queryResult, "TeamGroup")] = [ $team ];
		}
		else
		{
			array_push($result[odbc_result($queryResult, "TeamGroup")], $team);
		}

		
		//$i++;
	}
	odbc_free_result($queryResult);

	return $result;
}


function print_selected_table($array)
{
	foreach ($array as $group => $teams) 
	{
		printf("%'=47s - %s -  %'=47s\n", ' ', $group, ' ');
		printf(" %-32s%-10s%-10s%-10s%-10s%-10s%-10s%-10s \n%'-102s \n", "TeamName", "Points", "Games", "Wons", "Drawns", "Loses", "Ga", "Gf", " ");
		foreach ($teams as $team) {
			printf(" %-32s%-10s%-10s%-10s%-10s%-10s%-10s%-10s \n" , $team['TeamName'], $team['Points'], $team["Games"], $team['Wons'], $team['Drawns'], $team['Loses'], $team['Ga'], $team['Gf']);
		}
		printf("%'.102s \n\n", " ");
	}
	
}




?>