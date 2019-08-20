<?php

if (!function_exists(sql_exec_query))
{
	function sql_exec_query($connection, $query)
	{
		// set_error_handler(function($errno, $errstr, $errfile, $errline ) {
		//     throw new ErrorException($errstr, $errno, 0, $errfile, $errline);
		//  });
		try 
		{
			$queryResult = odbc_exec($connection, $query);
		} 
		catch (Exception $e) 
		{
			return false;
		}
		
		if (!$queryResult)
		{
			return false;
		}
		return $queryResult;
	}
}

if (!function_exists(sql_result_count))
{
	function sql_result_count($connection, $query)
	{
		if (!$queryResult = sql_exec_query($connection, $query))
		{
			return false;
		}

		$result = odbc_num_rows($queryResult);
		odbc_free_result($queryResult);

		return $result;
	}
}

if (!function_exists(sql_exec_prepare_query))
{
	function sql_exec_prepare_query($connection, $query)
	{
		 // set_error_handler(function($errno, $errstr, $errfile, $errline ) {
		 //    throw new ErrorException($errstr, $errno, 0, $errfile, $errline);
		 // });
		try 
		{
			$queryResult = odbc_execute(odbc_prepare($connection, $query));
		} 
		catch (Exception $e) 
		{
			return false;
		}
		
		if (!$queryResult)
		{
			return false;
		}
		return $queryResult;
	}
}

if (!function_exists(accept_action))
{
	function accept_action()
	{
		echo "Do you confirm action? Y[yes] or N[non]\n";
		$result = strtolower(trim(fgets(STDIN)));

		if ($result != "y" && $result != "n")
		{
			echo "Command error! Action was not confirmed!\n";
		}
		return $result == "y";
	}
}

if (!function_exists(is_correct_params))
{
	function is_correct_params($params)
	{
		if (count($params) == 0 || is_null($params))
		{
			return false;
		}
		$array = [];
		foreach ($params as $param) 
		{
			$datas = preg_split("[=]", trim($param), -1, PREG_SPLIT_NO_EMPTY);  
			if (count($datas) < 2 || $datas[1] == '')
			{
				echo "Error count of parameters!\n";
				return false;
			}
			$array[$datas[0]] = $datas[1];
		}
		return $array;
	}
}

if (!function_exists(get_update_fileds))
{
	function get_update_fileds($array, $fields)
	{
		$i = 0;
		$query = '';
		$fieldKeys = array_keys($fields);
		foreach ($array as $key => $value) 
		{
			if (!in_array($key, $fieldKeys))
			{
				echo "Unknown parameter - ".$key."\n";
				return false;
			}
			if ($key === 'id')
			{
				continue;
			}
			if (strlen(trim(preg_replace($fields[$key][0], " ", $value))) != 0 || strlen($value) > $fields[$key][1])
			{
				echo "Uncorrect usage of set-value parameter - ".$value."\n";
				return false;
			}
			$newValue = $fields[$key][3] ? str_replace("_", " ", $value) : $value;
			if ($fields[$key][2])
			{
				$query = $query.$key." = "."'".$newValue."' ";
			}
			else
			{
				$query = $query.$key." = ".$newValue." ";
			}
			$i++;
			if ($i < count($array))
			{
				$query = $query.", ";
			}
		}
		return $query;
	}
}


if (!function_exists(contains_fields))
{
	function contains_fields($array, $fields)
	{
		$keys = array_keys($array);
		foreach ($fields as $key => $v) 
		{	
			if (in_array($key, $keys) === false)
			{
				echo "Parameter '".$key."' not found!\n";
				return false;
			}
		}
		return true;
	}
}

if (!function_exists(get_insert_fields))
{
	function get_insert_fields($array, $fields)
	{
		$i = 0;
		$query = '';
		$fieldKeys = array_keys($fields);
		foreach ($array as $key => $value) 
		{
			if (!in_array($key, $fieldKeys))
			{
				echo "Unknown parameter - ".$key."\n";
				return false;
			}
			if ($key === 'id')
			{
				continue;
			}
			if (strlen(trim(preg_replace($fields[$key][0], " ", $value))) != 0 || strlen($value) > $fields[$key][1])
			{
				echo "Uncorrect usage of set-value parameter - ".$value."\n";
				return false;
			}
			$newValue = $fields[$key][3] ? str_replace("_", " ", $value) : $value;
			$query = $query.$newValue." ";
			$i++;
			if ($i < count($array))
			{
				$query = $query.", ";
			}
		}
		return $query;
	}
}

?>