#ifndef STADIUM_H
#define STADIUM_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>
#include <QMap>

class Stadium
{
public:
	Stadium();
	Stadium(const Stadium &pers);

	static QVector<Stadium> GetStadiums(const QSqlDatabase &db);
	static void PrintStadiums(const QVector<Stadium> &stadiums);
	static bool ChangeStadium(QSqlDatabase db, QString sName, QMap<QString, QString> params);
	static bool DeleteStadium(QSqlDatabase db, QString sName);
	static bool InsertStadium(QSqlDatabase db, QMap<QString, QString> params);
	static QString GetStadiumId(QSqlDatabase db, QString name);
	static bool isUsedStadium(QSqlDatabase, QString stid, QString date);

	int getId() { return id; }

private:
	int id, seatCount, sectorCount, pid;
	QString sTown, sName;
	QMap<QString, QString> values;
	QMap<QString, bool> types, replaces;
};

#endif // STADIUM_H
