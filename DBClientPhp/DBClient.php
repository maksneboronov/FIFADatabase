<?php

include_once("command_parser.php");

echo "\nConnecting..\n";
$link = odbc_connect("MAKS", "sa", "426456486") or die("Connection error!\n");
echo "\nConnection successful!\n";

echo "\nlogin: ";
$login = trim(fgets(STDIN));
echo "\npassword: ";
$password = trim(fgets(STDIN));
echo "\n";


$personalData = [ 0 => sql_auth($login, $password, $link)];
if ($personalData[0] === -2)
{
	echo "Command error!\n";
	return;
}
if ($personalData[0] === -1)
{
	echo "Uncorrect password!\n";
	return;
}
if (!($personalData[0]['access'] === 'A'))
{
	echo "You have not access for work in this application!\n";
	return;
}


$result = [];
while(true)
{
	echo "query: ";

	$command = trim(fgets(STDIN));

	$parseCommand = $datas = preg_split("[\s]", trim($command), -1, PREG_SPLIT_NO_EMPTY);  ;
	$mainCommand = $parseCommand[0];	

	switch (strtolower($mainCommand)) 
	{
		case 'get':
			if (count($parseCommand) < 2)
			{
				$result = false;
				break;
			}
			if (strtolower($parseCommand[1]) === 'info')
			{
				$result = sql_get_person_on_id($link, $personalData[0]['id']);
				if ($result)
				{
					print_selected_persons($result);
				}
				break;
			}
			$result = get_data_on_name($link, strtolower($parseCommand[1]), strtolower($parseCommand[2]));
			break;
		case 'rm':
			if (count($parseCommand) < 3)
			{
				$result = false;
				break;
			}
			$id = intval($parseCommand[2]);
			if ($id == 0 && $parseCommand[2] != "0")
			{
				$result = true;
				break;
			}
			$result = delete_data($link, strtolower($parseCommand[1]), $id);
			break;
		case 'ch':
			if (count($parseCommand) < 3)
			{
				$result = false;
				break;
			}
			$id = intval($parseCommand[2]);
			if ($id == 0 && $parseCommand[2] != "0")
			{
				$result = false;
				break;
			}
			$result = change_data($link, strtolower($parseCommand[1]), $id, array_slice($parseCommand, 3));
			break;
		case 'add':
			if (count($parseCommand) < 3)
			{
				$result = false;
				break;
			}
			$result = insert_data($link, strtolower($parseCommand[1]), array_slice($parseCommand, 2));
			break;

		case 'exit':
			odbc_close($link);
			return;

		default:
			echo "Error of enter command!\n";
			break;
	}

	if (!$result)
	{
		echo "Error of executing command!\nCommand template: [command] [datatype] {[other params], ...}";
	}

	echo "\n";
}


?>