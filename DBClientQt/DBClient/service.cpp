#include "service.h"

Service::Service()
{
}

QString Service::IsCorretParam(QMap<QString, QString> array, QMap<QString, QString> values, QMap<QString, bool> types, QMap<QString, bool> replaces)
{
	int i = 0;
	QString query = " ";
	foreach (QString key, array.keys())
	{
		if (!values.contains(key))
		{
			qDebug() << "Неизвестный параметр - " << key;
			return "";
		}

		QRegExp reg(values[key]);
		if (!reg.exactMatch(array[key]))
		{
			qDebug() << "Ошибка при вводе параметра - " << array[key];			
			return "";
		}
		QString newValue = replaces[key] ? array[key].replace("_", " ") : array[key];
		if (types[key])
		{
			query.append(key + " = '" + newValue + "' ");
		}
		else
		{
			query.append(key + " = " + newValue + " ");
		}
		if (++i < array.size())
		{
			query.append(", ");
		}
	}
	return query;
}

bool Service::contains_fields(QMap<QString, QString> array, QList<QString> fields)
{
	QList<QString> keys = array.keys();
	foreach(QString key, fields)
	{
		if (!keys.contains(key))
		{
			qDebug() << "Не найден параметр - " << key;
			return false;
		}
	}
	return true;
}
