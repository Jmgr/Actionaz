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

#ifndef CODEEXECUTER_H
#define CODEEXECUTER_H

#include "executer.h"

namespace LibExecuter
{
	class ScriptAgent;
}

class QScriptEngine;
class QScriptEngineDebugger;
class QMainWindow;

class CodeExecuter : public Executer
{
    Q_OBJECT
	
public:
    explicit CodeExecuter(QObject *parent = 0);
	
	bool start(QIODevice *device, const QString &filename);
	
private slots:
	void onEvaluationResumed();
	void onEvaluationPaused();
	void stopExecution();
	
private:
	QScriptEngine *mScriptEngine;
	LibExecuter::ScriptAgent *mScriptAgent;
	QScriptEngineDebugger *mScriptEngineDebugger;
	QMainWindow *mDebuggerWindow;
};

#endif // CODEEXECUTER_H
