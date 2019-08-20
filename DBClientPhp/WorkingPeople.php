<?php

include_once("sql_function.php");

function sql_select_workingPeople_on_match($connection, $id)
{
	$query = "SELECT DISTINCT p.id, p.persName, p.access, p.persLogin, p.phone FROM [fifaDataBase].[dbo].[WorkingPeople] as w INNER JOIN [fifaDataBase].[dbo].[PersonsView] as p ON w.pid = p.id AND w.mid = ".$id;
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
			'persName' => odbc_result($queryResult, "persName"), 
			'phone' => odbc_result($queryResult, "phone"), 
			'access' => odbc_result($queryResult, "access"), 
			'persLogin' => odbc_result($queryResult, "persLogin")
			];
		$i++;
	}
	odbc_free_result($queryResult);
	return $result;
}

function sql_insert_wp($connection, $array)
{	
	$intField = ['/[\d]{1,6}/', 6, false, false];
	$fields = ['mid' => $intField, 'pid' => $intField];
		
	if (get_update_fileds($array, $fields) === false || !contains_fields($array, $fields))
	{
		return false;
	}
	if (sql_is_used_man($connection, $array['pid'], sql_get_date($connection, $array['mid'])) > 0)
	{
		echo "Person is already used!\n";
		return true;
	}
	$query = "INSERT INTO [fifaDataBase].[dbo].[WorkingPeople](pid, mid) VALUES(".$array['pid'].", ".$array['mid'].")";
	echo $query."\n";
	return sql_exec_prepare_query($connection, $query);
}

function sql_get_date($connection, $id)
{
	$query = "SELECT mdate FROM [fifaDataBase].[dbo].[Matches] WHERE mid = ".$id;
	if (!$queryResult = sql_exec_query($connection, $query))
	{
		return false;
	}
	odbc_fetch_row($queryResult);
	$date = odbc_result($queryResult, 'mdate'); 
	odbc_free_result($queryResult);
	return $date;

}

function sql_is_used_man($connection, $id, $date)
{
	$query = "SELECT * FROM [fifaDataBase].[dbo].[Matches] as m INNER JOIN [fifaDataBase].[dbo].[WorkingPeople] as w ON m.mid = w.mid WHERE mdate = '".$date."' AND w.pid = ".$id;
	return sql_result_count($connection, $query);
}

function print_selected_workingPeople($array)
{
	printf("%'=103s \n%-5s%-32s%-13s%-9s%-32s \n%'=103s \n", " ", "Id", "Name", "Phone", "Access", "Login", " ");
	foreach ($array as $key => $value) 
	{
		printf("%-5s%-32s%-13s%-9s%-32s \n" , $value['id'], $value['persName'], $value['phone'], $value['access'], $value['persLogin']);
	}
	printf("%'=103s \n", " ");
}

function delete_workingPeople($connection, $id)
{
	$query = 'DELETE FROM [fifaDataBase].[dbo].[WorkingPeople] WHERE pid = '.$id;
	return sql_exec_prepare_query($connection, $query);
}

?>