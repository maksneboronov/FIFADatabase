#include "person.h"

#include <QVariant>
#include <QDebug>
#include <QCryptographicHash>

using std::endl;
//QTextStream in(stdin);

Person::Person()
{
	values["persLogin"] = "[\\d\\w_]{1,30}";
	values["persName"] = "[\\d\\w]{1,30}";
	values["access"] = "[AMRS]{1}";
	values["phone"] = "[\\d]{1,11}";
	values["passport"] = "[\\d]{10}";
	values["persPassword"] = "[\\d\\w]{5,}";

	types["persLogin"] = true;
	types["persName"] = true;
	types["access"] = true;
	types["phone"] = true;
	types["passport"] = true;
	types["persPassword"] = true;

	replaces["persLogin"] = false;
	replaces["persName"] = true;
	replaces["access"] = false;
	replaces["phone"] = false;
	replaces["passport"] = false;
	replaces["persPassword"] = false;
}

Person::Person(const Person &pers)
{
	this->name = pers.name;
	this->access = pers.access;
	this->id = pers.id;
	this->phone = pers.phone;
	this->passport = pers.passport;
	this->login = pers.login;
}

bool Person::IsManager(QSqlDatabase db, int id)
	{
		QString query = "SELECT access FROM dbo.PersonsView WHERE id = " + QString::number(id);
		QSqlQuery sql(db);
		if (!sql.exec(query))
		{			
			return false;
		}
		sql.next();
		QString result = sql.value("access").toString();		
		return result == "M";
	}

QVector<Person> Person::GetPersons(const QSqlDatabase &db)
{
	QSqlQuery sql(db);
	if (!sql.exec("SELECT id, persLogin, persName, passport, access, phone FROM dbo.PersonsView"))
	{
		return QVector<Person>();
	}

	QVector<Person> persons;
	while(sql.next())
	{
		Person pers = Person();

		pers.access = sql.value("access").toString();
		pers.login = sql.value("persLogin").toString();
		pers.name = sql.value("persName").toString();
		pers.phone = sql.value("phone").toString();
		pers.passport = sql.value("passport").toString();
		pers.id = sql.value("id").toInt();

		persons.append(pers);
	}

	return persons;
}

Person Person::Auth(const QSqlDatabase &db, const QString &login, const QString &password)
{
	QSqlQuery sql(db);
	Person p;
	if (!sql.exec("SELECT id, persName, passport, phone, access, persPassword, persLogin \
				  FROM dbo.PersonsView \
				  WHERE persLogin LIKE '" + login + "'"))
	{
		p.id = -1;
		return p;
	}

	if (!sql.next())
	{
		p.id = -1;
		return p;
	}

	QString passwordRec = sql.value("persPassword").toString();
	QString res = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex());

	if (!res.contains(passwordRec))
	{
		p.id = -2;
		return p;
	}

	Person pers = Person();
	pers.access = sql.value("access").toString();
	pers.login = sql.value("persLogin").toString();
	pers.name = sql.value("persName").toString();
	pers.phone = sql.value("phone").toString();
	pers.passport = sql.value("passport").toString();
	pers.id = sql.value("id").toInt();
	return pers;
}

bool Person::ChangePerson(QSqlDatabase db, QString passport, QMap<QString, QString> params)
{
	QString query = "UPDATE dbo.PersonsView SET ";

	if (params.contains("persPassword"))
	{
		params["persPassword"] = QString(QCryptographicHash::hash(params["persPassword"].toUtf8(), QCryptographicHash::Md5).toHex());
	}

	QString additing = Service::IsCorretParam(params, Person().values, Person().types, Person().replaces);
	if (additing.trimmed().length() == 0)
	{
		return false;
	}

	query.append(additing + " WHERE passport = '" + passport + "'");

	QSqlQuery sql(db);
	return sql.exec(query);
}

bool Person::InsertPerson(QSqlDatabase db, QMap<QString, QString> params)
{
// add persons persLogin=asd123 persPassword=8126849 persName=jhsad passport=1234567899 phone=214 access=R
	QString query = "EXEC dbo.InsertNewPerson ";
	QString additing = Service::IsCorretParam(params, Person().values, Person().types, Person().replaces);
	if (additing.length() == 0 || !Service::contains_fields(params, Person().values.keys()))
	{
		qDebug() << "Ошибка при вводе параметров!";
		return false;
	}
	if (params.contains("persPassword"))
	{
		params["persPassword"] = QString(QCryptographicHash::hash(params["persPassword"].toUtf8(), QCryptographicHash::Md5).toHex());
	}
	QSqlQuery sql(db);
	return sql.exec(query + "'" + params["persName"] + "', '" +
		params["passport"] + "', '" + params["phone"] + "', '" + params["access"] +
		"', '" + params["persLogin"] + "', '" + params["persPassword"] + "', 0");
}

bool Person::DeletePerson(QSqlDatabase db, QString passport)
{
	QSqlQuery sql(db);
	return sql.exec("DELETE FROM dbo.PersonsView WHERE passport = '" + passport + "'");
}

QString Person::GetPersonId(QSqlDatabase db, QString passport)
{
	QSqlQuery sql(db);
	if (!sql.exec("SELECT id FROM dbo.PersonsView WHERE passport LIKE '" + passport + "'"))
	{
		qDebug() << "Ошибка выполнения запроса!";
		return "";
	}
	if (!sql.first())
	{
		qDebug() << "Пользователь не найден!";
		return "";
	}
	return QString::number(sql.value("id").toInt());
}

void Person::PrintPersons(const QVector<Person> &persons)
{
	qDebug() << "=============================================================";

	for(int i = 0; i < persons.size(); ++i)
	{
		Person p = Person(persons[i]);
		qDebug() << "#" << i << "\nИмя: " << p.getName() << "\nЛогин: " << p.getLogin() << "\nДолжность: "
				 << p.getAccess() << "\nПаспортные данные: " << p.getPassport() << "\nНомер телефона: " << p.getPhone() << endl;
	}
	qDebug() << "=============================================================";
	qDebug() << endl;
}
