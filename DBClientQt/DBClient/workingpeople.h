#ifndef WORKINGPEOPLE_H
#define WORKINGPEOPLE_H


#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>
#include <QMap>

#include "service.h"

class WorkingPeople
{
public:
	WorkingPeople();
	WorkingPeople(const WorkingPeople &pers);

	static QVector<WorkingPeople> GetWorkingPeople(const QSqlDatabase &db, QString info);
	static void PrintWorkingPeople(const QVector<WorkingPeople> &persons);
	static bool DeleteWorkingPeople(QSqlDatabase db, QString match, QString passport);
	static bool InsertWorkingPeople(QSqlDatabase db, QMap<QString, QString> params);
	static bool IsUsedMan(QSqlDatabase, QString pid, QString date);

private:
	int id;
	QString name, login, phone, access, passp;
	QMap<QString, QString> values;
	QMap<QString, bool> types, replaces;
};

#endif // WORKINGPEOPLE_H
