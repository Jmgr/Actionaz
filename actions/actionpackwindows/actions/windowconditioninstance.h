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

#ifndef WINDOWCONDITIONINSTANCE_H
#define WINDOWCONDITIONINSTANCE_H

#include "actioninstance.h"
#include "stringlistpair.h"
#include "windowhandle.h"
#include "ifactionvalue.h"

#include <QTimer>

namespace Actions
{
	class WindowConditionInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Condition)

	public:
		enum Condition
		{
			Exists,
			DontExists
		};

		WindowConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);

		static ActionTools::StringListPair conditions;

		void stopExecution();
		void startExecution();

	private slots:
		void checkWindow();

	private:
		ActionTools::WindowHandle findWindow();

		QRegExp mTitleRegExp;
		ActionTools::IfActionValue mIfTrue;
		Condition mCondition;
		QString mPosition;
		QString mSize;
		QString mXCoordinate;
		QString mYCoordinate;
		QString mWidth;
		QString mHeight;
		QString mProcessId;
		QTimer mTimer;
		ActionTools::WindowHandle mTestedWindowHandle;

		Q_DISABLE_COPY(WindowConditionInstance)
	};
}

#endif // WINDOWCONDITIONINSTANCE_H
