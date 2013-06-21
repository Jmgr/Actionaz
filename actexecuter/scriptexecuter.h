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

#ifndef SCRIPTEXECUTER_H
#define SCRIPTEXECUTER_H

#include "executer.h"

namespace ActionTools
{
	class Script;
}

namespace LibExecuter
{
	class Executer;
}

class ScriptExecuter : public Executer
{
    Q_OBJECT
	
public:
    explicit ScriptExecuter(QObject *parent = 0);
	
	bool start(QIODevice *device, const QString &filename);

public slots:
	void executionStopped();
	void scriptError(int actionIndex, const QString &parameter, const QString &error);
	
private:
	ActionTools::Script *mScript;
	LibExecuter::Executer *mExecuter;
};

#endif // SCRIPTEXECUTER_H
