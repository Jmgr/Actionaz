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

#ifndef SCRIPT_H
#define SCRIPT_H

#include "actiontools_global.h"
#include "version.h"
#include "scriptparameter.h"

#include <QVariant>
#include <QStringList>
#include <QHash>
#include <QStack>

class QIODevice;

namespace ActionTools
{
	class ActionInstance;
	class ActionFactory;

	class ACTIONTOOLSSHARED_EXPORT Script : public QObject
	{
		Q_OBJECT

	public:
		enum ReadResult
		{
			ReadSuccess,			// Ok
			ReadInternal,			// Internal error
			ReadBadSchema,			// Did not pass schema validation
			ReadBadScriptVersion	// Script version is newer than ours
		};

		Script(ActionFactory *actionFactory, QObject *parent = 0);
		~Script();

		void appendAction(ActionInstance *actionInstance)					{ mActionInstances.append(actionInstance); }
		ActionInstance *appendAction(const QString &actionDefinitionId);
		ActionInstance *actionAt(int line) const;
		void insertAction(int line, ActionInstance *actionInstance);
		void setAction(int line, ActionInstance *actionInstance);
		void removeActions(int line, int count);
		void removeAction(int line);
		void removeAction(ActionInstance *actionInstance);
		void removeAll();
		void moveAction(int startLine, int endLine);
		int actionCount() const												{ return mActionInstances.count(); }
		int labelLine(const QString &label) const;
		bool hasEnabledActions() const;

		QSet<int> usedActions() const;

		bool write(QIODevice *device, const Tools::Version &programVersion, const Tools::Version &scriptVersion);
		ReadResult read(QIODevice *device, const Tools::Version &scriptVersion);
		bool validateContent(const QString &content);
		const QString &statusMessage() const								{ return mStatusMessage; }
		int line() const													{ return mLine; }
		int column() const													{ return mColumn; }
		const QString &programName() const									{ return mProgramName; }
		const Tools::Version &programVersion() const						{ return mProgramVersion; }
		const Tools::Version &scriptVersion() const							{ return mScriptVersion; }
		const QString &os() const											{ return mOs; }
		const QStringList &missingActions() const							{ return mMissingActions; }
		int pauseBefore() const												{ return mPauseBefore; }
		int pauseAfter() const												{ return mPauseAfter; }

		void addParameter(const ScriptParameter &parameter)					{ mParameters.append(parameter); }
		int parameterCount() const											{ return mParameters.count(); }
		const ScriptParameter &parameter(int index) const					{ return mParameters.at(index); }
		void removeAllParameters()											{ mParameters.clear(); }
		QList<ScriptParameter> parameters() const							{ return mParameters; }
		void setPauseBefore(int pauseBefore)								{ mPauseBefore = pauseBefore; }
		void setPauseAfter(int pauseAfter)									{ mPauseAfter = pauseAfter; }

		void addProcedure(const QString &procedureName, int line)			{ mProcedures.insert(procedureName, line); }
		int findProcedure(const QString &procedureName) const				{ return mProcedures.value(procedureName, -1); }
		void clearProcedures()												{ mProcedures.clear(); }

		void addProcedureCall(int callerLine)								{ mCallStack.push(callerLine); }
		bool hasProcedureCall() const										{ return !mCallStack.isEmpty(); }
		int popProcedureCall()												{ return mCallStack.pop(); }
		void clearCallStack()												{ mCallStack.clear(); }

		int actionIndexFromRuntimeId(qint64 runtimeId) const;
		QStringList procedureNames() const;
		QStringList labels() const;

	private:
		QList<ScriptParameter> mParameters;
		QList<ActionInstance *> mActionInstances;
		ActionFactory *mActionFactory;
		QString mStatusMessage;
		int mLine;
		int mColumn;
		QString mProgramName;
		Tools::Version mProgramVersion;
		Tools::Version mScriptVersion;
		QString mOs;
		QStringList mMissingActions;
		int mPauseBefore;
		int mPauseAfter;
		QHash<QString, int> mProcedures;
		QStack<int> mCallStack;

		Q_DISABLE_COPY(Script)
	};
}

#endif // SCRIPT_H
