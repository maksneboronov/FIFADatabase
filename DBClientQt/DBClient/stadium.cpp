#include "stadium.h"


#include <QVariant>
#include <QDebug>
#include "service.h"
#include <QSqlError>
#include "person.h"

Stadium::Stadium()
{
	values["stName"] = "[\\d\\w]{1,30}";
	values["town"] = "[\\d\\w]{1,30}";
	values["seatCount"] = "[\\d]{1,5}";
	values["sectorCount"] = "[\\d]{1,5}";
	values["pid"] = "[\\d]{10}";

	types["stName"] = true;
	types["town"] = true;
	types["seatCount"] = false;
	types["sectorCount"] = false;
	types["pid"] = false;

	replaces["stName"] = true;
	replaces["town"] = true;
	replaces["seatCount"] = false;
	replaces["sectorCount"] = false;
	replaces["pid"] = false;
}

Stadium::Stadium(const Stadium &pers)
{
	this->id = pers.id;
	this->pid = pers.id;
	this->sTown = pers.sTown;
	this->sName = pers.sName;
	this->seatCount = pers.seatCount;
	this->sectorCount = pers.sectorCount;
}

bool Stadium::isUsedStadium(QSqlDatabase db, QString stid, QString date)
{
	QString query = "SELECT * FROM dbo.Matches WHERE mdate = '" + date + "' AND Stid = " + stid;
	QSqlQuery sql(db);
	if (!sql.exec(query))
	{
		qDebug() << "Ошибка выполнения запроса!";
	}
	return sql.first();
}

QString Stadium::GetStadiumId(QSqlDatabase db, QString name)
{
	QSqlQuery sqlP(db);
	if (!sqlP.exec("SELECT id FROM dbo.StadiumsView where stName = '" + name.replace('_', ' ') + "'"))
	{
		qDebug() << "Ошибка выполнения запроса!";
		return "";
	}
	if (!sqlP.first())
	{
		qDebug() << "Стадион " << name << " не найден!";
		return "";
	}
	QString result = QString::number(sqlP.value("id").toInt());
	sqlP.clear();
	return result;
}

QVector<Stadium> Stadium::GetStadiums(const QSqlDatabase &db)
{
	QSqlQuery sql(db);
	if (!sql.exec("SELECT id, pid, stName, town, seatCount, sectorCount FROM dbo.StadiumsView"))
	{
		return QVector<Stadium>();
	}

	QVector<Stadium> stadiums;
	while(sql.next())
	{
		Stadium st = Stadium();

		st.sName = sql.value("stName").toString();
		st.sTown = sql.value("town").toString();
		st.id = sql.value("id").toInt();
		st.pid = sql.value("pid").toInt();
		st.seatCount = sql.value("seatCount").toInt();
		st.sectorCount = sql.value("sectorCount").toInt();

		stadiums.append(st);
	}	

	return stadiums;
}

bool Stadium::ChangeStadium(QSqlDatabase db, QString sName, QMap<QString, QString> params)
{
	sName = sName.replace('_', ' ');
	QString query = "UPDATE dbo.StadiumsView SET ";

	if (params.contains("pid"))
	{
		QSqlQuery sqlP(db);
		if (!sqlP.exec("SELECT id FROM dbo.PersonsView where passport = '" + params["pid"] + "'"))
		{
			qDebug() << "Ошибка!";
			return false;
		}
		sqlP.next();

		params["pid"] = QString::number(sqlP.value("id").toInt());		

		if (!Person::IsManager(db, sqlP.value("id").toInt()))
		{
			qDebug() << "Назначаемый человек - не менеджер!";
			return false;
		}
	}

	QString additing = Service::IsCorretParam(params, Stadium().values, Stadium().types, Stadium().replaces);
	if (additing.trimmed().length() == 0)
	{
		return false;
	}

	query.append(additing + " WHERE stName = '" + sName + "'");

	QSqlQuery sql(db);
	return sql.exec(query);
}

bool Stadium::InsertStadium(QSqlDatabase db, QMap<QString, QString> array)
{
	// add stadiums pid=6230765412 stName=asda12311 town=asdj seatCount=1 sectorCount=0
	QString query = "EXEC dbo.InsertStadium ";
	QString additing = Service::IsCorretParam(array, Stadium().values, Stadium().types, Stadium().replaces);
	if (additing.length() == 0 || !Service::contains_fields(array, Stadium().values.keys()))
	{
		qDebug() << "Ошибка при вводе параметров!";
		return false;
	}

	QSqlQuery sqlP(db);
	if (!sqlP.exec("SELECT id FROM dbo.PersonsView where passport = '" + array["pid"] + "'"))
	{
		qDebug() << "Ошибка!";
		return false;
	}
	sqlP.next();

	int pid = sqlP.value("id").toInt();
	sqlP.clear();
	if (!Person::IsManager(db, pid))
	{
		qDebug() << "Назначаемый человек - не менеджер!";
		return false;
	}

	QSqlQuery sqlS(db);
	return sqlS.exec(query + QString::number(pid) + " , " + array["seatCount"] + " , " + array["sectorCount"] + " , '" + array["stName"] +
		"' , '" + array["town"] + "' , 0x , 0;");
}

bool Stadium::DeleteStadium(QSqlDatabase db, QString sName)
{
	sName = sName.replace('_', ' ');
	QSqlQuery sql(db);
	return sql.exec("DELETE FROM dbo.StadiumsView WHERE stName = '" + sName + "'");
}

void Stadium::PrintStadiums(const QVector<Stadium> &stadiums)
{
	qDebug() << "=============================================================";

	for(int i = 0; i < stadiums.size(); ++i)
	{
		Stadium p = Stadium(stadiums[i]);
		qDebug() << "#" << i << "\nНазвание: " << p.sName << "\nГород: " << p.sTown
				<< "\nМенеджер: " << p.pid << "\nКоличество мест: "
				 << p.seatCount << "\nКоличество секторов: " << p.sectorCount << endl;
	}
	qDebug() << "=============================================================";
	qDebug() << endl;
}
