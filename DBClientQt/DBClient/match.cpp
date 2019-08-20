#include "match.h"

#include <QVariant>
#include <QDebug>
#include "service.h"
#include "team.h"
#include "stadium.h"

Match::Match()
{
	values["stid"] = "[\\d\\w]{1,30}";
	values["gH"] = "[\\d]{1,5}";
	values["gA"] = "[\\d]{1,5}";
	values["matchDate"] = "[\\d]{4}-[\\d]{2}-[\\d]{2}";
	values["hteam"] = "[\\d\\w]{1,30}";
	values["ateam"] = "[\\d\\w]{1,30}";

	types["stid"] = false;
	types["gH"] = false;
	types["gA"] = false;
	types["matchDate"] = true;
	types["hteam"] = false;
	types["ateam"] = false;

	replaces["stid"] = false;
	replaces["gH"] = false;
	replaces["gA"] = false;
	replaces["matchDate"] = false;
	replaces["hteam"] = false;
	replaces["ateam"] = false;
}

Match::Match(const Match &pers)
{
	this->gh = pers.gh;
	this->ga = pers.ga;
	this->group = pers.group;
	this->id = pers.id;
	this->date = pers.date;
	this->hteam = pers.hteam;
	this->ateam = pers.ateam;
	this->stid = pers.stid;
}

QString Match::GetMatchId(QSqlDatabase db, QString hteam, QString ateam, QString date)
{
	QSqlQuery sqlP(db);
	if (!sqlP.exec("SELECT mid FROM dbo.MatchViewWithGroup WHERE mdate = '" + date + "' and hteam = '" + hteam + "' and ateam = '" + ateam + "'"))
	{
		qDebug() << "Ошибка выполнения запроса!";
		return "";
	}
	if (!sqlP.first())
	{
		qDebug() << "Матч не найден!";
		return "";
	}
	QString result = QString::number(sqlP.value("mid").toInt());
	sqlP.clear();
	return result;
}

QVector<Match> Match::GetMatches(const QSqlDatabase &db)
{
	QSqlQuery sql(db);
	if (!sql.exec("SELECT mid, hteam, ateam, mdate, tgroup, mg1, mg2, Stid FROM dbo.MatchViewWithGroup"))
	{
		return QVector<Match>();
	}

	QVector<Match> Matches;
	while(sql.next())
	{
		Match match = Match();

		match.id = sql.value("mid").toInt();
		match.hteam = sql.value("hteam").toString();
		match.ateam = sql.value("ateam").toString();
		match.date = sql.value("mdate").toDate();
		match.group = sql.value("tgroup").toString();
		match.gh = sql.value("mg1").toInt();
		match.ga = sql.value("mg2").toInt();
		match.stid = sql.value("Stid").toInt();

		Matches.append(match);
	}

	return Matches;
}

bool Match::ChangeMatch(QSqlDatabase db, QString info, QMap<QString, QString> params)
{
//	ch matches 2018-11-11.assdas.werty gH=2
	QString query = "UPDATE dbo.MatchesView SET ";
	if (params.contains("hteam") || params.contains("ateam") || params.contains("matchDate"))
	{
		qDebug() << "Нельзя менять учавствующие команды и дату";
		return false;
	}

	QStringList data = info.split('.', QString::SkipEmptyParts);
	if (data.size() != 3)
	{
		qDebug() << "Недостаточное количество аргументов при указании матча!";
		return false;
	}

	params["matchDate"] = data[0];
	params["hteam"] = Team::GetCommandId(db, data[1]);
	params["ateam"]= Team::GetCommandId(db, data[2]);

	if (params.contains("stid"))
	{
		params["stid"] = Stadium::GetStadiumId(db, params["stid"]);
		if (params["stid"].length() == 0)
		{
			return false;
		}
		if (Stadium::isUsedStadium(db, params["stid"], data[0]))
		{
			qDebug() << "На этот день стадион уже занят!";
			return false;
		}
	}

	if (params["hteam"].length() == 0 || params["ateam"].length() == 0)
	{
		return false;
	}

	int i = 0;
	foreach(QString key, params.keys())
	{
		QRegExp reg(Match().values[key]);
		if (!reg.exactMatch(params[key]))
		{
			qDebug() << "Неверный формат введенного параметра - " << params[key] << " !";
			return false;
		}
		if (key == "hteam" || key == "ateam" || key == "matchDate")
		{
			++i;
			continue;
		}
		if (key != "matchDate")
		{
			query += key + " = " + params[key];
		}

		if (++i == params.size())
		{
			break;
		}
		query += " ,";
	}
	if (query[query.length() - 1] == ',')
	{
		query.remove(query.length() - 1, 1);
	}

	query += " WHERE matchDate = '" + params["matchDate"] + "' and hteam = " + params["hteam"] + " and ateam = " + params["ateam"];
	QSqlQuery sql(db);
	return sql.exec(query);
}

bool Match::InsertMatch(QSqlDatabase db, QMap<QString, QString> array)
{
// add matches hteam=assdas ateam=werty gH=1 gA=0 stid=asda111 matchDate=2018-11-11
	QString query = "EXEC dbo.InsertNewMatch ";
	QString additing = Service::IsCorretParam(array, Match().values, Match().types, Match().replaces);
	if (additing.length() == 0 || !Service::contains_fields(array, Match().values.keys()))
	{
		qDebug() << "Ошибка при вводе параметров!";
		return false;
	}

	QString hteam = Team::GetCommandId(db, array["hteam"]);
	QString ateam = Team::GetCommandId(db, array["ateam"]);
	QString stid = Stadium::GetStadiumId(db, array["stid"]);
	if (hteam.length() == 0 || ateam.length() == 0 || stid.length() == 0)
	{
		return false;
	}
	if (Team::isUsedTeam(db, array["hteam"], array["matchDate"]) || Team::isUsedTeam(db, array["ateam"], array["matchDate"]))
	{
		qDebug() << "Одна из команд уже проводит игру в этот день!";
		return false;
	}
	if (Stadium::isUsedStadium(db, stid, array["matchDate"]))
	{
		qDebug() << "На этот день стадион уже занят!";
		return false;
	}

	QSqlQuery sql(db);
	return sql.exec(query + hteam + ", " + ateam + ", " + stid + ", " + array["gH"]
		+ ", " + array["gA"] + ", '" + array["matchDate"] + "', 0");
}

bool Match::DeleteMatch(QSqlDatabase db, QString info)
{
	QStringList data = info.split('.', QString::SkipEmptyParts);

	if (data.size() != 3)
	{
		qDebug() << "Неверное количество параметров";
		return false;
	}
	QString date = data[0];
	QString hteam = Team::GetCommandId(db, data[1]);
	QString ateam = Team::GetCommandId(db, data[2]);

	if (hteam.length() == 0 || ateam.length() == 0)
	{
		return false;
	}
	QSqlQuery sql(db);
	return sql.exec("DELETE FROM dbo.MatchesView WHERE matchDate = '" + date + "' AND hteam = " + hteam + " AND ateam = " + ateam);
}

void Match::PrintMatches(const QVector<Match> &matches)
{
	qDebug() << "=============================================================";

	for(int i = 0; i < matches.size(); ++i)
	{
		Match p = Match(matches[i]);
		qDebug() << "#" << i << "\nДата: " << p.date.toString() << " " << p.hteam
				<< " " << p.gh << " : "
				 << p.ga << " " << p.ateam << "\nСтадион: " << p.stid << "\nГруппа: " << p.group << endl;
	}
	qDebug() << "=============================================================";
	qDebug() << endl;
}
