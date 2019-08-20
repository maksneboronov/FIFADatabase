<?php

include_once("sql_function.php");
include_once("Person.php");

function sql_select_stadiums($connection)
{
	$query = "SELECT id, pid, stName, town, seatCount, sectorCount FROM [fifaDataBase].[dbo].[StadiumsView]";
	if (!$queryResult = sql_exec_query($connection, $query))
	{
		return false;
	}
	$i = 0;
	$result = [];

	while(odbc_fetch_row($queryResult))
	{
		$result[$i] = [
			'id' => odbc_result($queryResult, "id"), 
			'stName' => odbc_result($queryResult, "stName"), 
			'town' => odbc_result($queryResult, "town"), 
			'seatCount' => odbc_result($queryResult, "seatCount"), 
			'sectorCount' => odbc_result($queryResult, "sectorCount"), 
			'pid' => odbc_result($queryResult, "pid")
			];
		$i++;
	}
	odbc_free_result($queryResult);

	return $result;
}

function delete_stadium($connection, $id)
{
	$query = 'DELETE FROM [fifaDataBase].[dbo].[StadiumsView] WHERE id = '.$id.';';
	return sql_exec_prepare_query($connection, $query);
}


function sql_change_stadium($connection, $id, $array)
{
	$query = "UPDATE [fifaDataBase].[dbo].[StadiumsView] SET ";
	
	$fields = ['stName' => ['/[\d\w]{1,30}/', 30, true, true], 'town' => ['/[\d\w\s]{1,30}/', 30, true, true], 'seatCount' => ['/[\d]{1,11}/', 11, false, false], 'sectorCount' => ['/[0-9]{1,2}/', 2, false, false], 'pid' => ['/[\d]{1,6}/', 6, false, false]];
	if (array_key_exists('pid', $array))
	{
		if (strlen(trim(preg_replace($fields['pid'][0], " ", $array['pid']))) != 0 || strlen($array['pid']) > 6 || strlen($array['pid']) < 1)
		{
			echo "Error of change meneger!\n";
			return false;
		}

		$access = sql_is_manager_query($connection, intval($array['pid']));
		if ($access == false)
		{
			return false;
		}
		else if ($access != 'M')
		{
			echo "Person is not manager!\n";
			return true;
		}
	}
	$additingQuery = get_update_fileds($array, $fields);
	if ($additingQuery === false)
	{
		return false;
	}
	$query = $query.$additingQuery."WHERE id = ".$id;

	return sql_exec_prepare_query($connection, $query);
}

function sql_insert_stadium($connection, $array)
{
	$query = "EXEC [fifaDataBase].[dbo].[InsertStadium] ";
	
	$fields = ['stName' => ['/[\d\w]{1,30}/', 30, true, true], 'town' => ['/[\d\w\s]{1,30}/', 30, true, true], 'seatCount' => ['/[\d]{1,11}/', 11, false, false], 'sectorCount' => ['/[0-9]{1,2}/', 2, false, false], 'pid' => ['/[\d]{1,6}/', 6, false, false]];
		
	if (get_update_fileds($array, $fields) === false || !contains_fields($array, $fields))
	{
		return false;
	}
	$access = sql_is_manager_query($connection, intval($array['pid']));
	if ($access == false)
	{
		return false;
	}
	else if ($access != 'M')
	{
		echo "Person is not manager!\n";
		return true;
	}

	$query = $query.$array['pid'].", ".$array['seatCount'].", ".$array['sectorCount'].", '".$array['stName']."', '".$array['town']."', 0x, 0";
	echo $query.'\n';
	return sql_exec_prepare_query($connection, $query);
}

function print_selected_stadiums($array)
{
	printf("%'=110s \n%-5s%-32s%-32s%-14s%-14s%-14s \n%'=110s \n", " ", "id", "stName", "town", "seatCount", "sectorCount", "pid", " ");
	foreach ($array as $key => $value) 
	{
		printf("%-5s%-32s%-32s%-14s%-14s%-14s \n" , $value['id'], $value['stName'], $value['town'], $value['seatCount'], $value['sectorCount'], $value['pid']);
	}
	printf("%'=110s \n", " ");
}
?>