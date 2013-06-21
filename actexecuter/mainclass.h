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

#ifndef MAINCLASS_H
#define MAINCLASS_H

#include "version.h"

#include <QObject>
#include <QUrl>

class Executer;
class QIODevice;
class QNetworkAccessManager;
class QNetworkReply;
class QTemporaryFile;

class MainClass : public QObject
{
	Q_OBJECT
	
public:
	enum ExecutionMode
	{
		Unknown,
		Code,
		Script
	};
	
	MainClass();
	
	bool start(ExecutionMode executionMode, QIODevice *device, const QString &filename);
	bool start(ExecutionMode executionMode, const QUrl &url);
	
private slots:
	void downloadFinished();

private:
	Executer *mExecuter;
	QNetworkAccessManager *mNetworkAccessManager;
	QNetworkReply *mNetworkReply;
	ExecutionMode mExecutionMode;
	QUrl mUrl;
};

#endif // MAINCLASS_H
