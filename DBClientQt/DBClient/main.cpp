#include <QCoreApplication>

#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QTextCodec>
#include <iostream>
#include <conio.h>
#include <QVector>
#include <QTextStream>
#include <QRegExp>
#include <QMap>

#include "workingpeople.h"
#include "person.h"
#include "stadium.h"
#include "team.h"
#include "match.h"

using std::endl;
QTextStream cin(stdin);

Person currentPerson;

bool IsManager(Person &pers)
{
	return pers.getAccess() == "M";
}
bool IsAdmin(Person &pers)
{
	return pers.getAccess() == "A";
}
bool IsReferi(Person &pers)
{
	return pers.getAccess() == "R";
}
bool IsStuard(Person &pers)
{
	return pers.getAccess() == "S";
}

bool IsAccept()
{
	qDebug() << "Are you accept to do action! Y or N";
	char result = getch();
	return result == 'Y' || result == 'y';
}

QStringList slice(const QStringList &list, uint from, int to = -1)
{
	to = (to <= -1 || to > list.count()) ? list.count() : to;
	if (list.count() < to || from >= to || list.count() == 0)
	{
		return QStringList();
	}

	QStringList result;
	for(int i = from; i < to; ++i)
	{
		result.append(list[i]);
	}
	return result;
}

QMap<QString, QString> is_correct_params(QStringList params)
{
	if (params.size() == 0)
	{
		return QMap<QString, QString>();
	}

	QMap<QString, QString> array;
	for(int i = 0; i < params.size(); ++i)
	{
		QString p = params[i];
		QStringList datas = p.split('=');
		if (datas.size() != 2 || datas[1].size() == 0)
		{
			qDebug() << "Ошибка количества параметров!";
			return QMap<QString, QString>();
		}
		array[datas[0]] = datas[1];
	}
	return array;
}

void get_data_on_name(QSqlDatabase db, QString dataname, QStringList list)
{
	if (!IsStuard(currentPerson) && !IsReferi(currentPerson) && dataname == "persons")
	{
		QVector<Person> persons = Person::GetPersons(db);
		Person::PrintPersons(persons);
	}
	else if (!IsStuard(currentPerson) && dataname == "teams")
	{
		QVector<Team> teams = Team::GetTeams(db);
		Team::PrintTeams(teams);
	}
	else if (dataname == "stadiums")
	{
		QVector<Stadium> stadiums = Stadium::GetStadiums(db);
		Stadium::PrintStadiums(stadiums);
	}
	else if (dataname == "matches")
	{
		QVector<Match> matches = Match::GetMatches(db);
		Match::PrintMatches(matches);
	}
	else if (dataname == "wp")
	{
		if (list.size() == 0)
		{
			qDebug() << "Ошибка ввода данных матча!";
			return;
		}
		QVector<WorkingPeople> wps = WorkingPeople::GetWorkingPeople(db, list[0]);
		WorkingPeople::PrintWorkingPeople(wps);
	}
	else if (dataname == "table")
	{
		Team::GetTable(db);
	}
	else
	{
		qDebug() << "Ошибка типа данных: " << dataname;
	}
}

void change_data(QSqlDatabase db, QString dataname, QString id, QStringList params)
{
	if (!IsAccept())
	{
		return;
	}
	QMap<QString, QString> map = is_correct_params(params);
	if (map.size() == 0)
	{
		return;
	}

	if (!IsStuard(currentPerson) && !IsReferi(currentPerson) && dataname == "person")
	{
		QRegExp reg("[\\d]{10}");
		if (!reg.exactMatch(id))
		{
			qDebug() << "Ошибка ввода паспорта!";
			return;
		}
		if (!Person::ChangePerson(db, id, map))
		{
			qDebug() << "При изменении данных персоны произошла ошибка!";
		}
	}
	else if (IsAdmin(currentPerson) && dataname == "team")
	{
		if (!Team::ChangeTeam(db, id, map))
		{
			qDebug() << "При изменении данных команды произошла ошибка!";
		}
	}
	else if (IsAdmin(currentPerson) && IsManager(currentPerson) && dataname == "stadium")
	{
		if (!Stadium::ChangeStadium(db, id, map))
		{
			qDebug() << "При изменении стадиона возникла ошибка!";
		}
	}
	else if (IsAdmin(currentPerson) && dataname == "match")
	{
		if (!Match::ChangeMatch(db, id, map))
		{
			qDebug() << "При изменении матча возникла ошибка!";
		}
	}
	else
	{
		qDebug() << "Ошибка типа данных: " << dataname;
	}
}

void delete_data(QSqlDatabase db, QString dataname, QString id, QStringList list)
{
	if (!IsAccept())
	{
		return;
	}
	if (dataname == "person")
	{
		QRegExp reg("[\\d]{10}");
		if (!reg.exactMatch(id))
		{
			qDebug() << "Ошибка ввода паспорта!";
			return;
		}
		if (!Person::DeletePerson(db, id))
		{
			qDebug() << "При удалении персоны возникла ошибка!";
		}
	}
	else if (dataname == "team")
	{
		if (!Team::DeleteTeam(db, id))
		{
			qDebug() << "При удалении команды возникла ошибка!";
		}
	}
	else if (dataname == "stadium")
	{
		if (!Stadium::DeleteStadium(db, id))
		{
			qDebug() << "При удалении стадиона возникла ошибка!";
		}
	}
	else if (dataname == "match")
	{
		if (!Match::DeleteMatch(db, id))
		{
			qDebug() << "При удалении матча возникла ошибка!";
		}
	}
	else if (dataname == "wp")
	{
		if (list.size() == 0)
		{
			qDebug() << "Ошибка ввода паспорта человека!";
			return;
		}
		if (!WorkingPeople::DeleteWorkingPeople(db, id, list[0]))
		{
			qDebug() << "При удалении рабочего возникла ошибка!";
		}
	}
	else
	{
		qDebug() << "Ошибка типа данных: " << dataname;
	}
}

void insert_data(QSqlDatabase db, QString dataname, QStringList params)
{
	QMap<QString, QString> map = is_correct_params(params);
	if (map.size() == 0 && params.size() != 0)
	{
		return;
	}
	if (!IsStuard(currentPerson) && !IsReferi(currentPerson) && dataname == "person")
	{
		if (!Person::InsertPerson(db, map))
		{
			qDebug() << "При добавлении персоны возникла ошибка!";
		}
	}
	else if (IsAdmin(currentPerson) && dataname == "team")
	{
		if (!Team::InsertTeam(db, map))
		{
			qDebug() << "При добавлении команды возникла ошибка!";
		}
	}
	else if (IsAdmin(currentPerson) && dataname == "stadium")
	{
		if (!Stadium::InsertStadium(db, map))
		{
			qDebug() << "При добавлении стадиона возникла ошибка!";
		}
	}
	else if (IsAdmin(currentPerson) && dataname == "matche")
	{
		if (!Match::InsertMatch(db, map))
		{
			qDebug() << "При добавлении матча возникла ошибка!";
		}
	}
	else if (!IsStuard(currentPerson) && !IsReferi(currentPerson) && dataname == "wp")
	{
		if (!WorkingPeople::InsertWorkingPeople(db, map))
		{
			qDebug() << "При добавлении рабочего возникла ошибка!";
		}
	}
	else
	{
		qDebug() << "Ошибка типа данных: " << dataname;
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM866"));

	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
	db.setConnectOptions();
	db.setUserName("sa");
	db.setPassword("426456486");
	db.setDatabaseName("Driver={SQL Server};Server=MAKS\\SQLSERVER;Trusted_Connection=Yes;Database=fifaDataBase;");

	qDebug() << "Connecting.. ";
	if (!db.open())
	{
		qDebug() << "Error of connection!";
		return -1;
	}
	qDebug() << "Server connected!" << endl;

	QString login, password;
	qDebug() << "Введите логин:";
	login = cin.readLine();
	qDebug() << "Введите пароль:";
	password = cin.readLine();
	currentPerson = Person::Auth(db, login, password);
	if (currentPerson.getId() == -1)
	{
		db.close();
		qDebug() << "Нет пользователя с таким логином!";
		return -1;
	}
	else if (currentPerson.getId() == -2)
	{
		db.close();
		qDebug() << "Неверный пароль!";
		return -2;
	}
	qDebug() << "Welcome!" << endl;

	while(true)
	{
		qDebug() << "query:";
		QString command = cin.readLine().trimmed();
		QStringList parseCommand = command.split(' ', QString::SkipEmptyParts);
		QString mainCommand = parseCommand[0].toLower();

		if (mainCommand == "print")
		{
			if (parseCommand.size() < 2)
			{
				qDebug() << "Мало аргументов!";
				continue;
			}
			get_data_on_name(db, parseCommand[1].toLower(), slice(parseCommand, 2));
		}
		else if (!IsStuard(currentPerson) && mainCommand == "change")
		{
			if (parseCommand.size() < 3)
			{
				qDebug() << "Мало аргументов!";
				continue;
			}
			change_data(db, parseCommand[1].toLower(), parseCommand[2], slice(parseCommand, 3));
		}
		else if (!IsStuard(currentPerson) && mainCommand == "add")
		{
			if (parseCommand.size() < 2)
			{
				qDebug() << "Мало аргументов!";
				continue;
			}
			insert_data(db, parseCommand[1].toLower(), slice(parseCommand, 2));
		}
		else if (IsAdmin(currentPerson) && mainCommand == "remove")
		{
			if (parseCommand.size() < 3)
			{
				qDebug() << "Мало аргументов!";
				continue;
			}
			delete_data(db, parseCommand[1].toLower(), parseCommand[2], slice(parseCommand, 3));
		}
		else if (mainCommand == "exit")
		{
			db.close();
		}
		else
		{
			qDebug() << "Ошибка запроса!\n";
			continue;
		}
	}



	return a.exec();
}
