#ifndef MATCH_H
#define MATCH_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>
#include <QMap>
#include <QDate>

class Match
{
public:
	Match();
	Match(const Match &pers);

	static QVector<Match> GetMatches(const QSqlDatabase &db);
	static void PrintMatches(const QVector<Match> &persons);
	static bool ChangeMatch(QSqlDatabase db, QString passport, QMap<QString, QString> params);
	static bool DeleteMatch(QSqlDatabase db, QString passport);
	static bool InsertMatch(QSqlDatabase db, QMap<QString, QString> params);
	static QString GetMatchId(QSqlDatabase db, QString hteam, QString ateam, QString date);

	int getId() { return id; }
private:
	int id, gh, ga, stid;
	QString group, hteam, ateam;
	QDate date;
	QMap<QString, QString> values;
	QMap<QString, bool> types, replaces;
};

#endif // MATCH_H
