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

#ifndef CODEACTIONAZ_H
#define CODEACTIONAZ_H

#include "executer_global.h"
#include "version.h"

#include <QObject>
#include <QScriptValue>

class QScriptContext;
class QScriptEngine;

namespace LibExecuter
{
	class EXECUTERSHARED_EXPORT CodeActionaz : public QObject
	{
		Q_OBJECT

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		static QScriptValue version(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue scriptVersion(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue isActExec(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue isActionaz(QScriptContext *context, QScriptEngine *engine);

		static void setActExec(bool isActExec);
		static void setActionazVersion(const Tools::Version &version);
		static void setScriptVersion(const Tools::Version &version);
	private:
		static bool mIsActExec;
		static Tools::Version mActionazVersion;
		static Tools::Version mScriptVersion;
	};
}

#endif // CODEACTIONAZ_H
