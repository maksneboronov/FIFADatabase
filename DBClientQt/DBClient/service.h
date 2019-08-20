#ifndef SERVICE_H
#define SERVICE_H


#include <QMap>
#include <conio.h>
#include <QRegExp>
#include <QTextStream>
#include <QDebug>

class Service
{
public:
	Service();

	static QString IsCorretParam(QMap<QString, QString> array, QMap<QString, QString> values, QMap<QString, bool> types, QMap<QString, bool> replaces);
	static bool contains_fields(QMap<QString, QString> array, QList<QString> fields);	
private:
	static QMap<QString, QString> values;
	static QMap<QString, bool> types, replaces;
};

#endif // SERVICE_H
