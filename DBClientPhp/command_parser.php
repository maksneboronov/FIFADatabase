<?php

include_once("person.php");
include_once("matches.php");
include_once("stadiums.php");
include_once("teams.php");
include_once("WorkingPeople.php");
include_once("table.php");

if (!function_exists(get_data_on_name))
{
	function get_data_on_name($conn, $dataname, $id)
	{
			switch ($dataname) 
			{
				case 'table':
					$result = sql_select_table($conn);
					if (!$result)
					{
						return "false";
					}
					print_selected_table($result);
					return $result;
				case 'teams':
					$result = sql_select_teams($conn);
					if (!$result)
					{
						return "false";
					}
					print_selected_teams($result);
					return $result;
				case 'persons':
					$result = sql_select_persons($conn);
					if (!$result)
					{
						return "false";
					}
					print_selected_persons($result);
					return $result;
				case 'matches':
					$result = sql_select_matches($conn);
					if (!$result)
					{
						return "false";
					}
					print_selected_matches($result);
					return $result;
				case 'stadiums':		
					$result = sql_select_stadiums($conn);
					if (!$result)
					{
						return "false";
					}
					print_selected_stadiums($result);
					return $result;
				case 'working':
					if (is_null($id))
					{
						$result = false;
						break;
					}
					$id = intval($id);
					if ($id == 0 && $parseCommand[2] != "0")
					{
						$result = false;
						break;
					}
					$result = sql_select_workingPeople_on_match($conn, $id);
					if (!$result)
					{
						return "false";
					}
					print_selected_workingPeople($result);
					return $result;
				
				default:
					echo "Error of datatype: ".$dataname."\n";
					return false;
			}
	}
}

if (!function_exists(delete_data))
{
	function delete_data($conn, $dataname, $id)
	{

		if (!accept_action())
		{	
			return true;
		}

		switch ($dataname) {
			case 'teams':
				return delete_team($conn, $id);
			case 'persons':
				return delete_person($conn, $id);
			case 'matches':
				return delete_match($conn, $id);			
			case 'stadiums':		
				return delete_stadium($conn, $id);
			case 'working':		
				return delete_workingPeople($conn, $id);
			default:
				echo "Error of datatype: ".$dataname."\n";
				return false;
		}
	}
}

if (!function_exists(change_data))
{
	function change_data($conn, $dataname, $id, $params)
	{
		$array = is_correct_params($params);
		if ($array === false)
		{
			return false;
		}

		if (!accept_action())
		{	
			return true;
		}

		switch ($dataname) {
			case 'teams':
				return sql_change_team($conn, $id, $array);
			case 'persons':
				return sql_change_person($conn, $id, $array);
			case 'matches':
				return sql_change_match($conn, $id, $array);			
			case 'stadiums':		
				return sql_change_stadium($conn, $id, $array);
			default:
				echo "Error of datatype: ".$dataname."\n";
				return false;
		}
	}
}


if (!function_exists(insert_data))
{
	function insert_data($conn, $dataname, $params)
	{
		$array = is_correct_params($params);
		if ($array === false)
		{
			return false;
		}

		switch ($dataname) {
			case 'teams':
				return sql_insert_team($conn, $array);
			case 'persons':
				return sql_insert_person($conn, $array);
			case 'matches':
				return sql_insert_match($conn, $array);			
			case 'stadiums':		
				return sql_insert_stadium($conn, $array);
			case 'working':		
				return sql_insert_wp($conn, $array);
			default:
				echo "Error of datatype: ".$dataname."\n";
				return false;
		}
	}
}




?>