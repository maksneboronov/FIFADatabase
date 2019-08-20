<?php

include_once("sql_function.php");

if (!function_exists(sql_get_person_on_id))
{
	function sql_get_person_on_id($connection, $id)
	{
		$query = "SELECT id, persName, passport, phone, access, persLogin, persPassword FROM [fifaDataBase].[dbo].[PersonsView] WHERE id = ".$id;
		if (!$queryResult = sql_exec_query($connection, $query))
		{
			return false;
		}
		odbc_fetch_row($queryResult);

		return 	[ 0 => [
				'id' => odbc_result($queryResult, "id"), 
				'persName' => odbc_result($queryResult, "persName"), 
				'passport' => odbc_result($queryResult, "passport"), 
				'phone' => odbc_result($queryResult, "phone"), 
				'access' => odbc_result($queryResult, "access"), 
				'persLogin' => odbc_result($queryResult, "persLogin"), 
				] ];
	}
}

if (!function_exists(sql_auth))
{
	function sql_auth($login, $password, $connection)
	{
		$query = "SELECT id, persName, passport, phone, access, persPassword, persLogin FROM [fifaDataBase].[dbo].[PersonsView] WHERE persLogin LIKE '".$login."'  ";
		if (!$queryResult = sql_exec_query($connection, $query))
		{
			return -2;
		}
		odbc_fetch_row($queryResult);
		$pos = strpos(md5($password), odbc_result($queryResult, "persPassword"));
		if ($pos === false)
		{
			return -1;
		}

		return [ 'id' => odbc_result($queryResult, "id"), 'access' => odbc_result($queryResult, "access") ];
	}
}

if (!function_exists(sql_is_manager_query))
{
	function sql_is_manager_query($connection, $id)
	{
		$query = 'SELECT access FROM [fifaDataBase].[dbo].[PersonsView] WHERE id = '.$id;
		if (!$queryResult = sql_exec_query($connection, $query))
		{
			return false;
		}

		$i = 0;
		$result = '';
		while(odbc_fetch_row($queryResult))
		{
			$result = odbc_result($queryResult, "access");
		}
		odbc_free_result($queryResult);
		return $result;
	}
}

if (!function_exists(sql_select_persons))
{
	function sql_select_persons($connection)
	{
		$query = 'SELECT id, persName, passport, phone, access, persLogin FROM [fifaDataBase].[dbo].[PersonsView]';
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
				'passport' => odbc_result($queryResult, "passport"), 
				'phone' => odbc_result($queryResult, "phone"), 
				'access' => odbc_result($queryResult, "access"), 
				'persLogin' => odbc_result($queryResult, "persLogin")
				];
			$i++;
		}
		odbc_free_result($queryResult);
		return $result;
	}
}

if (!function_exists(sql_insert_person))
{
	function sql_insert_person($connection, $array)
	{
		$query = "EXEC [fifaDataBase].[dbo].[InsertNewPerson] ";
		
		$fields = ['persName' => ['/[\d\w]{1,30}/', 30, true, true], 'persLogin' => ['/[\d\w]{1,30}/', 30, true, false], 'access' => ['/[AMSR]/', 1, true, false], 'phone' => ['/[\d]{1,11}/', 11, true, false], 'passport' => ['/[0-9]{10}/', 10, true, false], 'persPassword' => ['/[\d\w]{5,}/', 512, true, true]];
		
		if (get_update_fileds($array, $fields) === false || !contains_fields($array, $fields))
		{
			return false;
		}

		$query = $query."'".$array['persName']."', '".$array['passport']."', '".$array['phone']."', '".$array['access']."', '".$array['persLogin']."', '".md5($array['persPassword'])."', 0";

		return sql_exec_prepare_query($connection, $query);
	}
}

if (!function_exists(delete_person))
{
	function delete_person($connection, $id)
	{
		$query = 'DELETE FROM [fifaDataBase].[dbo].[PersonsView] WHERE id = '.$id;
		return sql_exec_prepare_query($connection, $query);
	}
}

if (!function_exists(sql_change_person))
{
	function sql_change_person($connection, $id, $array)
	{
		$query = "UPDATE [fifaDataBase].[dbo].[PersonsView] SET ";
		
		$fields = ['persName' => ['/[\d\w]{1,30}/', 30, true, true], 'persLogin' => ['/[\d\w]{1,30}/', 30, true, false], 'access' => ['/[AMSR]/', 1, true, false], 'phone' => ['/[\d]{1,11}/', 11, true, false], 'passport' => ['/[0-9]{10}/', 10, true, false], 'persPassword' => ['/[\d\w]{5,}/', 512, true, true]];
		if (array_key_exists('persPassword', $array))
		{
			if (strlen(trim(preg_replace($fields['persPassword'][0], " ", $array['persPassword']))) != 0 || strlen($array['persPassword']) > 30 || strlen($array['persPassword']) < 5)
			{
				echo "Error of enter password!\n";
				return false;
			}
			$array['persPassword'] = md5($array['persPassword']);
		}
		$additingQuery = get_update_fileds($array, $fields);
		if ($additingQuery === false)
		{
			return false;
		}
		$query = $query.$additingQuery."WHERE id = ".$id;

		return sql_exec_prepare_query($connection, $query);
	}
}

if (!function_exists(print_selected_persons))
{
	function print_selected_persons($array)
	{
		printf("%'=103s \n%-5s%-32s%-12s%-13s%-9s%-32s \n%'=103s \n", " ", "id", "persName", "passport", "phone", "access", "persLogin", " ");
		foreach ($array as $key => $value) 
		{
			printf("%-5s%-32s%-12s%-13s%-9s%-32s \n" , $value['id'], $value['persName'], $value['passport'], $value['phone'], $value['access'], $value['persLogin']);
		}
		printf("%'=103s \n", " ");
	}
}


?>