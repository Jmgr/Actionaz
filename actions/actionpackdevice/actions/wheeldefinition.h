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

#ifndef WHEELDEFINITION_H
#define WHEELDEFINITION_H

#include "actiondefinition.h"
#include "wheelinstance.h"
#include "numberparameterdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WheelDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT
	
	public:
		explicit WheelDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::NumberParameterDefinition *intensity = new ActionTools::NumberParameterDefinition(ActionTools::Name("intensity", tr("Intensity")), this);
			intensity->setTooltip(tr("Intensity of the movement, positive is up, negative is down"));
			intensity->setMinimum(std::numeric_limits<int>::min());
			intensity->setMaximum(std::numeric_limits<int>::max());
			addElement(intensity);
			
			addException(WheelInstance::FailedToSendInputException, tr("Send input failure"));
		}
	
		QString name() const													{ return QObject::tr("Wheel"); }
		QString id() const														{ return "ActionWheel"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Emulates the mouse wheel"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WheelInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Device; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/wheel.png"); }
		bool requirementCheck(QStringList &missingRequirements) const			{ return requirementCheckXTest(missingRequirements); }
	
	private:
		Q_DISABLE_COPY(WheelDefinition)
	};
}

#endif // WHEELDEFINITION_H
