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

#ifndef OPENURLDEFINITION_H
#define OPENURLDEFINITION_H

#include "actiondefinition.h"
#include "openurlinstance.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class OpenURLDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit OpenURLDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::TextParameterDefinition *url = new ActionTools::TextParameterDefinition(ActionTools::Name("url", tr("URL")), this);
			url->setTooltip(tr("The url to open"));
			addElement(url);

			addException(OpenURLInstance::FailedToOpenURL, tr("Failed to open URL"));
		}

		QString name() const													{ return QObject::tr("Open URL"); }
		QString id() const														{ return "ActionOpenURL"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Opens an URL"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new OpenURLInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/openurl.png"); }

	private:
		Q_DISABLE_COPY(OpenURLDefinition)
	};
}

#endif // OPENURLDEFINITION_H
