#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>
#include <QMap>

class Team
{
public:
	Team();
	Team(const Team &pers);

	static QVector<Team> GetTeams(const QSqlDatabase &db);
	static void PrintTeams(const QVector<Team> &persons);
	static bool ChangeTeam(QSqlDatabase db, QString passport, QMap<QString, QString> params);
	static bool DeleteTeam(QSqlDatabase db, QString passport);
	static bool InsertTeam(QSqlDatabase db, QMap<QString, QString> params);
	static void GetTable(QSqlDatabase db);
	static QString GetCommandId(QSqlDatabase db, QString name);
	static bool isUsedTeam(QSqlDatabase, QString name, QString date);

	int getId() { return id; }

private:
	int id, games, wons, loses, drawns, gf, ga, points;
	QString group, teamName, coach;
	QMap<QString, QString> values;
	QMap<QString, bool> types, replaces;
};

#endif // TEAM_H
