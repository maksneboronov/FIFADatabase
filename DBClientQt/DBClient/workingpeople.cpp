#include "workingpeople.h"

#include <QVariant>
#include <QDebug>

#include "match.h"
#include "team.h"
#include "person.h"

using std::endl;
//QTextStream in(stdin);

WorkingPeople::WorkingPeople()
{
	values["pid"] = "[\\d]{1,6}";
	values["match"] = "[\\d]{4}-[\\d]{2}-[\\d]{2}.[\\d\\w]+.[\\d\\w]+";
	types["pid"] = false;
	types["match"] = false;
	replaces["match"] = false;
	replaces["pid"] = false;
}

WorkingPeople::WorkingPeople(const WorkingPeople &pers)
{
	this->name = pers.name;
	this->access = pers.access;
	this->id = pers.id;
	this->phone = pers.phone;
	this->login = pers.login;
	this->passp = pers.passp;
}

QVector<WorkingPeople> WorkingPeople::GetWorkingPeople(const QSqlDatabase &db, QString info)
{
	//get wp 2018-11-11.assdas.werty
	QStringList data = info.split('.', QString::SkipEmptyParts);
	if (data.size() != 3)
	{
		qDebug() << "Недостаточное количество аргументов при указании матча!";
		return QVector<WorkingPeople>();
	}
	QString date = data[0];
	QString hteam = Team::GetCommandId(db, data[1]);
	QString ateam = Team::GetCommandId(db, data[2]);
	if (hteam.length() == 0 || ateam.length() == 0 || !QRegExp("[\\d]{4}-[\\d]{2}-[\\d]{2}").exactMatch(date))
	{
		qDebug() << "Ошибка ввода параметров!";
		return QVector<WorkingPeople>();
	}
	QString mid = Match::GetMatchId(db, data[1], data[2], date);
	if (mid.length() == 0)
	{
		return QVector<WorkingPeople>();
	}

	QSqlQuery sql(db);
	if (!sql.exec("SELECT DISTINCT p.id, p.persName, p.access, p.persLogin, p.phone, p.passport FROM dbo.WorkingPeople \
		as w INNER JOIN dbo.PersonsView as p ON w.pid = p.id AND w.mid = " + mid))
	{
		return QVector<WorkingPeople>();
	}

	QVector<WorkingPeople> persons;
	while(sql.next())
	{
		WorkingPeople pers = WorkingPeople();

		pers.access = sql.value("access").toString();
		pers.login = sql.value("persLogin").toString();
		pers.name = sql.value("persName").toString();
		pers.phone = sql.value("phone").toString();
		pers.id = sql.value("id").toInt();
		pers.passp = sql.value("passport").toString();

		persons.append(pers);
	}

	return persons;
}

bool WorkingPeople::InsertWorkingPeople(QSqlDatabase db, QMap<QString, QString> params)
{
	if (!params.contains("match"))
	{
		qDebug() << "Не найден параметр match!";
		return false;
	}
	QStringList data = params["match"].split('.', QString::SkipEmptyParts);
	if (data.size() != 3)
	{
		qDebug() << "Недостаточное количество аргументов при указании матча!";
		return false;
	}
	QString date = data[0];
	QString hteam = Team::GetCommandId(db, data[1]);
	QString ateam = Team::GetCommandId(db, data[2]);
	if (hteam.length() == 0 || ateam.length() == 0 || !QRegExp("[\\d]{4}-[\\d]{2}-[\\d]{2}").exactMatch(date))
	{
		qDebug() << "Ошибка ввода параметров!";
		return false;
	}
	QString mid = Match::GetMatchId(db, data[1], data[2], date);
	if (mid.length() == 0)
	{
		return false;
	}

	params["pid"] = Person::GetPersonId(db, params["pid"]);
	if (params["pid"].length() == 0)
	{
		return false;
	}
	if (Service::IsCorretParam(params, WorkingPeople().values, WorkingPeople().types, WorkingPeople().replaces).trimmed().length() == 0
		|| !Service::contains_fields(params, WorkingPeople().values.keys()))
	{
		qDebug() << "Ошибка ввода параметров";
		return false;
	}
	if (IsUsedMan(db, params["pid"], date))
	{
		qDebug() << "Человек уже работает в это день!";
		return false;
	}

	QSqlQuery sql(db);
	return sql.exec("INSERT INTO dbo.WorkingPeople(pid, mid) VALUES(" + params["pid"] + ", " + mid + ")");
}

bool WorkingPeople::IsUsedMan(QSqlDatabase db, QString pid, QString date)
{
	QString query = "SELECT * FROM dbo.Matches as m INNER JOIN dbo.WorkingPeople \
		 as w ON m.mid = w.mid WHERE mdate = '" + date + "' AND w.pid = " + pid;
	QSqlQuery sql(db);
	if (!sql.exec(query))
	{
		qDebug() << "Ошибка запроса!";
		return false;
	}
	return sql.first();
}

bool WorkingPeople::DeleteWorkingPeople(QSqlDatabase db, QString match, QString passport)
{
	QStringList data = match.split('.', QString::SkipEmptyParts);
	if (data.size() != 3)
	{
		qDebug() << "Недостаточное количество аргументов при указании матча!";
		return false;
	}
	QString date = data[0];
	QString hteam = Team::GetCommandId(db, data[1]);
	QString ateam = Team::GetCommandId(db, data[2]);
	if (hteam.length() == 0 || ateam.length() == 0 || !QRegExp("[\\d]{4}-[\\d]{2}-[\\d]{2}").exactMatch(date))
	{
		qDebug() << "Ошибка ввода параметров!";
		return false;
	}
	QString mid = Match::GetMatchId(db, data[1], data[2], date);
	if (mid.length() == 0)
	{
		return false;
	}

	QString pid = Person::GetPersonId(db, passport);
	if (pid.length() == 0)
	{
		return false;
	}
	QSqlQuery sql(db);
	return sql.exec("DELETE FROM dbo.WorkingPeople WHERE pid = " + pid + " and mid = " + mid);
}


void WorkingPeople::PrintWorkingPeople(const QVector<WorkingPeople> &persons)
{
	//get wp 2018-11-11.assdas.werty
	qDebug() << "\n=============================================================";

	for(int i = 0; i < persons.size(); ++i)
	{
		WorkingPeople p = WorkingPeople(persons[i]);
		qDebug() << "#" << i << "\nИмя: " << p.name << "\nЛогин: " << p.login << "\nДолжность: "
				 << p.access << "\nНомер телефона: " << p.phone << "\nПаспорт: " << p.passp << endl;
	}
	qDebug() << "=============================================================" << endl;
}

