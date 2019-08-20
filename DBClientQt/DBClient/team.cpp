#include "team.h"

#include <QVariant>
#include <QDebug>
#include "service.h"
#include <stdio.h>

using std::endl;
//QTextStream in(stdin);

Team::Team()
{
	values["TeamName"] = "[\\d\\w]{1,30}";
	values["Coach"] = "[\\d\\w]{1,30}";
	values["TeamGroup"] = "[A-Z]{1}";
	values["Games"] = "[\\d]{1,5}";
	values["Wons"] = "[\\d]{1,5}";
	values["Drawns"] = "[\\d]{1,5}";
	values["Loses"] = "[\\d]{1,5}";
	values["Gf"] = "[\\d]{1,5}";
	values["Ga"] = "[\\d]{1,5}";
	values["Points"] = "[\\d]{1,5}";

	types["TeamName"] = true;
	types["Coach"] = true;
	types["TeamGroup"] = true;
	types["Games"] = false;
	types["Wons"] = false;
	types["Drawns"] = false;
	types["Loses"] = false;
	types["Gf"] = false;
	types["Ga"] = false;
	types["Points"] = false;

	replaces["TeamName"] = true;
	replaces["Coach"] = true;
	replaces["TeamGroup"] = false;
	replaces["Games"] = false;
	replaces["Wons"] = false;
	replaces["Drawns"] = false;
	replaces["Loses"] = false;
	replaces["Gf"] = false;
	replaces["Ga"] = false;
	replaces["Points"] = false;
}

Team::Team(const Team &pers)
{
	this->coach = pers.coach;
	this->teamName = pers.teamName;
	this->group = pers.group;
	this->id = pers.id;
	this->wons = pers.wons;
	this->loses = pers.loses;
	this->drawns = pers.drawns;
	this->games = pers.games;
	this->points = pers.points;
	this->gf = pers.gf;
	this->ga = pers.ga;
}

bool Team::isUsedTeam(QSqlDatabase db, QString name, QString date)
{
	QString query = "SELECT * FROM dbo.MatchViewWithGroup WHERE mdate = '" +
		date + "' AND (hteam = '" + name.replace('_', ' ') + "' OR ateam = '" + name.replace('_', ' ') + "')";
	QSqlQuery sql(db);
	if (!sql.exec(query))
	{
		qDebug() << "Ошибка выполнения запроса!";
	}
	return sql.first();
}

QString Team::GetCommandId(QSqlDatabase db, QString name)
{
	QSqlQuery sqlP(db);
	if (!sqlP.exec("SELECT Id FROM dbo.TeamsView where TeamName = '" + name.replace('_', ' ') + "'"))
	{
		qDebug() << "Ошибка выполнения запроса!";
		return "";
	}
	if (!sqlP.first())
	{
		qDebug() << "Команда " << name << " не найдена!";
		return "";
	}
	QString result = QString::number(sqlP.value("Id").toInt());
	sqlP.clear();
	return result;
}

QVector<Team> Team::GetTeams(const QSqlDatabase &db)
{
	QSqlQuery sql(db);
	if (!sql.exec("SELECT Id, TeamName, TeamGroup, Coach, Games, Wons, Drawns, Loses, Ga, Gf, Points FROM dbo.TeamsView"))
	{
		return QVector<Team>();
	}

	QVector<Team> teams;
	while(sql.next())
	{
		Team team = Team();

		team.coach = sql.value("Coach").toString();
		team.teamName = sql.value("TeamName").toString();
		team.group = sql.value("TeamGroup").toString();
		team.id = sql.value("Id").toInt();
		team.games = sql.value("Games").toInt();
		team.wons = sql.value("Wons").toInt();
		team.drawns = sql.value("Drawns").toInt();
		team.loses = sql.value("Loses").toInt();
		team.ga = sql.value("Ga").toInt();
		team.gf = sql.value("Gf").toInt();
		team.points = sql.value("Points").toInt();

		teams.append(team);
	}

	return teams;
}

bool Team::ChangeTeam(QSqlDatabase db, QString teamName, QMap<QString, QString> params)
{
	teamName = teamName.replace('_', ' ');
	QString query = "UPDATE dbo.TeamsView SET ";

	QString additing = Service::IsCorretParam(params, Team().values, Team().types, Team().replaces);
	if (additing.trimmed().length() == 0)
	{
		return false;
	}

	query.append(additing + " WHERE TeamName = '" + teamName + "'");

	QSqlQuery sql(db);
	return sql.exec(query);
}

bool Team::InsertTeam(QSqlDatabase db, QMap<QString, QString> array)
{
// add persons persLogin=asd123 persPassword=8126849 persName=jhsad passport=1234567899 phone=214 access=R
	QString query = "EXEC dbo.InsertNewTeam ";
	QString additing = Service::IsCorretParam(array, Team().values, Team().types, Team().replaces);
	if (additing.length() == 0 || !Service::contains_fields(array, Team().values.keys()))
	{
		qDebug() << "Ошибка при вводе параметров!";
		return false;
	}

	QSqlQuery sql(db);
	return sql.exec(query + "'" + array["TeamName"] + "', '" + array["Coach"] + "', '" + array["TeamGroup"] + "', " + array["Games"]
		+ ", " + array["Wons"] + ", " + array["Drawns"] + ", " +
		array["Loses"] + ", " + array["Gf"] + ", " + array["Ga"] + ", " + array["Points"] + ", 0");
}

bool Team::DeleteTeam(QSqlDatabase db, QString teamName)
{
	teamName = teamName.replace('_', ' ');
	QSqlQuery sql(db);
	return sql.exec("DELETE FROM dbo.TeamsView WHERE TeamName = '" + teamName + "'");
}

void Team::GetTable(QSqlDatabase db)
{
	QString query = "SELECT Id, TeamName, TeamGroup, Coach, Games, Wons, Drawns, Loses, Ga, Gf, Points FROM dbo.TeamsView ORDER BY TeamGroup, Points DESC, Games DESC, Wons DESC, Drawns DESC, Loses DESC, Ga DESC, Gf DESC";
	QSqlQuery sql(db);
	if (!sql.exec(query))
	{
		return;
	}

	QMap<QString, QVector<Team> > table;
	while(sql.next())
	{
		Team team = Team();

		team.teamName = sql.value("TeamName").toString();
		team.id = sql.value("Id").toInt();
		team.games = sql.value("Games").toInt();
		team.wons = sql.value("Wons").toInt();
		team.drawns = sql.value("Drawns").toInt();
		team.loses = sql.value("Loses").toInt();
		team.ga = sql.value("Ga").toInt();
		team.gf = sql.value("Gf").toInt();
		team.points = sql.value("Points").toInt();

		QString group = sql.value("TeamGroup").toString();
		if (!table.contains(group))
		{
			table[group] = QVector<Team>();
		}
		table[group].append(team);
	}
	qDebug() << "=============================================================";

	foreach(QString key, table.keys())
	{
		qDebug() << "ГРУППА: " << key;
		foreach(Team team, table[key])
		{
			qDebug() << team.teamName << " О: " << team.points << " И: " << team.games << " П: " << team.wons << " Н: " << team.drawns << " Пр: " <<
				team.loses << " Гз: " << team.ga << " Гп: " << team.gf;
		}
		qDebug() << endl;
	}
	qDebug() << "=============================================================";
}


void Team::PrintTeams(const QVector<Team> &teams)
{
	qDebug() << "=============================================================";

	for(int i = 0; i < teams.size(); ++i)
	{
		Team p = Team(teams[i]);
		qDebug() << "#" << i << "\nНазвание: " << p.teamName << "\nТренер: " << p.coach
				<< "\nГруппа: " << p.group << "\nИгры: "
				 << p.games << "\nОчки: " << p.points << "\nПобеды: " << p.wons << "\nНичьи: " << p.drawns <<
				 "\nПоражения: " << p.loses << "\nГолов забито: " << p.ga << "\nГолов пропущено: " << p.gf << endl;
	}
	qDebug() << "=============================================================";
	qDebug() << endl;
}
