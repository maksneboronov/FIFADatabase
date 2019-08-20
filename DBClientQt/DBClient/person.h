#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>
#include <QMap>

#include "service.h"

class Person
{
public:
	Person();
	Person(const Person &pers);

	static QVector<Person> GetPersons(const QSqlDatabase &db);
	static void PrintPersons(const QVector<Person> &persons);
	static Person Auth(const QSqlDatabase &db, const QString &login, const QString &password);	
	static bool ChangePerson(QSqlDatabase db, QString passport, QMap<QString, QString> params);
	static bool DeletePerson(QSqlDatabase db, QString passport);
	static bool InsertPerson(QSqlDatabase db, QMap<QString, QString> params);
	static QString GetPersonId(QSqlDatabase db, QString passport);
	static bool IsManager(QSqlDatabase db, int id);

	int getId() { return id; }
	QString getName() { return name; }
	QString getLogin() { return login; }
	QString getPassport() { return passport; }
	QString getPhone() { return phone; }
	QString getAccess() { return access; }

	void setName(QString str) { name = str; }
	void setLogin(QString str) { login = str; }
	void setPassport(QString str) { passport = str; }
	void setPhone(QString str) { phone = str; }
	void setAccess(QString str) { access = str; }

private:
	int id;
	QString name, login, passport, phone, access;
	QMap<QString, QString> values;
	QMap<QString, bool> types, replaces;
};

#endif // PERSON_H
