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

#include "windowinstance.h"
#include "windowhandle.h"

#include <QRegExp>

namespace Actions
{
	ActionTools::StringListPair WindowInstance::actions = qMakePair(
			QStringList() << "close" << "killProcess" << "setForeground" << "minimize" << "maximize" << "move" << "resize",
			QStringList()
			<< QT_TRANSLATE_NOOP("WindowInstance::actions", "Close")
			<< QT_TRANSLATE_NOOP("WindowInstance::actions", "Kill process")
			<< QT_TRANSLATE_NOOP("WindowInstance::actions", "Set foreground")
			<< QT_TRANSLATE_NOOP("WindowInstance::actions", "Minimize")
			<< QT_TRANSLATE_NOOP("WindowInstance::actions", "Maximize")
			<< QT_TRANSLATE_NOOP("WindowInstance::actions", "Move")
			<< QT_TRANSLATE_NOOP("WindowInstance::actions", "Resize"));

	WindowInstance::WindowInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent)
	{
	}

	void WindowInstance::startExecution()
	{
		bool ok = true;

		QString title = evaluateString(ok, "title");
		Action action = evaluateListElement<Action>(ok, actions, "action");
		bool useBorders = evaluateBoolean(ok, "useBorders");

		if(!ok)
			return;

		QPoint movePosition;
		int resizeWidth = 0;
		int resizeHeight = 0;

		if(action == Move)
		{
			movePosition = evaluatePoint(ok, "movePosition");
			if(!ok)
				return;
		}
		else if(action == Resize)
		{
			resizeWidth = evaluateInteger(ok, "resizeWidth");
			resizeHeight = evaluateInteger(ok, "resizeHeight");
			if(!ok)
				return;
		}

		ActionTools::WindowHandle foundWindow = ActionTools::WindowHandle::findWindow(QRegExp(title, Qt::CaseSensitive, QRegExp::WildcardUnix));

		if(!foundWindow.isValid())
		{
			setCurrentParameter("title");
			emit executionException(CannotFindWindowException, tr("Cannot find any window matching \"%1\"").arg(title));
			return;
		}

		bool result = true;

		switch(action)
		{
		case Close:
			result = foundWindow.close();
			break;
		case KillProcess:
			result = foundWindow.killCreator();
			break;
		case SetForeground:
			result = foundWindow.setForeground();
			break;
		case Minimize:
			result = foundWindow.minimize();
			break;
		case Maximize:
			result = foundWindow.maximize();
			break;
		case Move:
			result = foundWindow.move(movePosition);
			break;
		case Resize:
			result = foundWindow.resize(QSize(resizeWidth, resizeHeight), useBorders);
			break;
		}

		if(!result)
		{
			setCurrentParameter("action");
			emit executionException(ActionFailedException, tr("\"%1\" failed").arg(actions.second[action]));
			return;
		}

		emit executionEnded();
	}
}
