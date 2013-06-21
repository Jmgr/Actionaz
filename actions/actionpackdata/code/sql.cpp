/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "sql.h"
#include "code/codetools.h"

#include <QScriptValueIterator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QUuid>
#include <QStringList>
#include <QSet>

namespace Code
{
	QScriptValue Sql::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 1)
		{
			throwError(context, engine, "NoDatabaseDriverError", tr("Please specify the database driver that should be used"));
			return engine->undefinedValue();
		}

		Driver driver = static_cast<Driver>(context->argument(0).toInt32());

		return CodeClass::constructor(new Sql(driver), context, engine);
	}

	QScriptValue Sql::drivers(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)

		QStringList driverNames = QSqlDatabase::drivers();
		QSet<Driver> driverList;

		for(int index = 0; index < driverNames.size(); ++index)
		{
			const QString &driverNameToInclude = driverNames.at(index);

			for(int driverIndex = 0; driverIndex < DriverCount; ++driverIndex)
			{
				if(driverName(static_cast<Driver>(driverIndex)) == driverNameToInclude)
					driverList.insert(static_cast<Driver>(driverIndex));
			}
		}

		QScriptValue back = engine->newArray(driverList.size());
		int index = 0;
		foreach(const Driver &driver, driverList)
		{
			back.setProperty(index, driver);

			++index;
		}

		return back;
	}

	void Sql::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassGlobalFunctionToScriptEngine<Sql>(&drivers, "drivers", scriptEngine);
	}

	Sql::Sql(Driver driver)
		: CodeClass(),
		mDatabase(new QSqlDatabase)
	{
		mDriverName = driverName(driver);
	}

	Sql::~Sql()
	{
		QString connectionName = mDatabase->connectionName();

		mDatabase->close();
		delete mDatabase;

        QSqlDatabase::removeDatabase(connectionName);
    }

    bool Sql::equals(const QScriptValue &other) const
    {
        if(other.isUndefined() || other.isNull())
            return false;

        QObject *object = other.toQObject();
        if(Sql *otherSql = qobject_cast<Sql*>(object))
            return (otherSql == this || otherSql->mDatabase == mDatabase);

        return false;
    }

	QScriptValue Sql::connect(const QScriptValue &parameters) const
	{
		mDatabase->close();

		if(!QSqlDatabase::isDriverAvailable(mDriverName))
		{
			throwError("DatabaseDriverUnavailableError", tr("The requested database driver is not available"));
			return thisObject();
		}

		*mDatabase = QSqlDatabase::addDatabase(mDriverName, QUuid::createUuid().toString());
		if(!mDatabase->isValid())
		{
			throwError("DatabaseDriverUnavailableError", tr("The requested database driver is not available"));
			return thisObject();
		}

		QScriptValueIterator it(parameters);
		QString hostName;
		int port = 0;
		QString databaseName;
		QString userName;
		QString password;
		QString options;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "hostName")
				hostName = it.value().toString();
			else if(it.name() == "port")
				port = it.value().toInteger();
			else if(it.name() == "databaseName")
				databaseName = it.value().toString();
			else if(it.name() == "userName")
				userName = it.value().toString();
			else if(it.name() == "password")
				password = it.value().toString();
			else if(it.name() == "options")
				options = it.value().toString();
		}

		mDatabase->setHostName(hostName);
		if(port != 0)
			mDatabase->setPort(port);
		mDatabase->setDatabaseName(databaseName);
		mDatabase->setConnectOptions(options);
		if(!mDatabase->open(userName, password))
		{
			throwError("ConnectionError", tr("Unable to establish a connection to the database"));
			return thisObject();
		}

		return thisObject();
	}

	QScriptValue Sql::prepare(const QString &queryString, const QScriptValue &parameters)
	{
		mQuery = QSqlQuery(*mDatabase);
		mQuery.setForwardOnly(true);
		if(!mQuery.prepare(queryString))
		{
			throwError("PrepareQueryError", tr("Failed to prepare the query"));
			return thisObject();
		}

		QScriptValueIterator it(parameters);
		while(it.hasNext())
		{
			it.next();

			mQuery.bindValue(it.name(), it.value().toString());
		}

		return thisObject();
	}

	QScriptValue Sql::execute(const QString &queryString)
	{
		if(!queryString.isEmpty())
		{
			mQuery = QSqlQuery(queryString, *mDatabase);
			mQuery.setForwardOnly(true);
		}

		if(!mQuery.exec())
		{
			QSqlError error = mQuery.lastError();
			throwError("ExecuteQueryError", tr("Failed to execute the query : %1").arg(error.text()));
			return thisObject();
		}

		return thisObject();
	}

	QScriptValue Sql::fetchResult(IndexStyle indexStyle)
	{
		if(!mQuery.isSelect())
		{
			throwError("FetchError", tr("Cannot fetch the result of a non-select query"));
			return thisObject();
		}

		int size = mQuery.size();
		if(size == -1)
			size = 0;

		QScriptValue back = engine()->newArray(size);

		switch(indexStyle)
		{
		case IndexNumber:
			{
				QSqlRecord record = mQuery.record();
				for(int index = 0; mQuery.next(); ++index)
				{
					QScriptValue row = engine()->newArray();
					for(int columnIndex = 0; columnIndex < record.count(); ++columnIndex)
					{
						row.setProperty(columnIndex, engine()->newVariant(mQuery.value(columnIndex)));
					}

					back.setProperty(index, row);
				}
			}
			break;
		case IndexName:
			{
				for(int index = 0; mQuery.next(); ++index)
				{
					QSqlRecord record = mQuery.record();
					QScriptValue row = engine()->newArray(record.count());
					for(int columnIndex = 0; columnIndex < record.count(); ++columnIndex)
					{
						row.setProperty(record.fieldName(columnIndex), engine()->newVariant(record.value(columnIndex)));
					}

					back.setProperty(index, row);
				}
			}
			break;
		}

		return back;
	}

	QScriptValue Sql::disconnect() const
	{
		mDatabase->close();

		return thisObject();
	}

	QString Sql::driverName(Driver driver)
	{
		switch(driver)
		{
		case SQLite2:
			return "QSQLITE2";
		case SQLite:
			return "QSQLITE";
		case PostgreSQL:
			return "QPSQL";
		case MySQL:
			return "QMYSQL";
		case ODBC:
			return "QODBC";
		case InterBase:
			return "QIBASE";
		case OCI:
			return "QOCI";
		case TDS:
			return "QTDS";
		case DB2:
			return "QDB2";
		default:
			return QString();
		}
	}
}
